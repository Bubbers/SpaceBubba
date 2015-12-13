
#ifndef BUBBA_3D_SMOKEPARTICLE_H
#define BUBBA_3D_SMOKEPARTICLE_H


#include <ParticleConf.h>

class SmokeParticle : public ParticleConf {
public:
    SmokeParticle() {}
    ~SmokeParticle() {}

    float3 calcPosition(float3 genPos);
    float3 getVelocity();
    float3 accelerate(float3 velocity);
    float3 getScale();
    float getLife();
    bool loop(float dt);
};


#endif //BUBBA_3D_SMOKEPARTICLE_H
