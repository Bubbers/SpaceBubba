//
// Created by johan on 2015-12-05.
//

#ifndef BUBBA_3D_BRUTEFORCEBROADPHASE_H
#define BUBBA_3D_BRUTEFORCEBROADPHASE_H

#include "BroadPhase.h"

class BFBroadPhase : BroadPhase
{
public:
    BFBroadPhase();

    virtual void addGameObject(GameObject* GameObject);
    virtual CollisionPairList computeCollisionPairs() ;
    virtual void updateCollision();
private:
    bool isPossiblyColliding(GameObject* gameObject1, GameObject* gameObject2);
    void computeExactCollision(CollisionPairList possibleCollision);
    void triggerObjectEvent(GameObject* object1, GameObject* object2, EventType eventType);

    std::vector<GameObject*> GameObjectList;
    std::vector<std::pair<int, int>> collidingList;
    void removeDirty();

};

#endif //BUBBA_3D_BRUTEFORCEBROADPHASE_H
