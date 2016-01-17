//
// Created by johan on 2015-12-13.
//
#include <ResourceManager.h>
#include <StandardRenderer.h>
#include <constants.h>
#include <MoveComponent.h>
#include <TimedLife.h>
#include <SpaceShipComponent.h>
#include <Scene.h>
#include <BFBroadPhase.h>
#include <DeathOnCollision.h>
#include <chrono>
#include "ShootComponent.h"
#include <timer.h>
#include <ControlsManager.h>
#include <Controls.h>


#ifdef WIN32
#include <Windows.h>
#endif


ShootComponent::ShootComponent(GameObject* object, SpaceShipComponent *objectMover, Scene *scene, BFBroadPhase *collisionHandler, float timeToLive) {
    this->object = object;
    this->objectMover = objectMover;
    this->scene = scene;
    this->collisionHandler = collisionHandler;
    this->timeToLive = timeToLive;
	timer.start();

    sound = AudioManager::loadAndFetchSound("../scenes/laser.wav");
}

ShootComponent::~ShootComponent() {
    sound->stop();
    delete sound;
}

void ShootComponent::update(float dt) {
    ControlsManager* cm = ControlsManager::getInstance();

#ifdef WIN32
	SYSTEMTIME time;
	GetSystemTime(&time);
	long ms = time.wHour *60*60*1000 + time.wMinute *60*1000 + time.wSecond * 1000 + time.wMilliseconds;
#endif
#ifdef __linux__
	long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
#endif



    if(cm->getStatus(SHOOT).isActive() && ms > canShootAfter) {
		
        canShootAfter = ms + 500;
        spawnBullet();
		timer.start();

        sound->play();
    }
}


void ShootComponent::spawnBullet() {

    float4 ps = object->getModelMatrix().c4;
    float3 location = make_vector(ps.x, ps.y, ps.z);
    float3 shipVelocity = objectMover->getVelocity();


    Shader* standardShader = ResourceManager::getShader(SIMPLE_SHADER_NAME);
    standardShader->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);


    Mesh* shotM = ResourceManager::loadAndFetchMesh("../scenes/shot.obj");
    GameObject *shot = new GameObject(shotM, Laser);
    //shot->move(make_translation(location));
    //shot->update(make_rotation_y<float4x4>(degreeToRad(90)));

    StandardRenderer *shotRenderer = new StandardRenderer(shotM, shot, standardShader);
    shot->addRenderComponent(shotRenderer);
    shot->setDynamic(true);

    MoveComponent *shotMover = new MoveComponent(shot);
    Quaternion rot = objectMover->getRotation();
    shotMover->setRotation(rot);
    shotMover->setVelocity(shipVelocity + normalize(objectMover->getFrontDir()) / 1.8f);
    shotMover->setLocation(location + normalize(objectMover->getFrontDir())*6);
    TimedLife *tl = new TimedLife(shot, timeToLive);
    shot->addComponent(tl);
    shot->addComponent(shotMover);

    DeathOnCollision* dc = new DeathOnCollision(shot, Asteroid);
    shot->addComponent(dc);

    scene->shadowCasters.push_back(shot);
    collisionHandler->addGameObject(shot);


}
