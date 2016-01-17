//
// Created by simon on 2015-12-12.
//

#include <MoveComponent.h>
#include <ParticleGenerator.h>
#include <Utils.h>
#include <ControlsManager.h>
#include "SpaceShipComponent.h"
#include "float3x3.h"
#include <Controls.h>
#include <linmath/Quaternion.h>
#include <MousePosition.h>


SpaceShipComponent::SpaceShipComponent(struct HudRenderer::HudConfig* hudConf, float* cameraThetaLocation,
                                       float* cameraPhiLocation, GameObject* ship, ParticleGenerator* generator1,
                                       ParticleGenerator* generator2, State* state)
        : MoveComponent(ship){
    this->hudConf = hudConf;
    this->cameraThetaLocation = cameraThetaLocation;
    this->generator1 = generator1;
    this->generator2 = generator2;
    this->state = state;
    this->cameraPhiLocation = cameraPhiLocation;


}

void SpaceShipComponent::onDeath() {
        generator1->setLooping(false);
        generator2->setLooping(false);
    *state = Died;
}

void SpaceShipComponent::update(float dt) {

    checkKeyPresses(dt);
    setVelocity(normalize(frontDir)*length(getVelocity()));
    MoveComponent::update(dt);
    hudConf->speed = length(getVelocity())*200;

    generator1->m_position = getLocation() - frontDir * 4.0 + rightDir;
    generator2->m_position = getLocation() - frontDir * 4.0 - rightDir;
};

float3 SpaceShipComponent::getUpDir() {
    return upDir;
}

void SpaceShipComponent::updateRot() {
    float3x3 mat3 = toMatrix3x3(getRotation());
    upDir = mat3*originalUpVector;
    rightDir = mat3*originalRightVector;
    frontDir = mat3*originalFrontVector;
}

void SpaceShipComponent::checkKeyPresses(float dt) {

    ControlsManager* cm = ControlsManager::getInstance();
    ControlStatus cs = cm->getStatus(ACCELERATE);
    if (cs.isActive()) {
        if(abs(accelerationSpeed) >= 0.00007f) {
            float ratio = 0.00007f/abs(accelerationSpeed);
            accelerationSpeed *= ratio;
        } else
            accelerationSpeed += 0.0000005f;
        setAcceleration(normalize(frontDir)*accelerationSpeed * -(cs.getValue() / 100.0f));
    }else{
        setAcceleration(make_vector(0.0f,0.0f,0.0f));
        accelerationSpeed = 0.0f;
    }

    if(length(getVelocity()) > maxSpeed) {
        printf("yolo\n");
        setVelocity(normalize(getVelocity()) * maxSpeed);
    }

    cs = cm->getStatus(ALTITUDE);
    float speedDif = turnSpeed*(cs.getValue() /150.0f);
    if(cs.isActive()){
        updateRotation(make_quaternion_axis_angle(rightDir,speedDif*dt));
        updateRot();
        *cameraPhiLocation -= speedDif*dt;
    }

    ControlStatus cs2 = cm->getStatus(TURN);
    speedDif = turnSpeed*-(cs2.getValue() / 150.0f);
    if (cs2.isActive()) {
        updateRotation(make_quaternion_axis_angle(upDir,speedDif*dt));
        updateRot();
        *cameraThetaLocation += speedDif*dt;
    }
}

float3 SpaceShipComponent::getFrontDir() { return frontDir; }
