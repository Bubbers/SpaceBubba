//
// Created by simon on 2015-12-13.
//

#include "SpawnAsteroidOnDeath.h"
#include <Shader.h>
#include <ResourceManager.h>
#include <constants.h>
#include <GameObject.h>
#include <StandardRenderer.h>
#include <MoveComponent.h>
#include <Utils.h>
#include <BFBroadPhase.h>
#include <Scene.h>
#include <SmokeParticle.h>
#include <ParticleGenerator.h>

SpawnAsteroidOnDeath::SpawnAsteroidOnDeath(GameObject* gameObject, Scene *scene, BFBroadPhase *collisionHandler, float3 scale, Camera* camera){
    this->gameObject = gameObject;
    this->scene = scene;
    this->collisionHandler = collisionHandler;
    this->scale = scale;
    this->camera = camera;
}

void SpawnAsteroidOnDeath::onDeath(){
    float4 ps = gameObject->getModelMatrix()->c4;
    float3 location = make_vector(ps.x, ps.y, ps.z);

    Shader* standardShader = ResourceManager::getShader(SIMPLE_SHADER_NAME);
    standardShader->setUniformBufferObjectBinding(UNIFORM_BUFFER_OBJECT_MATRICES_NAME, UNIFORM_BUFFER_OBJECT_MATRICES_INDEX);

    for(int i = 0 ; i<4 ; i++) {

        string strings[] = {"../scenes/rock1.obj", " ../scenes/rock2.obj", "../scenes/rock3.obj" };
        string mesh = getRandomElem(strings);
        Mesh* asteroidM = ResourceManager::loadAndFetchMesh(mesh);
        GameObject *asteroid = new GameObject(asteroidM);

        StandardRenderer *asteroidRenderer = new StandardRenderer(asteroidM, asteroid->getModelMatrix(), standardShader);
        asteroid->addRenderComponent(asteroidRenderer);
        asteroid->setDynamic(false);

        MoveComponent *asteroidMover = new MoveComponent(asteroid);
        asteroidMover->setRotation(createRandomVector(-0.01f, 0.01f));
        asteroidMover->setVelocity(createRandomVector(-0.01f, 0.01f));
        //asteroidMover->setScale(scale*0.25);
        asteroidMover->setLocation(location);
        asteroid->addComponent(asteroidMover);
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