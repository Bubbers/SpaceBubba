
#ifndef BUBBA_3D_DEATHONCOLLISION_H
#define BUBBA_3D_DEATHONCOLLISION_H

#include "GameObject.h"
#include "GameObjectType.h"
#include "IComponent.h"

class DeathOnCollision : public IComponent {
public:
    DeathOnCollision(GameObject *parent, GameObjectType relevantType);
    DeathOnCollision(GameObject *parent, GameObjectType relevantType, int pointsWorth, int *scoreBoard);

    void update(float dt);
    void beforeCollision(GameObjectType collider);

private:
    GameObjectType relevantType;
    GameObject *parent;
    int pointsWorth = 0;
    int *scoreBoard = nullptr;

};

#endif //BUBBA_3D_DEATHONCOLLISION_H
