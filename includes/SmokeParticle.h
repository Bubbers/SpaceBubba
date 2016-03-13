
#ifndef BUBBA_3D_SMOKEPARTICLE_H
#define BUBBA_3D_SMOKEPARTICLE_H


#include <ParticleConf.h>

class SmokeParticle : public ParticleConf {
public:
    SmokeParticle() {}
    ~SmokeParticle() {}

    float3 initialPosition();
    float3 initialVelocity();

    float3 accelerate(float3 velocity);
    float3 calcParticleScale();
    float calcLifetime();
    bool loop(float dt);
};


#endif //BUBBA_3D_SMOKEPARTICLE_H
