
#ifndef BUBBA_3D_SMOKEPARTICLE_H
#define BUBBA_3D_SMOKEPARTICLE_H


#include <ParticleConf.h>

class SmokeParticle : public ParticleConf {

    SmokeParticle() {}
    ~SmokeParticle() {}

    float3 calcPosition(float3 genPos);
    float3 getVelocity();
    float3 accelerate(float3 velocity);
    float getLife();
};


#endif //BUBBA_3D_SMOKEPARTICLE_H
