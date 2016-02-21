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
#include <SpaceShipComponent.h>
#include <SpawnAsteroidOnDeath.h>
#include <FireParticle.h>
#include "AudioManager.h"
#include "DeathOnCollision.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "constants.h"
#include "ShootComponent.h"
#include <ControlsManager.h>
#include <Controls.h>
#include <KeyboardButton.h>
#include <JoystickAxis.h>
#include <MouseButton.h>
#include <MouseAxis.h>
#include <SkyBoxRenderer.h>
#include <BFBroadPhase.h>
#include <Logger.h>
#include <PerspectiveCamera.h>
#include <Scene.h>
#include <Globals.h>
#include <JoystickButton.h>
#include <JoystickTranslator.h>
#include "CubeMapTexture.h"
#include <FileLogHandler.h>
#include <StdOutLogHandler.h>
#include <string>
#include <SpaceBubbaObject.h>
#include <Layout.h>
#include <ListLayout.h>
#include <Dimension.h>
#include <PositioningLayout.h>
#include <HUDGraphic.h>
#include <SpaceBubbaHudRenderer.h>
#include "CubeMapTexture.h"
#include "StdOutLogHandler.h"

#ifdef WIN32
#define SFML_STATIC
#endif

using namespace std;
using namespace chag;

#define SCREEN_WIDTH   1028
#define SCREEN_HEIGHT  800

#define TICK_PER_SECOND  50


static const float3 UP_VECTOR = make_vector(0.0f, 1.0f, 0.0f);
//*****************************************************************************
// Global variables
//*****************************************************************************

int points = 0;

//*****************************************************************************
// OBJ Model declarations
//*****************************************************************************

float* rWingSpeed;
SpaceBubbaObject *rWing;
SpaceBubbaObject skyBox;
SpaceBubbaObject *hud;
SpaceBubbaObject dstar;
SpaceShipComponent *spaceMover;
SpaceBubbaObject planet;
SpaceBubbaObject sun;

State state = Start;

Scene scene;

BFBroadPhase broadPhaseCollider;


//*****************************************************************************
// Cube Maps
//*****************************************************************************
CubeMapTexture* reflectionCubeMap;

//*****************************************************************************
// Camera state variables (updated in motion())
//*****************************************************************************
float camera_theta = static_cast<float> (M_PI / 1.0f);
float camera_phi   = static_cast<float> (M_PI / 2.6f);
float camera_r = 30.0f;
float camera_target_altitude = 5.2f;

bool chase = false;
//*****************************************************************************
// Camera
//*****************************************************************************

Camera *sunCamera;
Camera *playerCamera;
int camera = 6;

Renderer *renderer;

//*****************************************************************************
// Function declarations
//*****************************************************************************
void createCubeMaps();
void createMeshes();
void createCameras();
void createLights();
void createEffects();
void startAudio();
void mapKeyBindings();

float3 sphericalToCartesian(float theta, float phi, float r);


void display(float timeSinceStart,float timeSinceLastCall) {
    renderer->drawScene(playerCamera, &scene, timeSinceStart);
}

void checkKeys() {
    ControlsManager* cm = ControlsManager::getInstance();
    if (cm->getStatus(QUIT).isActive())
        exit(0);
}


float3 calculateNewCameraPosition() {
    if (state == Died)
        return playerCamera->getPosition();

    float3 location = rWing->getLocation();

    playerCamera->setLookAt(location + spaceMover->getUpDir()
                                       * camera_target_altitude);

    float x2 = length(spaceMover->getVelocity())-0.5f;
    float distanceDiff = -2*x2*x2+1.5f;
    return location - spaceMover->getFrontDir() * 30.0f * distanceDiff +
                      spaceMover->getUpDir()    * 10.0f * distanceDiff;
}

void idle(float timeSinceStart,float timeSinceLastCall) {

    sf::Joystick::update();
    checkKeys();

    // TODO(Any) Cleanup shouldnt be here. Let scene delete?
    std::vector<GameObject*>* toDelete = new std::vector<GameObject*>();

    scene.update(timeSinceLastCall*1000.0f, toDelete);

    playerCamera->setPosition(calculateNewCameraPosition());

    broadPhaseCollider.updateCollision();

    sf::Mouse::setPosition(sf::Vector2<int>(
                                   Globals::get(Globals::Key::WINDOW_WIDTH)/2,
                                   Globals::get(Globals::Key::WINDOW_HEIGHT)/2),
                                   *renderer->getWindow());

    playerCamera->setUpVector(normalize(spaceMover->getUpDir()));

    for (auto it = toDelete->begin(); it < toDelete->end(); it++) {
        delete (*it);
    }

    delete toDelete;
}

int main(int argc, char *argv[]) {

    rWingSpeed = (float*)calloc(1,sizeof(float));
    *rWingSpeed = 0.0f;

	Logger::addLogHandler(new FileLogHandler("logggg.log"));
	Logger::addLogHandler(new StdOutLogHandler());
	Logger::setLogLevel(LogLevel::DEBUG);
	int w = SCREEN_WIDTH;
	int h = SCREEN_HEIGHT;

	srand(time(NULL));
	renderer = new Renderer(w, h);
	renderer->setIdleMethod(idle);
	renderer->setDisplayMethod(display);

	try {
		mapKeyBindings();
	} catch (string unmatchingDuality) {
		Logger::logError(unmatchingDuality);
		return 1;
	}

	createCubeMaps();
	createCameras();
	createMeshes();
	createLights();
	createEffects();
	startAudio();

	renderer->getWindow()->setMouseCursorVisible(false);
	sf::Mouse::setPosition(sf::Vector2<int>(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2),
						   *renderer->getWindow());
	renderer->start(60);

	return 0;
}

void mapKeyBindings(){

    JoystickTranslator::getInstance()->init("../config/controls.json");

    ControlsManager *cm = ControlsManager::getInstance();
    cm->addBindings(ALTITUDE, {
            new KeyboardButton(sf::Keyboard::L, sf::Keyboard::P),
            new JoystickAxis(IJoystickTranslation::RIGHT_THUMBSTICK_Y,
                             true),
            new MouseAxis(MouseAxis::Axis::Y, 2.0f)});

    cm->addBindings(ACCELERATE, {
            new KeyboardButton(sf::Keyboard::S, sf::Keyboard::W),
            new JoystickAxis(IJoystickTranslation::LEFT_THUMBSTICK_Y,
                             true)});

    cm->addBindings(TURN, {
            new KeyboardButton(sf::Keyboard::D, sf::Keyboard::A),
            new JoystickAxis(IJoystickTranslation::RIGHT_THUMBSTICK_X,
                             true),
            new MouseAxis(MouseAxis::Axis::X, 3.0f)});

    cm->addBindings(SHOOT, {
            new KeyboardButton(sf::Keyboard::Space),
            new JoystickAxis(IJoystickTranslation::RT, false),
            new MouseButton(sf::Mouse::Button::Left)});

    cm->addBindings(QUIT, {new KeyboardButton(sf::Keyboard::Escape)});
    cm->addBindings(CONTINUE, {new KeyboardButton(sf::Keyboard::Return),
                               new JoystickButton(IJoystickTranslation::A)});
}

void createEffects() {
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
    reflectionCubeMap = new CubeMapTexture(
            "../scenes/space.png", "../scenes/space.png",
            "../scenes/space.png", "../scenes/space.png",
            "../scenes/space.png", "../scenes/space.png");
    scene.cubeMap = reflectionCubeMap;
}

Layout* createLayout(){
    Texture* meter = ResourceManager::loadAndFetchTexture("../scenes/HUD/meter2.0.png");
    Texture* arrow = ResourceManager::loadAndFetchTexture("../scenes/HUD/arrow.png");

    Layout* rootLayout = new ListLayout(ListLayout::VERTICAL,Dimension::fromPercentage(100),Dimension::fromPercentage(100));
    rootLayout->addChild(new ListLayout(ListLayout::VERTICAL,Dimension::fill(),Dimension::fill()));
    Layout* bottomBar = new ListLayout(ListLayout::HORIZONTAL,Dimension::fill(),Dimension::wrap());
    bottomBar->addChild(new ListLayout(ListLayout::VERTICAL,Dimension::fill(),Dimension::fromPixels(200)));

    PositioningLayout* meterL = new PositioningLayout(Dimension::fromPixels(200),Dimension::fromPixels(200));
    meterL->setBackground(new HUDGraphic(meter));
    PositioningLayout* arrowL = new PositioningLayout(Dimension::fromPixels(20),Dimension::fromPixels(120));
    arrowL->setBackground(new HUDGraphic(arrow,Dimension::fromPercentage(0),Dimension::fromPercentage(-33.0f)));
    arrowL->setId("speedArrow");
    meterL->addChild(arrowL,Dimension::fromPixels(90),Dimension::fromPixels(80));

    bottomBar->addChild(meterL);
    rootLayout->addChild(bottomBar);
    return rootLayout;
}

void createMeshes() {
    Logger::logInfo("Started loading meshes");


    // SKYBOX
    Mesh *skyBoxM = ResourceManager::loadAndFetchMesh("../scenes/sphere.obj");
    skyBox = SpaceBubbaObject(skyBoxM, Environment);
    SkyBoxRenderer *skyboxRenderer = new SkyBoxRenderer(playerCamera,
                                                        skyBoxM, &skyBox);
    skyboxRenderer->init("../scenes/x.png", "../scenes/fancyx.png",
                         "../scenes/fancyy.png", "../scenes/y.png",
                         "../scenes/fancyz.png", "../scenes/z.png");
    skyBox.addRenderComponent(skyboxRenderer);
    scene.shadowCasters.push_back(&skyBox);

    // OBJECTS
    Shader* standardShader = ResourceManager::getShader(SIMPLE_SHADER_NAME);
    standardShader->setUniformBufferObjectBinding(
            UNIFORM_BUFFER_OBJECT_MATRICES_NAME,
            UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);

    Mesh* rWingM = ResourceManager::loadAndFetchMesh("../scenes/R_wing.obj");
    Mesh* rWingCollision = ResourceManager::loadAndFetchMesh(
            "../scenes/Rwing collision.obj");
    rWing = new SpaceBubbaObject(rWingM, rWingCollision, Player);
    rWing->addCollidesWith(Asteroid);
    rWing->move(make_translation(make_vector(0.0f, 0.0f, 0.0f)));
    StandardRenderer *carRenderer = new StandardRenderer(rWingM, rWing,
                                                         standardShader);
    rWing->addRenderComponent(carRenderer);

    DeathOnCollision* wingD = new DeathOnCollision(rWing, Asteroid, 0, &points);
    rWing->addComponent(wingD);

    Texture *particleTexture = ResourceManager::loadAndFetchTexture(
            "../scenes/fire_part.png");

    FireParticle *fireConf = new FireParticle();
    ParticleGenerator *gen = new ParticleGenerator(
            particleTexture, 500, playerCamera,
            make_vector(0.0f, 0.0f, 0.0f), fireConf);
    SpaceBubbaObject *particleGenerator = new SpaceBubbaObject();
    particleGenerator->addRenderComponent(gen);
    scene.transparentObjects.push_back(particleGenerator);

    FireParticle *fireConf2 = new FireParticle();
    ParticleGenerator *gen2 = new ParticleGenerator(
            particleTexture, 500, playerCamera,
            make_vector(0.0f, 0.0f, 0.0f), fireConf2);
    SpaceBubbaObject *particleGenerator2 = new SpaceBubbaObject();
    particleGenerator2->addRenderComponent(gen2);
    scene.transparentObjects.push_back(particleGenerator2);


    // HUD
    hud = new SpaceBubbaObject();
    HudRenderer *hudRenderer = new SpaceBubbaHudRenderer(rWingSpeed);
    hudRenderer->setLayout(createLayout());
    hud->addRenderComponent(hudRenderer);
    scene.transparentObjects.push_back(hud);


    spaceMover = new SpaceShipComponent(rWingSpeed, &camera_theta, &camera_phi,
                                        rWing, gen, gen2, &state);
    ShootComponent *shooter = new ShootComponent(rWing, spaceMover, &scene,
                                                 &broadPhaseCollider, 1000);
    rWing->addComponent(shooter);
    rWing->addComponent(spaceMover);
    rWing->setDynamic(true);
    scene.shadowCasters.push_back(rWing);
    broadPhaseCollider.addGameObject(rWing);

    Mesh* dstarM = ResourceManager::loadAndFetchMesh("../scenes/dstar.obj");
    dstar = SpaceBubbaObject(dstarM, SpaceEntity);
    StandardRenderer *dstarRenderer = new StandardRenderer(dstarM, &dstar,
                                                           standardShader);
    dstar.addRenderComponent(dstarRenderer);

    MoveComponent *dstarMover = new MoveComponent(&dstar);
    dstarMover->setRotationSpeed(make_quaternion_axis_angle(UP_VECTOR, 0.001f));
    dstar.setLocation(make_vector(-10.0f, 0.0f, 16000.0f));
    dstar.setScale(make_vector(2000.0f, 2000.0f, 2000.0f));
    dstar.setDynamic(true);

    dstar.addComponent(dstarMover);
    scene.shadowCasters.push_back(&dstar);
    broadPhaseCollider.addGameObject(&dstar);

    Mesh* planetM = ResourceManager::loadAndFetchMesh("../scenes/planet.obj");
    planet = SpaceBubbaObject(planetM, SpaceEntity);
    StandardRenderer *planetRenderer = new StandardRenderer(planetM, &planet,
                                                            standardShader);
    planet.addRenderComponent(planetRenderer);
    MoveComponent *planetMover = new MoveComponent(&planet);
    planet.setLocation(make_vector(-25000.0f, 0.0f, 0.0f));
    planet.setScale(make_vector(2000.0f, 2000.0f, 2000.0f));
    planetMover->setRotationSpeed(make_quaternion_axis_angle(UP_VECTOR,
                                                             0.0005f));
    planet.addComponent(planetMover);

    scene.shadowCasters.push_back(&planet);
    broadPhaseCollider.addGameObject(&planet);

    Mesh* sunM = ResourceManager::loadAndFetchMesh("../scenes/sun.obj");
    sun = SpaceBubbaObject(sunM, SpaceEntity);
    MoveComponent *sunMover = new MoveComponent(&sun);
    sun.setLocation(make_vector(20000.0f, 0.0f, 0.0f));
    sun.setScale(make_vector(2000.0f, 2000.0f, 2000.0f));
    sunMover->setRotationSpeed(make_quaternion_axis_angle(UP_VECTOR, 0.001f));
    sun.addComponent(sunMover);
    StandardRenderer *sunRenderer = new StandardRenderer(sunM, &sun,
                                                         standardShader);
    sun.addRenderComponent(sunRenderer);

    scene.shadowCasters.push_back(&sun);
    broadPhaseCollider.addGameObject(&sun);


    for (int i = 0; i < 50; i++) {
        int rock = static_cast<int> (ceil(getRand(0.01f, 3.0f)));
        Mesh* asteroidM = ResourceManager::loadAndFetchMesh(
                "../scenes/rock" + to_string(rock) + ".obj");
        Mesh* astCollission = ResourceManager::loadAndFetchMesh(
                "../scenes/rock" + to_string(rock) + " collision.obj");
        SpaceBubbaObject *asteroid = new SpaceBubbaObject(asteroidM,
                                              astCollission, Asteroid);
        asteroid->addCollidesWith(Laser);
        StandardRenderer *asteroidRenderer = new StandardRenderer(
                asteroidM, asteroid, standardShader);
        asteroid->addRenderComponent(asteroidRenderer);
        asteroid->setDynamic(true);

        float3 location = createRandomVector(-200.0f, 200.0f) +
                make_vector(150.0f, 100.0f, 600.0f);
        float3 velocity = createRandomVector(-0.015f, 0.015f);
        float3 rotation = createRandomVector(-1.0f, 1.0f);

        location += rWing->getLocation();

        MoveComponent *asteroidMover = new MoveComponent(asteroid);
        asteroidMover->setVelocity(velocity);
        asteroidMover->setRotationSpeed(make_quaternion_axis_angle(
                                        rotation, getRand(0.0009, 0.0025)));
        asteroid->setLocation(location);
        // asteroidMover->setAcceleration(make_vector(-0.0000005f, 0.0f, 0.0f));
        asteroidMover->setScaleSpeed(make_vector(0.0005f, 0.0005f, 0.0005f));
        asteroid->addComponent(asteroidMover);

        DeathOnCollision* dca = new DeathOnCollision(asteroid, Laser,
                                                     1, &points);
        asteroid->addComponent(dca);

        SpawnAsteroidOnDeath *childrenSpawner = new SpawnAsteroidOnDeath(
                asteroid, &scene, &broadPhaseCollider,
                make_vector(1.0f, 1.0f, 1.0f), playerCamera, &points);
        asteroid->addComponent(childrenSpawner);

        scene.shadowCasters.push_back(asteroid);
        broadPhaseCollider.addGameObject(asteroid);
    }

    Logger::logInfo("Finished loading meshes.");
}

void createCameras() {
    int w = SCREEN_WIDTH;
    int h = SCREEN_HEIGHT;

    sunCamera = new PerspectiveCamera(make_vector(20000.0f, 0.0f, 0.0f),
                                      make_vector(0.0f, 0.0f, 0.0f), UP_VECTOR,
                                      45, 1.0f, 10.0f, 30000.0f);
    scene.shadowMapCamera = sunCamera;

    playerCamera = new PerspectiveCamera(
            sphericalToCartesian(camera_theta, camera_phi, camera_r),
            make_vector(0.0f, camera_target_altitude, 0.0f),
            UP_VECTOR,
            45, (float)w/(float)h, 0.1f, 50000.0f);
}

void startAudio() {
    sf::Music* music = AudioManager::loadAndFetchMusic("../scenes/ambient.ogg");
    music->setLoop(true);
    music->play();
}

// Helper function to turn spherical coordinates into cartesian (x,y,z)
float3 sphericalToCartesian(float theta, float phi, float r) {
    return make_vector(r * sinf(theta)*sinf(phi),
                       r * cosf(phi),
                       r * cosf(theta)*sinf(phi));
}
