//
// Created by johan on 2015-12-13.
//

#ifndef BUBBA_3D_FIRESPHERE_H
#define BUBBA_3D_FIRESPHERE_H

#include "FireParticle.h"

class FireSphere : public FireParticle {

    float3 calcPosition(float3 genPos);
    float3 getVelocity();
    float3 accelerate(float3 velocity);
    float getLife();
    float3 getScale();
    bool loop(float dt);

};

#endif //BUBBA_3D_FIRESPHERE_H
