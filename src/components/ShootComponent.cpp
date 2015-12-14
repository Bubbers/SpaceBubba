//
// Created by johan on 2015-12-13.
//
#include <ResourceManager.h>
#include <Utils.h>
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
#include "InputManager.h"


ShootComponent::ShootComponent(GameObject* object, SpaceShipComponent *objectMover, Scene *scene, BFBroadPhase *collisionHandler, float timeToLive) {
    this->object = object;
    this->objectMover = objectMover;
    this->scene = scene;
    this->collisionHandler = collisionHandler;
    this->timeToLive = timeToLive;

}

void ShootComponent::update(float dt) {
    InputManager* im = InputManager::getInstance();
    long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    if(im->isKeyDown((int)'r', false) && ms > canShootAfter) {
        canShootAfter = ms + 500;
        spawnBullet();
    }
}


void ShootComponent::spawnBullet() {

    float4 ps = object->getModelMatrix()->c4;
    float3 location = make_vector(ps.x, ps.y, ps.z);

    Shader* standardShader = ResourceManager::getShader(SIMPLE_SHADER_NAME);
    standardShader->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);


    Mesh* shotM = ResourceManager::loadAndFetchMesh("../scenes/shot.obj");
    GameObject *shot = new GameObject(shotM, Laser);
    //shot->move(make_translation(location));
    //shot->update(make_rotation_y<float4x4>(degreeToRad(90)));

    StandardRenderer *shotRenderer = new StandardRenderer(shotM, shot->getModelMatrix(), standardShader);
    shot->addRenderComponent(shotRenderer);
    shot->setDynamic(true);

    MoveComponent *shotMover = new MoveComponent(shot);
    shotMover->setRotation(objectMover->getRotation());
    shotMover->setVelocity(normalize(objectMover->getFrontDir()) / 1.8f);
    shotMover->setLocation(location + normalize(objectMover->getFrontDir())*6);
    TimedLife *tl = new TimedLife(shot, timeToLive);
    shot->addComponent(tl);
    shot->addComponent(shotMover);

    DeathOnCollision* dc = new DeathOnCollision(shot, Asteroid);
    shot->addComponent(dc);

    scene->shadowCasters.push_back(shot);
    collisionHandler->addGameObject(shot);
}
