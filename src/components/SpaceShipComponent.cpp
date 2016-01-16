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
    maxSpeed = 0.5f;
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
    MoveComponent::updateWithExtras(dt,make_identity<float4x4>(),make_identity<float4x4>(),updateRotation(dt),
                                    make_identity<float4x4>());
    hudConf->speed = length(getVelocity())*200;


	float4x4 modelMatrix = meshObject->getModelMatrix();
	float4 transformedUpVector = modelMatrix * originalUpVector;
	upDir = make_vector(transformedUpVector.x, transformedUpVector.y, transformedUpVector.z);

    float3 normVector = normalize(frontDir);
    float3 left = cross(normVector, upDir);
    generator1->m_position = getLocation() - normVector * 4.0 + left;
    generator2->m_position = getLocation() - normVector * 4.0 - left;
};

float3 SpaceShipComponent::getUpDir() {
    return upDir;
}

float4x4 SpaceShipComponent::updateRotation(float dt) {
    float4x4 roty = make_rotation_y<float4x4>(totalTurn); //to rotate the modelmatrix
    float3x3 rotvy = make_rotation_y<float3x3>(totalTurn); //to rotate frontDir
    float3 axisOriginalX = normalize(rotvy*make_vector(-1.0f,0.0f,0.0f)); //The axis that was to the right from the beginning
                                                                          //But should now be at the rotated position
    frontDir = make_rotation<float3x3>(axisOriginalX,totalIncl) * rotvy * make_vector(0.0f,0.0f,1.0f);
    setVelocity(frontDir*length(getVelocity()));
    return make_rotation<float4x4>(axisOriginalX,totalIncl)*roty;
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
    float speedDif = turnSpeed*-(cs.getValue() /150.0f);
    if(cs.isActive()){
        totalIncl += speedDif*dt;
        *cameraPhiLocation += speedDif*dt;
    }

    cs = cm->getStatus(TURN);
    speedDif = turnSpeed*-(cs.getValue() / 150.0f);
    if (cs.isActive()) {
        totalTurn += speedDif*dt;
        *cameraThetaLocation += speedDif*dt;
    }
}

float3 SpaceShipComponent::getFrontDir() { return frontDir; }
