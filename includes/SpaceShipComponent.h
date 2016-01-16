//
// Created by simon on 2015-12-12.
//

#ifndef BUBBA_3D_SPACESHIPCOMPONENT_H
#define BUBBA_3D_SPACESHIPCOMPONENT_H


#include "ParticleGenerator.h"

class SpaceShipComponent : public MoveComponent{
public:

    SpaceShipComponent(struct HudRenderer::HudConfig* hudConf, float* cameraThetaLocation, float* cameraPhiLocation,
                       GameObject* ship, ParticleGenerator* generator1, ParticleGenerator* generator2, State* state);

    void update(float dt) ;
    float3 getFrontDir();
	float3 getUpDir();
    void onDeath();

private:
    struct HudRenderer::HudConfig* hudConf;
    State* state;
    float* cameraThetaLocation, *cameraPhiLocation;
    void checkKeyPresses(float dt);
    float4x4 updateRotation(float dt);

    float turnSpeed = (float) (2 * M_PI / (180*20));
    float accelerationSpeed = 0.0f;
    float3 frontDir = make_vector(0.0f, 0.0f, 1.0f);
	float3 upDir = make_vector(0.0f,1.0f,0.0f);
    float maxSpeed;
    ParticleGenerator *generator1;
    ParticleGenerator *generator2;
    float totalTurn = 0.0f;
    float totalIncl = 0.0f;

	const float4 originalUpVector = make_vector(0.0f, 1.0f, 0.0f, 0.0f);
};


#endif //BUBBA_3D_SPACESHIPCOMPONENT_H
