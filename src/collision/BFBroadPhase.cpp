//
// Created by johan on 2015-12-05.
//

#include <GameObject.h>
#include <Collider.h>
#include <timer.h>
#include <Logger.h>
#include <sstream>
#include "BFBroadPhase.h"

BFBroadPhase::BFBroadPhase() {

}

void BFBroadPhase::addGameObject(GameObject *gameObject){
    GameObjectList.push_back(gameObject);
}

void BFBroadPhase::removeDirty(){
	for (auto it = GameObjectList.begin(); it != GameObjectList.end();) {
		if ((*it)->isDirty()) {
			it = GameObjectList.erase(it);
		}
		else {
			it++;
		}
	}
}

void BFBroadPhase::updateCollision() {
    utils::Timer timer;
    timer.start();

    removeDirty();

    CollisionPairList possibleCollision = computeCollisionPairs();
    computeExactCollision(possibleCollision);

    timer.stop();
    stringstream timeMessage;
    timeMessage << "Tested collision in : " << timer.getElapsedTime() << " ms";
    Logger::logDebug(timeMessage.str());
}


CollisionPairList BFBroadPhase::computeCollisionPairs() {
    CollisionPairList collisionPairs;

    for (auto i = GameObjectList.begin(); i != GameObjectList.end(); i++) {
        for (auto j = i + 1; j != GameObjectList.end(); j++) {
            GameObject* gameObject1 = *i;
            GameObject* gameObject2 = *j;

            if(isPossiblyColliding(gameObject1,gameObject2)) {
                collisionPairs.push_back(std::pair<GameObject *, GameObject *>(gameObject1, gameObject2));
            }

        }
    }
    return collisionPairs;
}


bool BFBroadPhase::isPossiblyColliding(GameObject* gameObject1, GameObject* gameObject2) {
    if(!gameObject1->isDynamicObject() && !gameObject2->isDynamicObject()
       || gameObject1->isDirty() || gameObject2->isDirty()) {
        return false;
    }

    AABB aabb1 = gameObject1->getAABB();
    AABB aabb2 = gameObject2->getAABB();

    if(gameObject1 != gameObject2 && AabbAabbintersection(&aabb1, &aabb2)) {
        return true;
    }
}

void BFBroadPhase::computeExactCollision(CollisionPairList possibleCollision) {
    for(auto i = possibleCollision.begin(); i != possibleCollision.end(); i++ ) {
        CollisionPair pair = *i;

        GameObject *object1 = pair.first;
        GameObject *object2 = pair.second;

        float4x4 object1ModelMatrix = object1->getModelMatrix();
        float4x4 object2ModelMatrix = object2->getModelMatrix();

        Octree* object1Oct = object1->getOctree();
        Octree* object2Oct = object2->getOctree();

        bool wasColliding = std::find(collidingList.begin(), collidingList.end(), std::pair<int, int>(object1->getId(), object2->getId())) != collidingList.end() ||
                            std::find(collidingList.begin(), collidingList.end(), std::pair<int, int>(object1->getId(), object2->getId())) != collidingList.end();
        if(octreeOctreeIntersection(object1Oct,&object1ModelMatrix,object2Oct, &object2ModelMatrix)) {
            if(wasColliding) {
                triggerObjectEvent(object1, object2, EventType::DuringCollision);
            } else {
                triggerObjectEvent(object1, object2, EventType::BeforeCollision);

                collidingList.push_back(std::pair<int, int> (object1->getId(), object2->getId()));
                collidingList.push_back(std::pair<int, int> (object2->getId(), object1->getId()));
            }
        } else if (wasColliding) {
            triggerObjectEvent(object1,object2, EventType::AfterCollision);

            collidingList.erase(std::remove(collidingList.begin(), collidingList.end(),
                                            std::pair<int, int>(object1->getId(), object2->getId())));
            collidingList.erase(std::remove(collidingList.begin(), collidingList.end(),
                                            std::pair<int, int>(object2->getId(), object1->getId())));
        }

    }
}

void BFBroadPhase::triggerObjectEvent(GameObject* object1, GameObject* object2, EventType eventType) {
    object1->callEvent(eventType, object1->getType());
    object2->callEvent(eventType, object2->getType());
}

