//
// Created by simon on 2015-12-13.
//

#ifndef BUBBA_3D_SPAWNONDEATH_H
#define BUBBA_3D_SPAWNONDEATH_H

#include "IComponent.h"
#include "float3.h"

using namespace chag;

class Scene;
class Camera;
class BFBroadPhase;
class GameObject;

class SpawnAsteroidOnDeath : public IComponent{

public:
    SpawnAsteroidOnDeath(GameObject* object, Scene *scene, BFBroadPhase *collisionHandler, float3 scale, Camera* camera, int* points);

    void onDeath() ;
    void update(float dt) {};

private:
    GameObject* gameObject;
    Scene* scene;
    BFBroadPhase* collisionHandler;
    float3 scale;
    Camera* camera;
    int* points;

};


#endif //BUBBA_3D_SPAWNONDEATH_H
