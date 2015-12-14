//
// Created by simon on 2015-12-12.
//

#ifndef BUBBA_3D_SPACESHIPCOMPONENT_H
#define BUBBA_3D_SPACESHIPCOMPONENT_H


#include "ParticleGenerator.h"

class SpaceShipComponent : public MoveComponent{
public:

    SpaceShipComponent(struct HudRenderer::HudConfig* hudConf, float* cameraThetaLocation, GameObject* ship, ParticleGenerator* generator1, ParticleGenerator* generator2, State* state);

    void update(float dt) ;
    float3 getFrontDir();
    float getTotalRotation();
    void onDeath();

private:
    struct HudRenderer::HudConfig* hudConf;
    State* state;
    float* cameraThetaLocation;
    void checkKeyPresses(float dt);

    float turnSpeed = (float) (2 * M_PI / (180*20));
    float accelerationSpeed = 0.0f;
    float3 frontDir = make_vector(0.0f, 0.0f, 1.0f);
    float totRotation = 0;
    ParticleGenerator *generator1;
    ParticleGenerator *generator2;

};


#endif //BUBBA_3D_SPACESHIPCOMPONENT_H
