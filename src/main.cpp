#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>
#include <sstream>

#include <Quaternion.h>
#include <Texture.h>
#include "collision/Collider.h"

#include "Renderer.h"
#include "timer.h"
#include "ParticleGenerator.h"
#include "ResourceManager.h"
#include "FireParticle.h"

using namespace std;
using namespace chag;


#define SCREEN_WIDTH			1028
#define SCREEN_HEIGHT			800

#define TICK_PER_SECOND  50


//*****************************************************************************
//	Global variables
//*****************************************************************************
bool paused = false;				// Tells us wether sun animation is paused
float currentTime = 0.0f;			// Tells us the current time
float timeSinceDraw = 0.0f;

const float3 vUp = make_vector(0.0f, 1.0f, 0.0f);
float3 startPosSun = make_vector(30.1f, 450.0f, 0.1f);


//*****************************************************************************
//	OBJ Model declarations
//*****************************************************************************
GameObject world;
Mesh worldCollision;
GameObject car;
GameObject factory;
GameObject water;
GameObject spider;
GameObject lamp;
GameObject lamp2;
GameObject lamp3;
GameObject normalTest;
GameObject normalTestWithout;
ParticleGenerator* gen;

Scene scene;


//*****************************************************************************
//	Cube Maps
//*****************************************************************************
CubeMapTexture* reflectionCubeMap;
Skybox* skybox;
Fbo cMapAll;


//*****************************************************************************
//	Camera state variables (updated in motion())
//*****************************************************************************
float camera_theta = (float) (M_PI / 1.0f);
float camera_phi = (float) (M_PI / 4.0f);
float camera_r = 30.0f; 
float camera_target_altitude = 5.2f; 

//****************************************************************************
//	Input state variables
//*****************************************************************************
bool leftDown = false;
bool middleDown = false;
bool rightDown = false;
int prev_x = 0;
int prev_y = 0;
bool keysDown[256];


//*****************************************************************************
//	Camera
//*****************************************************************************
Camera *cubeMapCameras[6];
Camera *sunCamera;
Camera *playerCamera;
int camera = 6;

//*****************************************************************************
//	Car object variables
//*****************************************************************************
struct Car{
  float3 frontDir;
  float3 upDir;
  float3 location;
  float3 wheel1, wheel2, wheel3,  wheel4;

  float rotationSpeed;
  float moveSpeed;
  float angley, anglez, anglex;
  float lengthx, lengthz;

  Car() {
    frontDir = make_vector(0.0f, 0.0f, 1.0f);
    upDir = make_vector(0.0f, 1.0f, 0.0f);
    location = make_vector(0.0f, 10.0f, 0.0f);
    wheel1 = make_vector( 1.2f, 0.0f,  1.5f);
    wheel2 = make_vector( 1.2f, 0.0f, -1.5f);
    wheel3 = make_vector(-0.8f, 0.0f,  1.5f);
    wheel4 = make_vector(-0.8f, 0.0f, -1.5f);

    rotationSpeed = (float) (2 * M_PI / 180);
    moveSpeed = 0.5;
    angley = 0; anglez = 0; anglex = 0;
    lengthx = 2; lengthz = 3;
  }
} carLoc;


Renderer *renderer;
//*****************************************************************************
//	Collision objects
//*****************************************************************************
Collider *collider;
Octree *octTree;
bool hasChanged = true;


//*****************************************************************************
//	Function declarations
//*****************************************************************************
void drawCubeMap(Fbo fbo, Scene scene);
void createCubeMaps();
void createMeshes();
void createCameras();
void createLights();
void createEffects();

float degreeToRad(float degree);
float radToDegree(float rad);
float3 sphericalToCartesian(float theta, float phi, float r);

void updatePlayer();


//*****************************************************************************
//	Function implementation
//****************************************************************************
void checkIntersection() {
	float3 upVec = make_vector(0.0f, 1.0f, 0.0f);

	//Calculate intersections
	float3x3 rot = make_rotation_y<float3x3>(carLoc.angley);
	utils::Timer timer;
	timer.start();
	float a = collider->rayIntersection(carLoc.location + rot * carLoc.wheel1, -upVec);
	float b = collider->rayIntersection(carLoc.location + rot * carLoc.wheel2, -upVec);
	float c = collider->rayIntersection(carLoc.location + rot * carLoc.wheel3, -upVec);
	float d = collider->rayIntersection(carLoc.location + rot * carLoc.wheel4, -upVec);
	timer.stop();

	stringstream timeMessage;
	timeMessage << "Tested 4 ray/aabb intersections in " << timer.getElapsedTime() << " ms";
	Logger::logDebug(timeMessage.str());
	if (a == 0 && b == 0 && c == 0 && d == 0) {
		return;
	}
	//Calculate 3d points of intersection
	float3 af = carLoc.wheel1 - (upVec * a);
	float3 bf = carLoc.wheel2 - (upVec * b);
	float3 cf = carLoc.wheel3 - (upVec * c);
	float3 df = carLoc.wheel4 - (upVec * d);

	//Calculate new up vector
	float3 vABa = af - bf;
	float3 vCBa = cf - bf;
	float3 newUpa = cross(vABa, vCBa);

	float3 vABb = bf - cf;
	float3 vCBb = df - cf;
	float3 newUpb = cross(vABb, vCBb);

	float3 halfVector = normalize(newUpa - newUpb);
	carLoc.upDir = -(rot * halfVector);

	

	//Change wheel locations 
	carLoc.wheel1 += upVec * a;
	carLoc.wheel2 += upVec * b;
	carLoc.wheel3 += upVec * c;
	carLoc.wheel4 += upVec * d;

	float3 frontDir = normalize(carLoc.frontDir);
	float3 rightDir = normalize(cross(frontDir, vUp));

	float anglex = -(degreeToRad(90.0f) - acosf(dot(normalize(carLoc.upDir), frontDir)));
	float anglez = (degreeToRad(90.0f) - acosf(dot(normalize(carLoc.upDir), rightDir)));
	
	Quaternion qatX = make_quaternion_axis_angle(rightDir, anglex);
	Quaternion qatZ = make_quaternion_axis_angle(make_rotation_y<float3x3>(-carLoc.angley) * frontDir, anglez);

	float4 newLoc = makematrix(qatX) * makematrix(qatZ) * make_vector4(carLoc.wheel1, 1.0f);

	carLoc.location += make_vector(0.0f, carLoc.wheel1.y + (carLoc.wheel1.y - newLoc.y), 0.0f);
	//printf("%f\n", carLoc.wheel1.y + (carLoc.wheel1.y - newLoc.y));
}

void display(void)
{

	Camera *cam;
	if (camera == 6) {
		cam = playerCamera;
	}
	else if (camera == 7) {
		cam = sunCamera;
	}
	else
	{
		cam = cubeMapCameras[camera];
	}
	renderer->drawScene(*cam, scene, currentTime);
	//drawCubeMap(cMapAll, scene);
	
	renderer->swapBuffer();
}

void handleKeys(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 27:   
		exit(0);
	case 32:
		paused = !paused;
		break;
	case 'w':
	case 'W':
		keysDown[(int)'w'] = true;
		break;
	case 'a':
	case 'A': 
		keysDown[(int)'a'] = true;
		break;
	case 's':   
	case 'S':
		keysDown[(int)'s'] = true;
		break;
	case 'd':   
	case 'D':
		keysDown[(int)'d'] = true;
		break;
	}
}

void handleKeysRelease(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:   
		exit(0);
	case 32:
		paused = !paused;
		break;
	case 'w':
	case 'W':
		keysDown[(int)'w'] = false;
		break;
	case 'a':
	case 'A':
		keysDown[(int)'a'] = false;
		break;
	case 's':
	case 'S':
		keysDown[(int)'s'] = false;
		break;
	case 'd':
	case 'D':
		keysDown[(int)'d'] = false;
		break;
	}
}
void handleSpecialKeys(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_LEFT:
		printf("Left arrow\n");
		camera = (camera + 1) % 8;
		break;
	case GLUT_KEY_RIGHT:
		camera = (camera - 1);
		camera = camera == -1 ? 7 : camera;
		break;
	case GLUT_KEY_UP:
		break;
	case GLUT_KEY_DOWN:
		break;
	}
}
void mouse(int button, int state, int x, int y)
{
	// reset the previous position, such that we only get movement performed after the button
	// was pressed.
	prev_x = x;
	prev_y = y;

	bool buttonDown = state == GLUT_DOWN;

	switch(button)
	{
	case GLUT_LEFT_BUTTON:
		leftDown = buttonDown;
		break;
	case GLUT_MIDDLE_BUTTON:
		middleDown = buttonDown;
		break;
	case GLUT_RIGHT_BUTTON: 
		rightDown = buttonDown;
	default:
		break;
	}
}
void motion(int x, int y)
{
	int delta_x = x - prev_x;
	int delta_y = y - prev_y;

	if(middleDown)
	{
		camera_r -= float(delta_y) * 0.3f;
		// make sure cameraDistance does not become too small
		camera_r = max(0.1f, camera_r);
	}
	if(leftDown)
	{
		camera_phi	-= float(delta_y) * 0.3f * float(M_PI) / 180.0f;
		camera_phi = min(max(0.01f, camera_phi), float(M_PI) - 0.01f);
		camera_theta -= float(delta_x) * 0.3f * float(M_PI) / 180.0f;
	}

	if(rightDown)
	{
		camera_target_altitude += float(delta_y) * 0.1f; 
	}
	prev_x = x;
	prev_y = y;
}
void tick() {
	if (keysDown[(int)'w']) {
		float3 term = carLoc.frontDir * carLoc.moveSpeed;
		carLoc.location += term;
		hasChanged = true;

	}if (keysDown[(int)'s']) {
		float3 term = carLoc.frontDir * carLoc.moveSpeed;
		carLoc.location -= term;
		hasChanged = true;

	}if (keysDown[(int)'a'] && (keysDown[(int)'w'] || keysDown[(int)'s'])) {
		carLoc.angley += carLoc.rotationSpeed;
		carLoc.frontDir = make_rotation_y<float3x3>(carLoc.rotationSpeed) * carLoc.frontDir;
		camera_theta += carLoc.rotationSpeed;
		hasChanged = true;

	}if (keysDown[(int)'d'] && (keysDown[(int)'w'] || keysDown[(int)'s'])) {
		carLoc.angley -= carLoc.rotationSpeed;
		carLoc.frontDir = make_rotation_y<float3x3>(-carLoc.rotationSpeed) * carLoc.frontDir;
		camera_theta -= carLoc.rotationSpeed;
		hasChanged = true;
	}
}
void idle( int v )
{
	float elapsedTime = glutGet(GLUT_ELAPSED_TIME) - timeSinceDraw;
	float time = (1000 / TICK_PER_SECOND) - elapsedTime;
	printf("%f", elapsedTime);
	if (time < 0) {

		// Reseting dynamic stuff in collider
		collider->resetOctree();
		collider->insertMesh(&spider);

		glutTimerFunc(1000 / TICK_PER_SECOND, idle, 0);
		timeSinceDraw = float(glutGet(GLUT_ELAPSED_TIME));
		static float startTime = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

		currentTime = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f - startTime;

		// rotate light around X axis, sunlike fashion.
		// do one full revolution every 20 seconds.
		float4x4 rotateLight = make_rotation_x<float4x4>((float) (2.0f * M_PI * currentTime / 20.0f));

		// rotate and update global light position.
		float3 pos = make_vector3(rotateLight * make_vector(30.1f, 450.0f, 0.1f, 1.0f));
		scene.directionalLight.direction = -pos;
		sunCamera->setPosition(pos);

		//Calculate camera matrix
		playerCamera->setLookAt(carLoc.location + make_vector(0.0f, camera_target_altitude, 0.0f));
		playerCamera->setPosition(carLoc.location + sphericalToCartesian(camera_theta, camera_phi, camera_r));

		gen->update(elapsedTime);
		gen->m_position = make_vector(3 * sin(currentTime) * sin(currentTime)* sin(currentTime), 3 * sin(currentTime), 5 * sin(currentTime) * cos(currentTime)) + make_vector(0.0f, 15.0f, 0.0f);

		tick();

		if (hasChanged){
			checkIntersection();
			updatePlayer();
			hasChanged = false;
		}

		glutPostRedisplay();
	}
	else {
		glutTimerFunc(int(time), idle, 0);
	}
}

int main(int argc, char *argv[])
{
	Logger::debug = false;
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;

	renderer = new Renderer(argc, argv, w, h);
	glutTimerFunc(50, idle, 0);
	glutDisplayFunc(display);

	glutKeyboardFunc(handleKeys); 
	glutKeyboardUpFunc(handleKeysRelease);
	glutSpecialFunc(handleSpecialKeys); 
	glutMouseFunc(mouse); 
	glutMotionFunc(motion);

	renderer->initGL();

	createCubeMaps();
	createCameras();
	createMeshes();
	createLights();
	createEffects();

	renderer->start();
	
	return 0;
}

void createEffects(){
	Fog f;
	f.fEquation = FOG_EQ::LINEAR;
	renderer->effects.fog = f;

	Blur b;
	b.cutOff = 0.9f;
	b.active = true;
	renderer->effects.blur = b;
}

void createLights() {
	PointLight sun;
	sun.diffuseColor  = make_vector(0.0f, 2.0f, 0.0f);
	sun.specularColor = make_vector(0.1f, 0.1f, 0.1f);
	sun.ambientColor  = make_vector(0.0f, 0.0f, 0.0f);
	sun.position      = make_vector(10.1f, 7.0f, 10.1f);
	sun.attenuation.constant = 1.0f;
	sun.attenuation.linear   = 1.0f;
	sun.attenuation.exp      = 0.0f;
	scene.pointLights.push_back(sun);

	PointLight sun2;
	sun2.diffuseColor = make_vector(100.6f, 0.6f, 0.6f);
	sun2.specularColor = make_vector(1.0f, 0.0f, 0.0f);
	sun2.ambientColor = make_vector(0.05f, 0.05f, 0.05f);
	sun2.position = make_vector(30.0f, 7.0f, 30.0f);
	sun2.attenuation.constant = 1.0f;
	sun2.attenuation.linear = 0.7f;
	sun2.attenuation.exp = 1.8f;
	scene.pointLights.push_back(sun2);

	PointLight houseLight;
	houseLight.diffuseColor = make_vector(10.0f, 10.0f, 0.0f);
	houseLight.specularColor = make_vector(1.0f, 0.0f, 0.0f);
	houseLight.ambientColor = make_vector(0.05f, 0.05f, 0.05f);
	houseLight.position = make_vector(93.0f, 2.0f, -93.0f);
	houseLight.attenuation.constant = 1.0f;
	houseLight.attenuation.linear = 0.7f;
	houseLight.attenuation.exp = 1.8f;
	scene.pointLights.push_back(houseLight);

	DirectionalLight sun3;
	sun3.diffuseColor  = make_vector(0.6f, 0.6f, 0.6f);
	sun3.specularColor = make_vector(0.6f, 0.6f, 0.6f);
	sun3.ambientColor  = make_vector(0.35f, 0.35f, 0.35f);
	sun3.direction     = make_vector(0.0f, -100.0f, 0.0f);
	scene.directionalLight = sun3;

	SpotLight spot;
	spot.diffuseColor  = make_vector(0.6f, 0.6f, 0.6f);
	spot.specularColor = make_vector(0.6f, 0.6f, 0.6f);
	spot.ambientColor  = make_vector(0.05f, 0.05f, 0.05f);
	spot.direction = make_vector(0.0f, -10.0f, 0.0f);
	spot.position = make_vector(0.0f, 10.0f, -10.0f);
	spot.cutOff      = cos(degreeToRad(20.5));
	spot.outerCutOff = cos(degreeToRad(28.5));
	spot.attenuation.constant = 1.0f;
	spot.attenuation.linear = 0.0f;
	spot.attenuation.exp = 0.0f;
	scene.spotLights.push_back(spot);
}

void createCubeMaps() {
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;
	//*************************************************************************
	// Load cube map texture
	//*************************************************************************
	reflectionCubeMap = new CubeMapTexture("../scenes/posx.jpg", "../scenes/negx.jpg", "../scenes/posy.jpg", "../scenes/posy.jpg", "../scenes/negz.jpg", "../scenes/posz.jpg");
	scene.cubeMap = reflectionCubeMap;
	//X
	cubeMapCameras[0] = new PerspectiveCamera(carLoc.location + make_vector(0.0f, 3.0f, 0.0f), make_vector(100.0f, 3.0f, 0.0f), make_vector(0.0f, -1.0f, 0.0f), 90, 1, 0.1f, 1000.0f);
	cubeMapCameras[1] = new PerspectiveCamera(carLoc.location + make_vector(0.0f, 3.0f, 0.0f), make_vector(-100.0f, 3.0f, 0.0f), make_vector(0.0f, -1.0f, 0.0f), 90, 1, 0.1f, 1000.0f);

	//Y
	cubeMapCameras[2] = new PerspectiveCamera(carLoc.location + make_vector(0.0f, 3.0f, 0.0f), make_vector(0.1f, 100.1f, 0.0f), make_vector(0.0f, -1.0f, 0.0f), 90, 1, 0.1f, 1000.0f);
	cubeMapCameras[3] = new PerspectiveCamera(carLoc.location + make_vector(0.0f, 3.0f, 0.0f), make_vector(0.1f, -100.0f, 0.0f), make_vector(0.0f, -1.0f, 0.0f), 90, 1, 0.1f, 1000.0f);

	//Z
	cubeMapCameras[4] = new PerspectiveCamera(carLoc.location + make_vector(0.0f, 3.0f, 0.0f), make_vector(0.1f, 3.0f, 100.0f), make_vector(0.0f, -1.0f, 0.0f), 90, 1, 0.1f, 1000.0f);
	cubeMapCameras[5] = new PerspectiveCamera(carLoc.location + make_vector(0.0f, 3.0f, 0.0f), make_vector(0.1f, 3.0f, -100.0f), make_vector(0.0f, -1.0f, 0.0f), 90, 1, 0.1f, 1000.0f);

	cMapAll.width = w;
	cMapAll.height = h;

	glGenFramebuffers(1, &cMapAll.id);
	glBindFramebuffer(GL_FRAMEBUFFER, cMapAll.id);

	glGenTextures(1, &cMapAll.texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cMapAll.texture);

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, CUBE_MAP_RESOLUTION, CUBE_MAP_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, CUBE_MAP_RESOLUTION, CUBE_MAP_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, CUBE_MAP_RESOLUTION, CUBE_MAP_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, CUBE_MAP_RESOLUTION, CUBE_MAP_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, CUBE_MAP_RESOLUTION, CUBE_MAP_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, CUBE_MAP_RESOLUTION, CUBE_MAP_RESOLUTION, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, cMapAll.texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, cMapAll.texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, cMapAll.texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, cMapAll.texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, cMapAll.texture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, cMapAll.texture, 0);

	const string CUBE_MAP_SHADER = "CUBE_MAP_SHADER";
	ResourceManager::loadShader("../shaders/simple.vert", "../shaders/simple.frag", CUBE_MAP_SHADER);
	cMapAll.shaderProgram = ResourceManager::getShader(CUBE_MAP_SHADER);
}


void createMeshes() {
	Logger::logInfo("Started loading meshes");

	Texture *particleTexture = ResourceManager::loadAndFetchTexture("../scenes/engineflare1.jpg");

	FireParticle *fireConf = new FireParticle();
	gen = new ParticleGenerator(particleTexture, 200, playerCamera, make_vector(0.0f, 15.0f, 0.0f), fireConf);
	scene.transparentObjects.push_back(gen);

	//*************************************************************************
	// Load the models from disk
	//*************************************************************************
	//Load shadow casters
	Mesh* carM = ResourceManager::loadAndFetchMesh("../scenes/untitled.dae");
	car = GameObject(carM);
	car.move(make_translation(make_vector(0.0f, 0.0f, 0.0f)));
	scene.shadowCasters.push_back(&car);

	Mesh* worldM = ResourceManager::loadAndFetchMesh("../scenes/world.obj");
	world = GameObject(worldM);
	world.move(make_translation(make_vector(0.0f, 0.0f, 0.0f)));
	scene.shadowCasters.push_back(&world);

	Mesh* factoryM = ResourceManager::loadAndFetchMesh("../scenes/test.obj");
	factory = GameObject(factoryM);
    factory.move(make_translation(make_vector(-15.0f, 6.0f, 0.0f)) * make_rotation_y<float4x4>(
			(float) (M_PI / 180 * 90)) * make_scale<float4x4>(make_vector(2.0f, 2.0f, 2.0f)));
	scene.shadowCasters.push_back(&factory);
	
	Mesh* spiderM = ResourceManager::loadAndFetchMesh("../scenes/spider.obj");
	spider = GameObject(spiderM);
    spider.move(make_translation(make_vector(40.0f, 2.0f, 0.0f)) * make_rotation_x<float4x4>((float) (M_PI / 180 * 0)) * make_scale<float4x4>(0.1f));
	scene.shadowCasters.push_back(&spider);

	Mesh* waterM = ResourceManager::loadAndFetchMesh("../scenes/water.obj");
	water = GameObject(waterM);
    water.move(make_translation(make_vector(0.0f, -6.0f, 0.0f)));
	scene.shadowCasters.push_back(&water);
	
	Mesh* lampM = ResourceManager::loadAndFetchMesh("../scenes/sphere.obj");

	lamp = GameObject(lampM);
    lamp.move(make_translation(make_vector(10.0f, 10.0f, 10.0f)));
	scene.shadowCasters.push_back(&lamp);

	Mesh* lamp2M = ResourceManager::loadAndFetchMesh("../scenes/sphere.obj");
	lamp2 = GameObject(lamp2M);
    lamp2.move(make_translation(make_vector(30.0f, 10.0f, 30.0f)));
	scene.shadowCasters.push_back(&lamp2);
	
	Mesh* lamp3M = ResourceManager::loadAndFetchMesh("../scenes/sphere.obj");
	lamp3 = GameObject(lamp3M);
    lamp3.move(make_translation(make_vector(0.0f, 10.0f, -10.0f)));
	scene.shadowCasters.push_back(&lamp3);

	Mesh* normalTestM = ResourceManager::loadAndFetchMesh("../scenes/boxwNormals.obj");
	normalTest = GameObject(normalTestM);
    normalTest.move(make_translation(make_vector(0.0f, 10.0f, 0.0f)) * make_rotation_x<float4x4>(
			(float) (M_PI / 180 * 30)));
	scene.shadowCasters.push_back(&normalTest);

	Mesh* normalTestWithoutM = ResourceManager::loadAndFetchMesh("../scenes/boxwoNormals.obj");
	normalTestWithout = GameObject(normalTestWithoutM);
    normalTestWithout.move(make_translation(make_vector(5.0f, 10.0f, 0.0f)) * make_rotation_x<float4x4>(
			(float) (M_PI / 180 * 30)));
	scene.shadowCasters.push_back(&normalTestWithout);
	

	Logger::logInfo("Finished loading meshes.");

	//*************************************************************************
	// Generate Octtree from meshes
	//*************************************************************************
	Logger::logInfo("Started creating octree");
	
	utils::Timer timer;
	timer.start();
	float3 origin = make_vector(0.0f, 0.0f, 0.0f);
	float3 halfVector = make_vector(200.0f, 200.0f, 200.0f); //TODO

	octTree = new Octree(origin, halfVector, 0);

	collider = new Collider(octTree);
	collider->addMesh(&world);
	collider->addMesh(&water);
	collider->addMesh(&factory);
    collider->addMesh(&spider);
  
	collider->insertAll(); //TODO enlargen octrees afterhand instead
	collider->saveOctree();
	Logger::logInfo("Finished loading octree");
	renderer->setOctree(*octTree);
	
	timer.stop();
	
	stringstream timeMessage;
	timeMessage << "Created octree in : " << timer.getElapsedTime() << " ms";
	Logger::logInfo(timeMessage.str());
}

void createCameras() {
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;

	sunCamera = new PerspectiveCamera(startPosSun, make_vector(0.0f, 0.0f, 0.0f), make_vector(0.0f, 1.0f, 0.0f), 45, 1.0f, 280.0f, 600.0f);
	scene.shadowMapCamera = sunCamera;

	playerCamera = new PerspectiveCamera(
		carLoc.location + sphericalToCartesian(camera_theta, camera_phi, camera_r),
		carLoc.location + make_vector(0.0f, camera_target_altitude, 0.0f),
		make_vector(0.0f, 1.0f, 0.0f),
		45, float(w) / float(h), 0.1f, 1000.0f
		);

	skybox = new Skybox(playerCamera);
	skybox->init("../scenes/posx.jpg", "../scenes/negx.jpg", "../scenes/posy.jpg", "../scenes/posy.jpg", "../scenes/negz.jpg", "../scenes/posz.jpg");
	scene.transparentObjects.push_back(skybox);
}

float degreeToRad(float degree) {
	return (float) (degree * M_PI / 180);
}

float radToDegree(float rad) {
	return (float) (rad * 180 / M_PI);
}

// Helper function to turn spherical coordinates into cartesian (x,y,z)
float3 sphericalToCartesian(float theta, float phi, float r)
{
	return make_vector(r * sinf(theta)*sinf(phi),
		r * cosf(phi),
		r * cosf(theta)*sinf(phi));
}

void updatePlayer() {
	float3 frontDir = normalize(carLoc.frontDir);
	float3 rightDir = normalize(cross(frontDir, vUp));

	float anglex = -(degreeToRad(90.0f) - acosf(dot(normalize(carLoc.upDir), frontDir)));
	float anglez = (degreeToRad(90.0f) - acosf(dot(normalize(carLoc.upDir), rightDir)));

	Quaternion qatX = make_quaternion_axis_angle(rightDir, anglex);
	Quaternion qatY = make_quaternion_axis_angle(vUp, carLoc.angley);
	Quaternion qatZ = make_quaternion_axis_angle(make_rotation_y<float3x3>(-carLoc.angley) * frontDir, anglez);

	car.move(make_translation(carLoc.location));
	car.update(makematrix(qatX));
	car.update(makematrix(qatY));
	car.update(makematrix(qatZ));
}

