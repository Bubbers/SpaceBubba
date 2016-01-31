//
// Created by simon on 2016-01-30.
//

#ifndef SUPER_BUBBA_AWESOME_SPACE_SPACEBUBBAOBJECT_H
#define SUPER_BUBBA_AWESOME_SPACE_SPACEBUBBAOBJECT_H

#include <GameObject.h>
#include <GameObjectType.h>

class SpaceBubbaObject : public GameObject{
public:
    SpaceBubbaObject(Mesh* model, Mesh* collisionMesh, GameObjectType type);
    SpaceBubbaObject(Mesh* model, GameObjectType type);
    SpaceBubbaObject();

    virtual ~SpaceBubbaObject(){}

    void setType(GameObjectType type);

};


#endif //SUPER_BUBBA_AWESOME_SPACE_SPACEBUBBAOBJECT_H
