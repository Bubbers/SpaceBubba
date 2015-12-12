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
    maxSpeed = 10.0f;
}

void SpaceShipComponent::update(float dt) {

    checkKeyPresses();
    MoveComponent::update(dt);
    hudConf->speed = length(velocity);

};

void SpaceShipComponent::checkKeyPresses() {

    InputManager* im = InputManager::getInstance();
    if (im->isKeyDown('w',false) || im->isKeyDown('s',false)) {
        if(length(acceleration) >= 0.003f) {
            float ratio = 0.003f/length(acceleration);
            acceleration *= ratio;
        }
        acceleration = acceleration + 0.00005f*(im->isKeyDown('w',false) ? 1.0f : -1.0f);
    }else{
        acceleration = make_vector(0.0f,0.0f,0.0f);
    }

    if (im->isKeyDown('a',false)) {
        angley += rotationSpeed;
        *cameraThetaLocation += rotationSpeed;

    }
    if (im->isKeyDown('d',false)) {
        angley -= rotationSpeed;
        *cameraThetaLocation -= rotationSpeed;
    }
}