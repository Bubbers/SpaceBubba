#ifndef BUBBA_3D_KEYBOARDMOVECOMPONENT_H
#define BUBBA_3D_KEYBOARDMOVECOMPONENT_H

#include "IComponent.h"
#include "float3.h"
#include "GameObject.h"
#include "HudRenderer.h"

using namespace chag;




class MoveComponent : public IComponent {
public:
    MoveComponent();
    MoveComponent(GameObject* meshObject);
    MoveComponent(GameObject* meshObject, float3 rotation, float3 velocity, float3 acceleration);
    void update(float dt);
    void afterCollision();
    void duringCollision();
    void beforeCollision();
protected:

    float3 velocity = make_vector(0.0f, 0.0f, 0.0f);
    float3 acceleration = make_vector(0.0f, 0.0f, 0.0f);
    float3 rotation = make_vector(0.0f,0.0f,0.0f);
    float maxSpeed;

    GameObject* meshObject;

    void updateMeshObject(float dt);
};


#endif //BUBBA_3D_KEYBOARDMOVECOMPONENT_H
