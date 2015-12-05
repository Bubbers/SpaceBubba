#ifndef BUBBA_3D_KEYBOARDMOVECOMPONENT_H
#define BUBBA_3D_KEYBOARDMOVECOMPONENT_H

#include "IComponent.h"
#include "float3.h"
#include "Car.h"
#include "GameObject.h"
#include "Collider.h"

using namespace chag;




class CarMoveComponent : public IComponent {
public:
    CarMoveComponent();
    CarMoveComponent(bool keysDown[],bool* hasChangedLocation, Car* car, float* cameraThetaLocation, GameObject* carObject, Collider* collisionHandler);
    void update(float dt);
    void onCollision();
private:
    bool *keysDown;
    bool *hasChangedLocation;
    float* cameraThetaLocation;
    Car* car;
    GameObject* carObject;
    Collider* collisionHandler;

    void checkKeyPresses();
    void updateCarObject();
    void checkIntersection();
};


#endif //BUBBA_3D_KEYBOARDMOVECOMPONENT_H