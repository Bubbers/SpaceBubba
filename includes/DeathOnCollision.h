
#ifndef BUBBA_3D_DEATHONCOLLISION_H
#define BUBBA_3D_DEATHONCOLLISION_H

#include "GameObject.h"
#include "GameObjectType.h"
#include "IComponent.h"
#include <SpaceBubbaObject.h>

class DeathOnCollision : public IComponent {
public:
    DeathOnCollision(SpaceBubbaObject *parent, GameObjectType relevantType);
    DeathOnCollision(SpaceBubbaObject *parent, GameObjectType relevantType, int pointsWorth, int *scoreBoard);

    void update(float dt);
    void beforeCollision(GameObject* collider);

private:
    GameObjectType relevantType;
    SpaceBubbaObject *parent;
    int pointsWorth = 0;
    int *scoreBoard = nullptr;

};

#endif //BUBBA_3D_DEATHONCOLLISION_H
