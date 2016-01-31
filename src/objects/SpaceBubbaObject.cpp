//
// Created by simon on 2016-01-30.
//

#include "SpaceBubbaObject.h"


void SpaceBubbaObject::setType(GameObjectType type) {
    setIdentifier(type);
}

SpaceBubbaObject::SpaceBubbaObject(Mesh* model, Mesh* collisionMesh, GameObjectType type) : GameObject(model,collisionMesh){
    setType(type);
}
SpaceBubbaObject::SpaceBubbaObject(Mesh* model, GameObjectType type) : GameObject(model){
    setType(type);
}
SpaceBubbaObject::SpaceBubbaObject() : GameObject(){}