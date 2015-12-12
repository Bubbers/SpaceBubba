#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>
#include <sstream>

#include <Quaternion.h>
#include <Texture.h>
#include <StandardRenderer.h>
#include <CarMoveComponent.h>
#include <InputManager.h>

#include "float3.h"
#include "Renderer.h"
#include "timer.h"
#include "ParticleGenerator.h"
#include "ResourceManager.h"
#include "FireParticle.h"
#include "constants.h"
#include "BFBroadPhase.h"
#include "HudRenderer.h"
#include "SoundManager.h"

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

float3 startPosSun = make_vector(30.1f, 450.0f, 0.1f);




//*****************************************************************************
//	OBJ Model declarations
//*****************************************************************************
GameObject world;
GameObject car;
GameObject factory;
GameObject water;
GameObject spider;
GameObject lamp;
GameObject lamp2;
GameObject lamp3;
GameObject normalTest;
GameObject normalTestWithout;
GameObject particleGenerator;
ParticleGenerator *gen;
GameObject skyBox;

Scene scene;

BFBroadPhase broadPhaseCollider;

//*****************************************************************************
//	Cube Maps
//*****************************************************************************
CubeMapTexture* reflectionCubeMap;

Fbo cMapAll;


//*****************************************************************************
//	Camera state variables (updated in motion())
//*****************************************************************************
float camera_theta = (float) (M_PI / 1.0f);
float camera_phi = (float) (M_PI / 4.0f);
float camera_r = 30.0f; 
float camera_target_altitude = 5.2f;

//*****************************************************************************
//	Camera
//*****************************************************************************
Camera *cubeMapCameras[6];
Camera *sunCamera;
Camera *playerCamera;
int camera = 6;


Renderer *renderer;


//*****************************************************************************
//	Function declarations
//*****************************************************************************
void drawCubeMap(Fbo fbo, Scene scene);
void createCubeMaps();
void createMeshes();
void createCameras();
void createLights();
void createEffects();

float3 sphericalToCartesian(float theta, float phi, float r);


//*****************************************************************************
//	Function implementation
//****************************************************************************


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


void checkKeys()
{
	InputManager* im = InputManager::getInstance();
	if(im->isKeyDown(27,true))
		exit(0);
}

void specialKey(int key, int x, int y)
{

	switch(key)
	{
	case InputManager::LEFT_ARROW:
		printf("Left arrow\n");
		camera = (camera + 1) % 8;
		break;
	case InputManager::RIGHT_ARROW:
		camera = (camera - 1);
		camera = camera == -1 ? 7 : camera;
		break;
	case InputManager::UP_ARROW:
		break;
	case InputManager::DOWN_ARROW:
		break;
	}
}
void motion(int x, int y, int delta_x, int delta_y)
{

	InputManager* im = InputManager::getInstance();

	if(im->isMouseButtonDown(InputManager::MOUSE_MIDDLE))
	{
		camera_r -= float(delta_y) * 0.3f;
		// make sure cameraDistance does not become too small
		camera_r = max(0.1f, camera_r);
	}
	if(im->isMouseButtonDown(InputManager::MOUSE_LEFT))
	{
		camera_phi	-= float(delta_y) * 0.3f * float(M_PI) / 180.0f;
		camera_phi = min(max(0.01f, camera_phi), float(M_PI) - 0.01f);
		camera_theta -= float(delta_x) * 0.3f * float(M_PI) / 180.0f;
	}

	if(im->isMouseButtonDown(InputManager::MOUSE_RIGHT))
	{
		camera_target_altitude += float(delta_y) * 0.1f; 
	}
}

void idle( int v )
{
	float elapsedTime = glutGet(GLUT_ELAPSED_TIME) - timeSinceDraw;
	float4 ps = car.getModelMatrix()->c4;
	float3 location = make_vector(ps.x, ps.y, ps.z);

	float time = (1000 / TICK_PER_SECOND) - elapsedTime;
	playerCamera->setLookAt(location + make_vector(0.0f, camera_target_altitude, 0.0f));
	checkKeys();
	if (time < 0) {
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
		playerCamera->setLookAt(location + make_vector(0.0f, camera_target_altitude, 0.0f));
		playerCamera->setPosition(location + sphericalToCartesian(camera_theta, camera_phi, camera_r));

		gen->update(elapsedTime);
		gen->m_position = make_vector(3 * sin(currentTime) * sin(currentTime)* sin(currentTime), 3 * sin(currentTime), 5 * sin(currentTime) * cos(currentTime)) + make_vector(0.0f, 15.0f, 0.0f);

		lamp.move(make_translation(make_vector(0.0f, sin(currentTime) * 10, 0.0f)));

        scene.update(elapsedTime);

        broadPhaseCollider.updateCollision();

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

	InputManager* im = InputManager::getInstance();
	im->addMouseMoveListener(motion);
	im->addSpecialKeyListener(specialKey);

    SoundManager sm;

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
	b.cutOff = 1.0f;
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
	cubeMapCameras[0] = new PerspectiveCamera(make_vector(0.0f, 3.0f, 0.0f), make_vector(100.0f, 3.0f, 0.0f), make_vector(0.0f, -1.0f, 0.0f), 90, 1, 0.1f, 1000.0f);
	cubeMapCameras[1] = new PerspectiveCamera(make_vector(0.0f, 3.0f, 0.0f), make_vector(-100.0f, 3.0f, 0.0f), make_vector(0.0f, -1.0f, 0.0f), 90, 1, 0.1f, 1000.0f);

	//Y
	cubeMapCameras[2] = new PerspectiveCamera(make_vector(0.0f, 3.0f, 0.0f), make_vector(0.1f, 100.1f, 0.0f), make_vector(0.0f, -1.0f, 0.0f), 90, 1, 0.1f, 1000.0f);
	cubeMapCameras[3] = new PerspectiveCamera(make_vector(0.0f, 3.0f, 0.0f), make_vector(0.1f, -100.0f, 0.0f), make_vector(0.0f, -1.0f, 0.0f), 90, 1, 0.1f, 1000.0f);

	//Z
	cubeMapCameras[4] = new PerspectiveCamera(make_vector(0.0f, 3.0f, 0.0f), make_vector(0.1f, 3.0f, 100.0f), make_vector(0.0f, -1.0f, 0.0f), 90, 1, 0.1f, 1000.0f);
	cubeMapCameras[5] = new PerspectiveCamera(make_vector(0.0f, 3.0f, 0.0f), make_vector(0.1f, 3.0f, -100.0f), make_vector(0.0f, -1.0f, 0.0f), 90, 1, 0.1f, 1000.0f);

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

GameObject *test;
void createMeshes() {

	test = new GameObject();
	HudRenderer *hudRenderer = new HudRenderer();
	test->addRenderComponent(hudRenderer);
	scene.transparentObjects.push_back(test);

	Logger::logInfo("Started loading meshes");

	Texture *particleTexture = ResourceManager::loadAndFetchTexture("../scenes/engineflare1.jpg");

	FireParticle *fireConf = new FireParticle();
	gen = new ParticleGenerator(particleTexture, 200, playerCamera, make_vector(0.0f, 15.0f, 0.0f), fireConf);
    particleGenerator = GameObject();
    particleGenerator.addRenderComponent(gen);
	particleGenerator.setDynamic(true);
	scene.transparentObjects.push_back(&particleGenerator);

	Mesh *skyBoxM = ResourceManager::loadAndFetchMesh("../scenes/sphere.obj");
	skyBox = GameObject(skyBoxM);

    SkyBoxRenderer *skyboxRenderer = new SkyBoxRenderer(playerCamera, skyBoxM, skyBox.getModelMatrix());
    skyboxRenderer->init("../scenes/posx.jpg", "../scenes/negx.jpg", "../scenes/sky_box.jpg", "../scenes/posy.jpg", "../scenes/negz.jpg", "../scenes/posz.jpg");
    skyBox.addRenderComponent(skyboxRenderer);
    scene.shadowCasters.push_back(&skyBox);

	//*************************************************************************
	// Load the models from disk
	//*************************************************************************
	Shader* standardShader = ResourceManager::getShader(SIMPLE_SHADER_NAME);
	standardShader->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);

	//Load shadow casters
	Mesh* carM = ResourceManager::loadAndFetchMesh("../scenes/R_wing.obj"); //untitled.dae");
	car = GameObject(carM);
	car.move(make_translation(make_vector(0.0f, 0.0f, 0.0f)));

	StandardRenderer *carRenderer = new StandardRenderer(carM, car.getModelMatrix(), standardShader);
	car.addRenderComponent(carRenderer);

	CarMoveComponent *carMoveComponent = new CarMoveComponent(&camera_theta, &car);
	car.addComponent(carMoveComponent);
	car.setDynamic(true);
	scene.shadowCasters.push_back(&car);
	broadPhaseCollider.addGameObject(&car);

	Mesh* worldM = ResourceManager::loadAndFetchMesh("../scenes/world.obj");
	world = GameObject(worldM);
	world.move(make_translation(make_vector(0.0f, 0.0f, 0.0f)));
	StandardRenderer *worldRenderer = new StandardRenderer(worldM, world.getModelMatrix(), standardShader);
	world.addRenderComponent(worldRenderer);
	scene.shadowCasters.push_back(&world);
	broadPhaseCollider.addGameObject(&world);
	renderer->setOctree(*world.getOctree());

	Mesh* factoryM = ResourceManager::loadAndFetchMesh("../scenes/test.obj");
	factory = GameObject(factoryM);
    factory.move(make_translation(make_vector(-15.0f, 6.0f, 0.0f)) * make_rotation_y<float4x4>(
                 (float) (M_PI / 180 * 90)) * make_scale<float4x4>(make_vector(2.0f, 2.0f, 2.0f)));
    StandardRenderer *factoryRender = new StandardRenderer(factoryM, factory.getModelMatrix(), standardShader);
    factory.addRenderComponent(factoryRender);
	scene.shadowCasters.push_back(&factory);
	broadPhaseCollider.addGameObject(&factory);

	Mesh* waterM = ResourceManager::loadAndFetchMesh("../scenes/water.obj");
	water = GameObject(waterM);
    water.move(make_translation(make_vector(0.0f, -6.0f, 0.0f)));
    StandardRenderer *waterRenderer = new StandardRenderer(waterM, water.getModelMatrix(), standardShader);
    water.addRenderComponent(waterRenderer);
	scene.shadowCasters.push_back(&water);
	broadPhaseCollider.addGameObject(&water);
	
	Mesh* lampM = ResourceManager::loadAndFetchMesh("../scenes/boxwoNormals.obj");
	lamp = GameObject(lampM);
    lamp.move(make_translation(make_vector(0.0f, 10.0f, 0.0f)));
    StandardRenderer *lamp1Renderer = new StandardRenderer(lampM, lamp.getModelMatrix(), standardShader);
    lamp.addRenderComponent(lamp1Renderer);
	scene.shadowCasters.push_back(&lamp);
	broadPhaseCollider.addGameObject(&lamp);

	Mesh* lamp2M = ResourceManager::loadAndFetchMesh("../scenes/sphere.obj");
	lamp2 = GameObject(lamp2M);
    lamp2.move(make_translation(make_vector(30.0f, 10.0f, 30.0f)));
    StandardRenderer *lamp2Renderer = new StandardRenderer(lamp2M, lamp2.getModelMatrix(), standardShader);
    lamp2.addRenderComponent(lamp2Renderer);
    scene.shadowCasters.push_back(&lamp2);
	broadPhaseCollider.addGameObject(&lamp2);

	Mesh* lamp3M = ResourceManager::loadAndFetchMesh("../scenes/sphere.obj");
	lamp3 = GameObject(lamp3M);
    lamp3.move(make_translation(make_vector(0.0f, 10.0f, -10.0f)));
    StandardRenderer *lamp3Renderer = new StandardRenderer(lamp3M, lamp3.getModelMatrix(), standardShader);
    lamp3.addRenderComponent(lamp3Renderer);
    scene.shadowCasters.push_back(&lamp3);
	broadPhaseCollider.addGameObject(&lamp3);

	/*Mesh* normalTestM = ResourceManager::loadAndFetchMesh("../scenes/boxwNormals.obj");
	normalTest = GameObject(normalTestM);
    normalTest.move(make_translation(make_vector(0.0f, 10.0f, 0.0f)) * make_rotation_x<float4x4>(
			(float) (M_PI / 180 * 30)));
    StandardRenderer *normalTestRenderer = new StandardRenderer(normalTestM, normalTest.getModelMatrix(), standardShader);
    normalTest.addRenderComponent(normalTestRenderer);
	scene.shadowCasters.push_back(&normalTest);
	broadPhaseCollider.addGameObject(&normalTest);*/

	Mesh* normalTestWithoutM = ResourceManager::loadAndFetchMesh("../scenes/boxwoNormals.obj");
	normalTestWithout = GameObject(normalTestWithoutM);
    normalTestWithout.move(make_translation(make_vector(5.0f, 10.0f, 0.0f)) * make_rotation_x<float4x4>(
			(float) (M_PI / 180 * 30)));
    StandardRenderer *normalTestWithoutRenderer = new StandardRenderer(normalTestWithoutM, normalTestWithout.getModelMatrix(), standardShader);
    normalTestWithout.addRenderComponent(normalTestWithoutRenderer);
	scene.shadowCasters.push_back(&normalTestWithout);
	broadPhaseCollider.addGameObject(&normalTestWithout);

	Logger::logInfo("Finished loading meshes.");

}

void createCameras() {
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;

	sunCamera = new PerspectiveCamera(startPosSun, make_vector(0.0f, 0.0f, 0.0f), make_vector(0.0f, 1.0f, 0.0f), 45, 1.0f, 280.0f, 600.0f);
	scene.shadowMapCamera = sunCamera;

	playerCamera = new PerspectiveCamera(
		sphericalToCartesian(camera_theta, camera_phi, camera_r),
		make_vector(0.0f, camera_target_altitude, 0.0f),
		make_vector(0.0f, 1.0f, 0.0f),
		45, float(w) / float(h), 0.1f, 1000.0f
		);


}

// Helper function to turn spherical coordinates into cartesian (x,y,z)
float3 sphericalToCartesian(float theta, float phi, float r)
{
	return make_vector(r * sinf(theta)*sinf(phi),
		r * cosf(phi),
		r * cosf(theta)*sinf(phi));
}
