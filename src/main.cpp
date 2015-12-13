#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <vector>
#include <sstream>

#include <Quaternion.h>
#include <StandardRenderer.h>
#include "MoveComponent.h"
#include <InputManager.h>

#include "Renderer.h"
#include "ResourceManager.h"
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
float currentTime = 0.0f;			// Tells us the current time
float timeSinceDraw = 0.0f;

//*****************************************************************************
//	OBJ Model declarations
//*****************************************************************************
GameObject rWing;
GameObject skyBox;
GameObject *hud;
GameObject asteroid;

Scene scene;

BFBroadPhase broadPhaseCollider;

//*****************************************************************************
//	Sound
//*****************************************************************************
SoundManager sm;

//*****************************************************************************
//	Cube Maps
//*****************************************************************************
CubeMapTexture* reflectionCubeMap;

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

Camera *sunCamera;
Camera *playerCamera;
int camera = 6;

Renderer *renderer;

//*****************************************************************************
//	Function declarations
//*****************************************************************************
void createCubeMaps();
void createMeshes();
void createCameras();
void createLights();
void createEffects();

float3 sphericalToCartesian(float theta, float phi, float r);


void display(void)
{
	renderer->drawScene(*playerCamera, scene, currentTime);
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

float camspeed = 0.0f;
bool chase = false;
void idle( int v )
{
	float elapsedTime = glutGet(GLUT_ELAPSED_TIME) - timeSinceDraw;

	float time = (1000 / TICK_PER_SECOND) - elapsedTime;
	checkKeys();
	if (time < 0) {
		glutTimerFunc(1000 / TICK_PER_SECOND, idle, 0);
		timeSinceDraw = float(glutGet(GLUT_ELAPSED_TIME));
		static float startTime = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

		currentTime = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f - startTime;

        scene.update(elapsedTime);

		float4 ps = rWing.getModelMatrix()->c4;
		float3 location = make_vector(ps.x, ps.y, ps.z);

		playerCamera->setLookAt(location + make_vector(0.0f, camera_target_altitude, 0.0f));
		float3 pc = playerCamera->getPosition();


		Logger::logDebug("### location ###");
		Logger::logDebug(to_string(location.x));
		Logger::logDebug(to_string(location.y));
		Logger::logDebug(to_string(location.z));
		float3 diff = (location - pc);
		float dist = 100.f;
		if (length(diff) > dist) {
			chase = true;
		}

		if (chase) {
			if (camspeed < 1.0f) {
				camspeed += .01f;
			} else {
				chase = false;
			}
		} else {
			if(camspeed > 0.7) {
				camspeed -= 0.01f;
			} else {
				camspeed = 0.7f;
			}
		}

		Logger::logDebug("### DIFF ###");
		Logger::logDebug(to_string(diff.x));
		Logger::logDebug(to_string(diff.y));
		Logger::logDebug(to_string(diff.z));

		Logger::logDebug("### PC ###");
		Logger::logDebug(to_string(pc.x));
		Logger::logDebug(to_string(pc.y));
		Logger::logDebug(to_string(pc.z));

		float3 newPos = location + sphericalToCartesian(camera_theta, camera_phi, camera_r);

		float3 cameraDiff = (camspeed) * (newPos - pc);

		playerCamera->setPosition(pc + cameraDiff);


		Logger::logDebug("### CAM ###");
		Logger::logDebug(to_string(playerCamera->getPosition().x));
		Logger::logDebug(to_string(playerCamera->getPosition().y));
		Logger::logDebug(to_string(playerCamera->getPosition().z));
        broadPhaseCollider.updateCollision();

		glutPostRedisplay();
	}
	else {
		glutTimerFunc(int(time), idle, 0);
	}
}

int main(int argc, char *argv[])
{
	Logger::debug = true;
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;

	renderer = new Renderer(argc, argv, w, h);
	glutTimerFunc(50, idle, 0);
	glutDisplayFunc(display);

	InputManager* im = InputManager::getInstance();
	im->addMouseMoveListener(motion);
	im->addSpecialKeyListener(specialKey);


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
}

void createLights() {
	DirectionalLight sun;
	sun.diffuseColor  = make_vector(0.6f, 0.6f, 0.6f);
	sun.specularColor = make_vector(0.6f, 0.6f, 0.6f);
	sun.ambientColor  = make_vector(0.15f, 0.15f, 0.15f);
	sun.direction     = make_vector(0.0f, -100.0f, 0.0f);
	scene.directionalLight = sun;
}

void createCubeMaps() {
	reflectionCubeMap = new CubeMapTexture("../scenes/posx.jpg", "../scenes/negx.jpg", "../scenes/posy.jpg", "../scenes/posy.jpg", "../scenes/negz.jpg", "../scenes/posz.jpg");
	scene.cubeMap = reflectionCubeMap;
}


void createMeshes() {
	Logger::logInfo("Started loading meshes");

    //HUD
	hud = new GameObject();
	HudRenderer *hudRenderer = new HudRenderer();
	hud->addRenderComponent(hudRenderer);
	scene.transparentObjects.push_back(hud);

    //SKYBOX
	Mesh *skyBoxM = ResourceManager::loadAndFetchMesh("../scenes/sphere.obj");
	skyBox = GameObject(skyBoxM);
    SkyBoxRenderer *skyboxRenderer = new SkyBoxRenderer(playerCamera, skyBoxM, skyBox.getModelMatrix());
    skyboxRenderer->init("../scenes/posx.jpg", "../scenes/negx.jpg", "../scenes/sky_box.jpg", "../scenes/posy.jpg", "../scenes/negz.jpg", "../scenes/posz.jpg");
    skyBox.addRenderComponent(skyboxRenderer);
    scene.shadowCasters.push_back(&skyBox);

    //OBJECTS
	Shader* standardShader = ResourceManager::getShader(SIMPLE_SHADER_NAME);
	standardShader->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);

	Mesh* rWingM = ResourceManager::loadAndFetchMesh("../scenes/R_wing.obj"); //untitled.dae");
	rWing = GameObject(rWingM);
	rWing.move(make_translation(make_vector(0.0f, 0.0f, 0.0f)));
	StandardRenderer *carRenderer = new StandardRenderer(rWingM, rWing.getModelMatrix(), standardShader);
	rWing.addRenderComponent(carRenderer);

	MoveComponent *carMoveComponent = new MoveComponent(hudRenderer->getConfig(),&camera_theta, &rWing);
	rWing.addComponent(carMoveComponent);
	rWing.setDynamic(true);
	scene.shadowCasters.push_back(&rWing);
	broadPhaseCollider.addGameObject(&rWing);

	Mesh* asteroidM = ResourceManager::loadAndFetchMesh("../scenes/asteroid.obj");
	asteroid = GameObject(asteroidM);
	asteroid.move(make_translation(make_vector(10.0f, 10.0f, 10.0f)));
	StandardRenderer *asteroidRenderer = new StandardRenderer(asteroidM, asteroid.getModelMatrix(), standardShader);
	asteroid.addRenderComponent(asteroidRenderer);
	asteroid.setDynamic(true);
	scene.shadowCasters.push_back(&asteroid);
	broadPhaseCollider.addGameObject(&asteroid);

	Logger::logInfo("Finished loading meshes.");
}

void createCameras() {
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;

	sunCamera = new PerspectiveCamera( make_vector(30.1f, 450.0f, 0.1f), make_vector(0.0f, 0.0f, 0.0f), make_vector(0.0f, 1.0f, 0.0f), 45, 1.0f, 280.0f, 600.0f);
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
