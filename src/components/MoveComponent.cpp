//
// Created by johan on 2015-11-29.
//

#include <GameObject.h>
#include <linmath/Quaternion.h>
#include <Logger.h>
#include <timer.h>
#include <sstream>
#include <InputManager.h>
#include <HudRenderer.h>
#include "MoveComponent.h"
#include "float3x3.h"
#include "Utils.h"

MoveComponent::MoveComponent(){

}

MoveComponent::MoveComponent(struct HudRenderer::HudConfig* hudConf, float* cameraThetaLocation, GameObject* carObject) {
    this->cameraThetaLocation = cameraThetaLocation;
    this->carObject = carObject;
    this->hudConf = hudConf;
}

void MoveComponent::update(float dt) {
    checkKeyPresses(dt);
    updateCarObject();
    hasChanged = false;
    anglex = 0;
    angley = 0;
    anglez = 0;
}

void MoveComponent::afterCollision() {
    Logger::logInfo("AfterCollision");
}

void MoveComponent::duringCollision() {
    Logger::logInfo("DuringCollision");
}

void MoveComponent::beforeCollision() {
    Logger::logInfo("BeforeCollision");
}



void MoveComponent::checkKeyPresses(float dt) {

    InputManager* im = InputManager::getInstance();
    if (im->isKeyDown('w',false)) {
        float3 term = frontDir * moveSpeed;
        velocity += term * dt;

        hasChanged = true;

    }
    if (im->isKeyDown('s',false)) {
        float3 term = frontDir * moveSpeed;
        velocity -= term * dt;
        hasChanged = true;

    }
    if(im->isKeyDown('w',false) || im->isKeyDown('s',false)){

        float speed = sqrt(velocity.x*velocity.x+velocity.y*velocity.y + velocity.z*velocity.z);
        if(abs(speed) > maxSpeed){
            float ratio = maxSpeed/abs(speed);
            velocity = make_vector(velocity.x*ratio,velocity.y*ratio,velocity.z*ratio);
        }
        hudConf->speed = (int)floor(speed);

    }
    if (im->isKeyDown('a',false)) {
        angley += rotationSpeed;
        *cameraThetaLocation += rotationSpeed;
        hasChanged = true;

    }
    if (im->isKeyDown('d',false)) {
        angley -= rotationSpeed;
        *cameraThetaLocation -= rotationSpeed;
        hasChanged = true;
    }
}

void MoveComponent::updateCarObject(){
    float3 vUp = make_vector(0.0f, 1.0f, 0.0f);

    float3 n_frontDir = normalize(frontDir);
    float3 rightDir = normalize(cross(frontDir, vUp));

    float anglex = -(degreeToRad(90.0f) - acosf(dot(normalize(upDir), n_frontDir)));
    float anglez = (degreeToRad(90.0f) - acosf(dot(normalize(upDir), rightDir)));

    Quaternion qatX = make_quaternion_axis_angle(rightDir, anglex);
    Quaternion qatY = make_quaternion_axis_angle(vUp, angley);
    Quaternion qatZ = make_quaternion_axis_angle(make_rotation_y<float3x3>(-angley) * n_frontDir, anglez);

    carObject->update(make_translation(location + velocity));
    carObject->update(makematrix(qatX));
    carObject->update(makematrix(qatY));
    carObject->update(makematrix(qatZ));
}

