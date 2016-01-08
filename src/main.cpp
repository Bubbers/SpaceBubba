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
#include <SpaceShipComponent.h>
#include <SpawnAsteroidOnDeath.h>
#include <FireParticle.h>
#include <SoundManager.h>
#include <MouseWarp.h>
#include "FireSphere.h"

#include "DeathOnCollision.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "constants.h"
#include "ShootComponent.h"
#include <SFML/Window.hpp>
#include <ControlsManager.h>
#include <Controls.h>
#include <KeyboardButton.h>
#include <JoystickAxis.h>
#include <JoystickButton.h>
#include <MouseButton.h>
#include <MouseAxis.h>


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

int points = 0;

//*****************************************************************************
//	OBJ Model declarations
//*****************************************************************************


GameObject rWing;
GameObject skyBox;
GameObject *hud;
GameObject dstar;
SpaceShipComponent *spaceMover;
GameObject planet;
GameObject sun;

State state = Start;


Scene scene;

BFBroadPhase broadPhaseCollider;


SoundManager* soundManager;



//*****************************************************************************
//	Cube Maps
//*****************************************************************************
CubeMapTexture* reflectionCubeMap;

//*****************************************************************************
//	Camera state variables (updated in motion())
//*****************************************************************************
float camera_theta = (float) (M_PI / 1.0f);
float camera_phi = (float) (M_PI / 2.6f);
float camera_r = 30.0f; 
float camera_target_altitude = 5.2f;

float camspeed = 0.0f;
bool chase = false;
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
	ControlsManager* cm = ControlsManager::getInstance();
	if(cm->getStatus(QUIT).isActive())
		exit(0);

}

MouseWarp motion(int x, int y, int delta_x, int delta_y)
{
/*
	InputManager* im = InputManager::getInstance();
	bool someDown = false;
	Logger::logDebug("yo");

	if (im->isMouseButtonDown(InputManager::MOUSE_MIDDLE)) {
		camera_r -= float(delta_y) * 0.3f;
		// make sure cameraDistance does not become too small
		camera_r = max(0.1f, camera_r);
		someDown = true;
	}
	if (im->isMouseButtonDown(InputManager::MOUSE_LEFT)) {
		camera_phi -= float(delta_y) * 0.3f * float(M_PI) / 180.0f;
		camera_phi = min(max(0.01f, camera_phi), float(M_PI) - 0.01f);
		camera_theta -= float(delta_x) * 0.3f * float(M_PI) / 180.0f;
		someDown = true;
	}

	if (im->isMouseButtonDown(InputManager::MOUSE_RIGHT)) {
		camera_target_altitude += float(delta_y) * 0.1f;
		someDown = true;
	}
	if(someDown) {
		glutSetCursor(GLUT_CURSOR_NONE);
		return MouseWarp(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	}else {
		glutSetCursor(GLUT_CURSOR_INFO);
		return MouseWarp::noWarp();
	}*/
}


float3 calculateNewCameraPosition() {
	float4 ps = rWing.getModelMatrix().c4;
	float3 location = make_vector(ps.x, ps.y, ps.z);

	playerCamera->setLookAt(location + make_vector(0.0f, camera_target_altitude, 0.0f));
	float3 pc = playerCamera->getPosition();

	float3 diff = (location - pc);
	float dist = 20.f;
	if (length(diff) > dist) {
		chase = true;
	}

	if (chase) {
		if (camspeed < 1.5f) {
			camspeed += .0018f;
		} else {
			chase = false;
		}
	} else {
		if(camspeed > 0.0) {
			camspeed -= 0.0f;
		} else {
			camspeed = 0.0f;
		}
	}

	float3 newPos = location + sphericalToCartesian(camera_theta, camera_phi, camera_r);
	float3 cameraDiff = (camspeed) * (newPos - pc);

	return pc + cameraDiff;
}

void idle( int v )
{
	float elapsedTime = glutGet(GLUT_ELAPSED_TIME) - timeSinceDraw;

	sf::Joystick::update();
	float time = (1000 / TICK_PER_SECOND) - elapsedTime;
	checkKeys();
	if (time < 0) {
		glutTimerFunc(1000 / TICK_PER_SECOND, idle, 0);
		timeSinceDraw = float(glutGet(GLUT_ELAPSED_TIME));
		static float startTime = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;

		currentTime = float(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f - startTime;

		std::vector<GameObject*>* toDelete = new std::vector<GameObject*>();

        scene.update(elapsedTime, toDelete);

		playerCamera->setPosition(calculateNewCameraPosition());

        broadPhaseCollider.updateCollision();

		glutWarpPointer(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

		for(auto it = toDelete->begin(); it < toDelete->end(); it++){
			delete *it;
		}

		delete toDelete;

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

	srand(time(NULL));
	renderer = new Renderer(argc, argv, w, h);
	glutTimerFunc(50, idle, 0);
	glutDisplayFunc(display);

	InputManager* im = InputManager::getInstance();
	//im->addMouseMoveListener(motion);
	ControlsManager* cm = ControlsManager::getInstance();
	try {
		cm->addBindings(ALTITUDE, {new KeyboardButton(sf::Keyboard::L, sf::Keyboard::P),
								   new JoystickAxis(sf::Joystick::Axis::V, true),
								   new MouseAxis(MouseAxis::Axis::Y, 2.0f)});
		cm->addBindings(ACCELERATE, {new KeyboardButton(sf::Keyboard::S, sf::Keyboard::W),
									 new JoystickAxis(sf::Joystick::Axis::Y, true)});
		cm->addBindings(TURN, {new KeyboardButton(sf::Keyboard::D, sf::Keyboard::A),
							   new JoystickAxis(sf::Joystick::Axis::U, true), new MouseAxis(MouseAxis::Axis::X, 3.0f)});
		cm->addBindings(SHOOT, {new KeyboardButton(sf::Keyboard::Space), new JoystickAxis(sf::Joystick::Axis::R, false),
								new MouseButton(sf::Mouse::Button::Left)});
		cm->addBindings(QUIT, {new KeyboardButton(sf::Keyboard::Escape)});
	}catch(string unmatchingDuality){
		Logger::logSevere(unmatchingDuality);
		return 1;
	}

	renderer->initGL();

	soundManager = new SoundManager();

	createCubeMaps();
	createCameras();
	createMeshes();
	createLights();
	createEffects();

	glutWarpPointer(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);
	glutSetCursor(GLUT_CURSOR_NONE);
	renderer->start();
	
	return 0;
}

void createEffects(){
	Fog f;
	f.fEquation = FOG_EQ::LINEAR;
	f.fStart = 5000.0f;
	f.fEnd   = 1000000.0f;
	renderer->effects.fog = f;
}

void createLights() {
	DirectionalLight sun;
	sun.diffuseColor  = make_vector(0.6f, 0.6f, 0.6f);
	sun.specularColor = make_vector(0.6f, 0.6f, 0.6f);
	sun.ambientColor  = make_vector(0.15f, 0.15f, 0.15f);
	sun.direction     = -make_vector(15000.0f, 0.0f, 0.0f);
	scene.directionalLight = sun;


}

void createCubeMaps() {
	reflectionCubeMap = new CubeMapTexture("../scenes/space.png", "../scenes/space.png", "../scenes/space.png", "../scenes/space.png", "../scenes/space.png", "../scenes/space.png");
	scene.cubeMap = reflectionCubeMap;
}


void createMeshes() {
	Logger::logInfo("Started loading meshes");


    
	//SKYBOX
	Mesh *skyBoxM = ResourceManager::loadAndFetchMesh("../scenes/sphere.obj");
	skyBox = GameObject(skyBoxM,Environment);
    SkyBoxRenderer *skyboxRenderer = new SkyBoxRenderer(playerCamera, skyBoxM, &skyBox);
    skyboxRenderer->init("../scenes/x.png", "../scenes/fancyx.png", "../scenes/fancyy.png", "../scenes/y.png", "../scenes/fancyz.png", "../scenes/z.png");
    skyBox.addRenderComponent(skyboxRenderer);
    scene.shadowCasters.push_back(&skyBox);

    //OBJECTS
	Shader* standardShader = ResourceManager::getShader(SIMPLE_SHADER_NAME);
	standardShader->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);

	Mesh* rWingM = ResourceManager::loadAndFetchMesh("../scenes/R_wing.obj");
	rWing = GameObject(rWingM, Player);
	rWing.move(make_translation(make_vector(0.0f, 0.0f, 0.0f)));
	StandardRenderer *carRenderer = new StandardRenderer(rWingM, &rWing, standardShader);
	rWing.addRenderComponent(carRenderer);

	DeathOnCollision* wingD = new DeathOnCollision(&rWing, Asteroid, 0, &points);
	rWing.addComponent(wingD);

	Texture *particleTexture = ResourceManager::loadAndFetchTexture("../scenes/fire_part.png");

	FireParticle *fireConf = new FireParticle();
	ParticleGenerator *gen = new ParticleGenerator(particleTexture, 500, playerCamera, make_vector(0.0f, 0.0f, 0.0f), fireConf);
	GameObject *particleGenerator = new GameObject();
	particleGenerator->addRenderComponent(gen);
	scene.transparentObjects.push_back(particleGenerator);

	FireParticle *fireConf2 = new FireParticle();
	ParticleGenerator *gen2 = new ParticleGenerator(particleTexture, 500, playerCamera, make_vector(0.0f, 0.0f, 0.0f), fireConf2);
	GameObject *particleGenerator2 = new GameObject();
	particleGenerator2->addRenderComponent(gen2);
	scene.transparentObjects.push_back(particleGenerator2);


	//HUD
	hud = new GameObject();
	HudRenderer *hudRenderer = new HudRenderer(&points, &state);
	hud->addRenderComponent(hudRenderer);
	scene.transparentObjects.push_back(hud);


	spaceMover = new SpaceShipComponent(hudRenderer->getConfig(),&camera_theta, &camera_phi, &rWing, gen, gen2, &state);
	ShootComponent *shooter = new ShootComponent(&rWing, spaceMover, &scene, &broadPhaseCollider, 1000);
	rWing.addComponent(shooter);
	rWing.addComponent(spaceMover);
	rWing.setDynamic(true);
	scene.shadowCasters.push_back(&rWing);
	broadPhaseCollider.addGameObject(&rWing);

	Mesh* dstarM = ResourceManager::loadAndFetchMesh("../scenes/dstar.obj");
	dstar = GameObject(dstarM, SpaceEntity);
	StandardRenderer *dstarRenderer = new StandardRenderer(dstarM, &dstar, standardShader);
	dstar.addRenderComponent(dstarRenderer);

	MoveComponent *dstarMover = new MoveComponent(&dstar);
	dstarMover->setRotationAxis(make_vector(0.0f, 1.0f, 0.0f));
	dstarMover->setRotationSpeed(0.0001f);
	dstarMover->setLocation(make_vector(-10.0f, 0.0f, 16000.0f));
	dstarMover->setScale(make_vector(2000.0f, 2000.0f, 2000.0f));
	dstar.setDynamic(true);

	dstar.addComponent(dstarMover);
	scene.shadowCasters.push_back(&dstar);
	broadPhaseCollider.addGameObject(&dstar);

	Mesh* planetM = ResourceManager::loadAndFetchMesh("../scenes/planet.obj");
	planet = GameObject(planetM, SpaceEntity);
	StandardRenderer *planetRenderer = new StandardRenderer(planetM, &planet, standardShader);
	planet.addRenderComponent(planetRenderer);
	MoveComponent *planetMover = new MoveComponent(&planet);
	planetMover->setLocation(make_vector(-25000.0f, 0.0f, 0.0f));
	planetMover->setScale(make_vector(2000.0f, 2000.0f, 2000.0f));
	planetMover->setRotationSpeed(0.00005f);
	planetMover->setRotationAxis(make_vector(0.0f,1.0f,0.0f));
	planet.addComponent(planetMover);

	scene.shadowCasters.push_back(&planet);
	broadPhaseCollider.addGameObject(&planet);

	Mesh* sunM = ResourceManager::loadAndFetchMesh("../scenes/sun.obj");
	sun = GameObject(sunM, SpaceEntity);
	MoveComponent *sunMover = new MoveComponent(&sun);
	sunMover->setLocation(make_vector(20000.0f, 0.0f, 0.0f));
	sunMover->setScale(make_vector(2000.0f, 2000.0f, 2000.0f));
	sunMover->setRotationSpeed(0.0001f);
	sunMover->setRotationAxis(make_vector(0.0f, 1.0f, 0.0f));
	sun.addComponent(sunMover);
	StandardRenderer *sunRenderer = new StandardRenderer(sunM, &sun, standardShader);
	sun.addRenderComponent(sunRenderer);

	scene.shadowCasters.push_back(&sun);
	broadPhaseCollider.addGameObject(&sun);



    for( int i = 0; i < 50; i++) {
        Mesh* asteroidM = ResourceManager::loadAndFetchMesh("../scenes/asteroid.obj");
        GameObject *asteroid = new GameObject(asteroidM, Asteroid);
        StandardRenderer *asteroidRenderer = new StandardRenderer(asteroidM, asteroid, standardShader);
        asteroid->addRenderComponent(asteroidRenderer);
        asteroid->setDynamic(true);

        float3 location = createRandomVector(-200.0f, 200.0f) + make_vector(150.0f,100.0f, 600.0f);
        float3 velocity = createRandomVector(-0.015f, 0.015f);
        float3 rotation = createRandomVector(-0.0025f, 0.0025f);

        location += spaceMover->getLocation();

        MoveComponent *asteroidMover = new MoveComponent(asteroid);
        asteroidMover->setVelocity(velocity);
        asteroidMover->setRotationSpeed(length(rotation));
		asteroidMover->setRotationAxis(rotation);
        asteroidMover->setLocation(location);
        //asteroidMover->setAcceleration(make_vector(-0.0000005f, 0.0f, 0.0f));
        asteroidMover->setScaleSpeed(make_vector(0.0005f,0.0005f,0.0005f));
        asteroid->addComponent(asteroidMover);

        DeathOnCollision* dca = new DeathOnCollision(asteroid, Laser, 1, &points);
        asteroid->addComponent(dca);

        SpawnAsteroidOnDeath *childrenSpawner = new SpawnAsteroidOnDeath(asteroid,&scene,&broadPhaseCollider,make_vector(1.0f,1.0f,1.0f), playerCamera, &points);
        asteroid->addComponent(childrenSpawner);

        scene.shadowCasters.push_back(asteroid);
        broadPhaseCollider.addGameObject(asteroid);
    }

	Logger::logInfo("Finished loading meshes.");
}

void createCameras() {
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;

	sunCamera = new PerspectiveCamera( make_vector(20000.0f, 0.0f, 0.0f), make_vector(0.0f, 0.0f, 0.0f), make_vector(0.0f, 1.0f, 0.0f), 45, 1.0f, 10.0f, 30000.0f);
	scene.shadowMapCamera = sunCamera;

	playerCamera = new PerspectiveCamera(
		sphericalToCartesian(camera_theta, camera_phi, camera_r),
		make_vector(0.0f, camera_target_altitude, 0.0f),
		make_vector(0.0f, 1.0f, 0.0f),
		45, float(w) / float(h), 0.1f, 50000.0f
		);
}

// Helper function to turn spherical coordinates into cartesian (x,y,z)
float3 sphericalToCartesian(float theta, float phi, float r)
{
	return make_vector(r * sinf(theta)*sinf(phi),
		r * cosf(phi),
		r * cosf(theta)*sinf(phi));
}
