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

MoveComponent::MoveComponent(GameObject* meshObject, float3 rotation, float3 velocity, float3 acceleration) : MoveComponent (meshObject){
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->rotation = rotation;
}

void MoveComponent::update(float dt) {
    updateMeshObject(dt);
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

    velocity += acceleration*dt;
    if(length(velocity) > maxSpeed){
        float ratio = maxSpeed/length(velocity);
        velocity = ratio*velocity;
    }

    Quaternion qatX = make_quaternion_axis_angle(make_vector(1.0f,0.0f,0.0f), rotation.x);
    Quaternion qatY = make_quaternion_axis_angle(make_vector(0.0f,1.0f,0.0f), rotation.y);
    Quaternion qatZ = make_quaternion_axis_angle(make_vector(0.0f,0.0f,1.0f), rotation.z);

    meshObject->update(make_translation(velocity*dt));
    meshObject->update(makematrix(qatX));
    meshObject->update(makematrix(qatY));
    meshObject->update(makematrix(qatZ));
}

