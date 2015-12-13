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
    MoveComponent(GameObject* meshObject, float3 rotationSpeed, float3 rotation, float3 velocity,
                  float3 location, float3 acceleration, float3 scale, float3 scaleSpeed);
    void update(float dt);
    void afterCollision();
    void duringCollision();
    void beforeCollision();
    float3 getVelocity();
    float3 getAcceleration();
    float3 getRotation();
    float3 getLocation();
    float3 getRotationSpeed();
    float3 getScale();
    float3 getScaleSpeed();
    void setVelocity(float3 v);
    void setAcceleration(float3 a);
    void setRotation(float3 r);
    void setLocation(float3 l);
    void setRotationSpeed(float3 rs);
    void setScale(float3 s);
    void setScaleSpeed(float3 ss);
protected:

    float3 velocity = make_vector(0.0f, 0.0f, 0.0f);
    float3 acceleration = make_vector(0.0f, 0.0f, 0.0f);
    float3 rotation = make_vector(0.0f,0.0f,0.0f);
    float3 location = make_vector(0.0f,0.0f,0.0f);
    float3 rotationSpeed = make_vector(0.0f,0.0f,0.0f);
    float3 scale = make_vector(1.0f,1.0f,1.0f);
    float3 scaleSpeed = make_vector(0.0f,0.0f,0.0f);
    float maxSpeed;

    GameObject* meshObject;

    void updateMeshObject(float dt);
};


#endif //BUBBA_3D_KEYBOARDMOVECOMPONENT_H
