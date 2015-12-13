//
// Created by simon on 2015-12-12.
//

#include <MoveComponent.h>
#include <InputManager.h>
#include "SpaceShipComponent.h"


SpaceShipComponent::SpaceShipComponent(struct HudRenderer::HudConfig* hudConf, float* cameraThetaLocation, GameObject* ship)
        : MoveComponent(ship){
    this->hudConf = hudConf;
    this->cameraThetaLocation = cameraThetaLocation;
    maxSpeed = 0.5f;
}

void SpaceShipComponent::update(float dt) {

    checkKeyPresses();
    MoveComponent::update(dt);
    hudConf->speed = length(velocity)*200;

};

void SpaceShipComponent::checkKeyPresses() {

    InputManager* im = InputManager::getInstance();
    if (im->isKeyDown('w',false) || im->isKeyDown('s',false)) {
        if(length(acceleration) >= 0.00007f) {
            float ratio = 0.00007f/length(acceleration);
            acceleration *= ratio;
        }
        acceleration = (acceleration + 0.000005f*(im->isKeyDown('w',false) ? 1.0f : -1.0f))*make_vector(0.0f,0.0f,1.0f);
    }else{
        acceleration = make_vector(0.0f,0.0f,0.0f);
    }

    if (im->isKeyDown('a',false)) {
        rotation.y = rotationSpeed;
        *cameraThetaLocation += rotationSpeed;
    } else if (im->isKeyDown('d',false)) {
        rotation.y = - rotationSpeed;
        *cameraThetaLocation -= rotationSpeed;
    }else
        rotation.y = 0;
}