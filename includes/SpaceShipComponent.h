//
// Created by simon on 2015-12-12.
//

#ifndef BUBBA_3D_SPACESHIPCOMPONENT_H
#define BUBBA_3D_SPACESHIPCOMPONENT_H


#include "ParticleGenerator.h"

class SpaceShipComponent : public MoveComponent{
public:

    SpaceShipComponent(float* speed, float* cameraThetaLocation, float* cameraPhiLocation,
                       GameObject* ship, ParticleGenerator* generator1, ParticleGenerator* generator2, State* state);

    void update(float dt) ;
    float3 getFrontDir();
	float3 getUpDir();
    void onDeath();
	const float maxSpeed = 0.5f;

private:
    State* state;
    float* cameraThetaLocation, *cameraPhiLocation;
    void checkKeyPresses(float dt);
    void updateRot();

	const float3 originalUpVector = make_vector(0.0f, 1.0f, 0.0f);
	const float3 originalFrontVector = make_vector(0.0f,0.0f,1.0f);
	const float3 originalRightVector = make_vector(1.0f,0.0f,0.0f);

    float turnSpeed = (float) (2 * M_PI / (180*20));
    float accelerationSpeed = 0.0f;
    float3 frontDir = originalFrontVector;
	float3 upDir = originalUpVector;
	float3 rightDir = originalRightVector;
    ParticleGenerator *generator1;
    ParticleGenerator *generator2;
    float totalTurn = 0.0f;
    float totalIncl = 0.0f;
	float* speed;
};


#endif //BUBBA_3D_SPACESHIPCOMPONENT_H
