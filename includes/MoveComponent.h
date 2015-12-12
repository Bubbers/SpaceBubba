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
    MoveComponent(GameObject* meshObject, float3 location, float3 velocity, float3 acceleration);
    void update(float dt);
    void afterCollision();
    void duringCollision();
    void beforeCollision();
protected:

    float3 frontDir = make_vector(0.0f, 0.0f, 1.0f);
    float3 upDir = make_vector(0.0f, 1.0f, 0.0f);
    float3 velocity = make_vector(0.0f, 0.0f, 0.0f);
    float3 acceleration = make_vector(0.0f, 0.0f, 0.0f);
    float3 location = make_vector(0.0f, 0.0f, 0.f);
    float maxSpeed;

    float rotationSpeed = (float) (2 * M_PI / 180);

    GameObject* meshObject;
    float angley = 0;
    float anglez = 0;
    float anglex = 0;
    float lengthx = 2;
    float lengthz = 3;

    void updateMeshObject(float dt);
};


#endif //BUBBA_3D_KEYBOARDMOVECOMPONENT_H
