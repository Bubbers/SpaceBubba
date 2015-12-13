#include "DeathOnCollision.h"

DeathOnCollision::DeathOnCollision(GameObject *parent, GameObjectType relevantType) {
    this->relevantType = relevantType;
    this->parent = parent;
}

DeathOnCollision::DeathOnCollision(GameObject *parent, GameObjectType relevantType, int pointsWorth, int *scoreBoard) :
        DeathOnCollision(parent, relevantType) {
    this->pointsWorth = pointsWorth;
    this->scoreBoard = scoreBoard;
}

void DeathOnCollision::update(float dt) {
//    pointsWorth += int(dt / 1000.0f);
}

void DeathOnCollision::beforeCollision() {

    if (pointsWorth > 0) {
        *scoreBoard += pointsWorth;
    }
    parent->makeDirty();
}
