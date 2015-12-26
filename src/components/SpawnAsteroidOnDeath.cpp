//
// Created by simon on 2015-12-13.
//

#include "SpawnAsteroidOnDeath.h"
#include <ResourceManager.h>
#include <constants.h>
#include <StandardRenderer.h>
#include <MoveComponent.h>
#include <SmokeParticle.h>
#include <ParticleGenerator.h>
#include <DeathOnCollision.h>

SpawnAsteroidOnDeath::SpawnAsteroidOnDeath(GameObject* gameObject, Scene *scene, BFBroadPhase *collisionHandler, float3 scale, Camera* camera, int* points){

    this->gameObject = gameObject;
    this->scene = scene;
    this->collisionHandler = collisionHandler;
    this->scale = scale;
    this->camera = camera;
    this->points = points;
}


void SpawnAsteroidOnDeath::onDeath(){

    if(length(scale) < 0.25f)
        return;

    float4 ps = gameObject->getModelMatrix().c4;
    float3 location = make_vector(ps.x, ps.y, ps.z);

    Shader* standardShader = ResourceManager::getShader(SIMPLE_SHADER_NAME);
    standardShader->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);

    for(int i = 0 ; i < 4 ; i++) {

        string strings[] = {"../scenes/rock1.obj", "../scenes/rock2.obj", "../scenes/rock3.obj" };
        string mesh = getRandomElem(strings);
        Mesh* asteroidM = ResourceManager::loadAndFetchMesh(mesh);
        GameObject *asteroid = new GameObject(asteroidM, Asteroid);

        StandardRenderer *asteroidRenderer = new StandardRenderer(asteroidM, asteroid, standardShader);
        asteroid->addRenderComponent(asteroidRenderer);
        asteroid->setDynamic(true);

        MoveComponent *asteroidMover = new MoveComponent(asteroid);
        asteroidMover->setRotationAxis(createRandomVector(-1.0f, 1.0f));
        asteroidMover->setRotationSpeed(getRand(0.001f,0.005f));
        asteroidMover->setVelocity(createRandomVector(-0.01f, 0.01f));
        asteroidMover->setScale(scale*4.0f);
        asteroidMover->setLocation(location + createRandomVector(1.0f,2.0f));
        asteroid->addComponent(asteroidMover);

        DeathOnCollision* dc = new DeathOnCollision(asteroid, Laser, 1, points);
        asteroid->addComponent(dc);

        SpawnAsteroidOnDeath* spawner = new SpawnAsteroidOnDeath(asteroid,scene,collisionHandler,scale*0.5f,camera,points);
        asteroid->addComponent(spawner);

        scene->shadowCasters.push_back(asteroid);
        collisionHandler->addGameObject(asteroid);

    }

    Texture *particleTexture = ResourceManager::loadAndFetchTexture("../scenes/smoke_part.png");

    float3 cameraLocationUnit = normalize(camera->getPosition() - location);

    SmokeParticle *smokeConf = new SmokeParticle();
    ParticleGenerator *gen = new ParticleGenerator(particleTexture, 200, camera, location + cameraLocationUnit * 5, smokeConf);
    GameObject *particleGenerator = new GameObject();
    particleGenerator->addRenderComponent(gen);
    scene->transparentObjects.push_back(particleGenerator);

}