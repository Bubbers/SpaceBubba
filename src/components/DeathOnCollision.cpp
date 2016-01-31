
#include "DeathOnCollision.h"

DeathOnCollision::DeathOnCollision(SpaceBubbaObject *parent, GameObjectType relevantType) {
    this->relevantType = relevantType;
    this->parent = parent;
}

DeathOnCollision::DeathOnCollision(SpaceBubbaObject *parent, GameObjectType relevantType, int pointsWorth, int *scoreBoard) :
        DeathOnCollision(parent, relevantType) {
    this->pointsWorth = pointsWorth;
    this->scoreBoard = scoreBoard;
}

void DeathOnCollision::update(float dt) {
//    pointsWorth += int(dt / 1000.0f);
}

void DeathOnCollision::beforeCollision(GameObject* collider) {


    if(relevantType == collider->getIdentifier()) {

        if (pointsWorth > 0) {
            *scoreBoard += pointsWorth;
        }
        parent->makeDirty();
    }
}
