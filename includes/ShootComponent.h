//
// Created by johan on 2015-12-13.
//

#ifndef BUBBA_3D_SHOOTCOMPONENT_H
#define BUBBA_3D_SHOOTCOMPONENT_H

#include "IComponent.h"
#include <timer.h>
#include "AudioManager.h"

class GameObject;

class ShootComponent : public IComponent {
public:
    ShootComponent(GameObject* object, SpaceShipComponent *objectMover, Scene *scene, float timeToLive);
    ~ShootComponent();
    virtual void update(float dt);

private:
    GameObject* object;
    SpaceShipComponent *objectMover;
    Scene *scene;
    float timeToLive = 0;
    long canShootAfter = 0;
	utils::Timer timer;
    sf::Sound* sound;

    void spawnBullet();
};


#endif //BUBBA_3D_SHOOTCOMPONENT_H
