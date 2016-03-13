//
// Created by johan on 2015-12-13.
//

#ifndef BUBBA_3D_FIRESPHERE_H
#define BUBBA_3D_FIRESPHERE_H

#include "FireParticle.h"

class FireSphere : public FireParticle {

    float3 initialPosition(float3 genPos);
    float3 initialVelocity();
    float3 accelerate(float3 velocity);
    float calcLifetime();
    float3 calcParticleScale();
    bool loop(float dt);

};

#endif //BUBBA_3D_FIRESPHERE_H
