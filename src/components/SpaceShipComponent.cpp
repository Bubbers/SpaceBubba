//
// Created by simon on 2015-12-12.
//

#include <MoveComponent.h>
#include <Utils.h>
#include <ControlsManager.h>
#include "SpaceShipComponent.h"
#include "linmath/float3x3.h"
#include <Controls.h>
#include <linmath/Quaternion.h>
#include <MousePosition.h>


SpaceShipComponent::SpaceShipComponent(float* speed,float* cameraThetaLocation,
                                       float* cameraPhiLocation, GameObject* ship, ParticleGenerator* generator1,
                                       ParticleGenerator* generator2, State* state) : MoveComponent(ship){
    this->cameraThetaLocation = cameraThetaLocation;
    this->state = state;
    this->cameraPhiLocation = cameraPhiLocation;
    this->speed = speed;

}

void SpaceShipComponent::onDeath() {
    *state = Died;
}

void SpaceShipComponent::update(float dt) {

    checkKeyPresses(dt);
    setVelocity(normalize(frontDir)*length(getVelocity()));
    MoveComponent::update(dt);

    *speed = length(getVelocity())/maxSpeed;

    generator1->m_position = meshObject->getLocation() - frontDir * 4.0 + rightDir;
    generator2->m_position = meshObject->getLocation() - frontDir * 4.0 - rightDir;
};

float3 SpaceShipComponent::getUpDir() {
    return upDir;
}

void SpaceShipComponent::updateRot() {
    float3x3 mat3 = toMatrix3x3(meshObject->getRelativeRotation());
    upDir = mat3*originalUpVector;
    rightDir = mat3*originalRightVector;
    frontDir = mat3*originalFrontVector;
}

void SpaceShipComponent::checkKeyPresses(float dt) {

    ControlsManager* cm = ControlsManager::getInstance();
    ControlStatus cs = cm->getStatus(ACCELERATE);
    if (cs.isActive() && (length(getVelocity()) < maxSpeed || cs.getValue() > 0)) {
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

    cs = cm->getStatus(ALTITUDE);
    float speedDif = turnSpeed*(cs.getValue() /150.0f);
    if(cs.isActive()){
        meshObject->updateRotation(make_quaternion_axis_angle(rightDir,speedDif*dt));
        updateRot();
        *cameraPhiLocation -= speedDif*dt;
    }

    ControlStatus cs2 = cm->getStatus(TURN);
    speedDif = turnSpeed*-(cs2.getValue() / 150.0f);
    if (cs2.isActive()) {
        meshObject->updateRotation(make_quaternion_axis_angle(upDir,speedDif*dt));
        updateRot();
        *cameraThetaLocation += speedDif*dt;
    }
}

float3 SpaceShipComponent::getFrontDir() { return frontDir; }
