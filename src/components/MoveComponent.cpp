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

MoveComponent::MoveComponent(GameObject* meshObject) {
    this->meshObject = meshObject;
    maxSpeed = INT32_MAX;
}

MoveComponent::MoveComponent(GameObject* meshObject, float3 location, float3 velocity, float3 acceleration) : MoveComponent (meshObject){
    this->location = location;
    this->velocity = velocity;
    this->acceleration = acceleration;
}

void MoveComponent::update(float dt) {
    updateMeshObject(dt);
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

void MoveComponent::updateMeshObject(float dt){
    float3 vUp = make_vector(0.0f, 1.0f, 0.0f);

    velocity += acceleration*dt;
    if(length(velocity) > maxSpeed){
        float ratio = maxSpeed/length(velocity);
        velocity = ratio*velocity;
    }

    float3 n_frontDir = normalize(frontDir);
    float3 rightDir = normalize(cross(frontDir, vUp));

    float anglex = -(degreeToRad(90.0f) - acosf(dot(normalize(upDir), n_frontDir)));
    float anglez = (degreeToRad(90.0f) - acosf(dot(normalize(upDir), rightDir)));

    Quaternion qatX = make_quaternion_axis_angle(rightDir, anglex);
    Quaternion qatY = make_quaternion_axis_angle(vUp, angley);
    Quaternion qatZ = make_quaternion_axis_angle(make_rotation_y<float3x3>(-angley) * n_frontDir, anglez);

    meshObject->update(make_translation(location + velocity*frontDir));
    meshObject->update(makematrix(qatX));
    meshObject->update(makematrix(qatY));
    meshObject->update(makematrix(qatZ));
}

