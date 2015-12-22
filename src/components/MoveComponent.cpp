//
// Created by johan on 2015-11-29.
//

#include <GameObject.h>
#include "MoveComponent.h"
#include "float3x3.h"

MoveComponent::MoveComponent(){

}

MoveComponent::MoveComponent(GameObject* meshObject) {
    this->meshObject = meshObject;
    maxSpeed = FLT_MAX;
}

MoveComponent::MoveComponent(GameObject* meshObject, float3 rotationSpeed, float3 rotation,
                             float3 velocity, float3 location, float3 acceleration,
                             float3 scale, float3 scaleSpeed) : MoveComponent (meshObject){
    this->velocity = velocity;
    this->acceleration = acceleration;
    this->rotation = rotation;
    this->rotationSpeed = rotationSpeed;
    this->location = location;
    this->scale = scale;
    this->scaleSpeed = scaleSpeed;
}

void MoveComponent::update(float dt) {
    updateMeshObject(dt);
}

void MoveComponent::afterCollision() {
}

void MoveComponent::duringCollision() {
}

void MoveComponent::beforeCollision() {
}


void MoveComponent::updateMeshObject(float dt){

    velocity += acceleration*dt;
    if(length(velocity) > maxSpeed){
        float ratio = maxSpeed/length(velocity);
        velocity = ratio*velocity;
    }

    location += velocity*dt;
    rotation += rotationSpeed*dt;
    scale += scaleSpeed*dt;


    meshObject->move(make_translation(location)*
                             make_rotation_x<float4x4>(rotation.x)*
                             make_rotation_y<float4x4>(rotation.y) *
                             make_rotation_z<float4x4>(rotation.z)*
                             make_scale<float4x4>(scale));


}

float3 MoveComponent::getVelocity(){ return velocity; }
float3 MoveComponent::getAcceleration(){ return acceleration; }
float3 MoveComponent::getRotation() { return rotation; }
float3 MoveComponent::getLocation(){ return location; }
float3 MoveComponent::getRotationSpeed(){ return rotationSpeed; }
float3 MoveComponent::getScale(){ return scale; }
float3 MoveComponent::getScaleSpeed(){ return scaleSpeed; }

void MoveComponent::setVelocity(float3 v){velocity = v;}
void MoveComponent::setAcceleration(float3 a){acceleration = a;}
void MoveComponent::setRotation(float3 r){rotation = r;}
void MoveComponent::setLocation(float3 l){location = l;}
void MoveComponent::setRotationSpeed(float3 rs){rotationSpeed = rs;}
void MoveComponent::setScale(float3 s){scale = s;}
void MoveComponent::setScaleSpeed(float3 ss){scaleSpeed = ss;}
