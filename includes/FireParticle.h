#ifndef BUBBA_3D_FIREPARTICLE_H
#define BUBBA_3D_FIREPARTICLE_H

#include "ParticleConf.h"

using namespace chag;

class FireParticle : public ParticleConf {
public:
    FireParticle() {}
    ~FireParticle() {}

    float3 initialPosition(float3 genPos);
    float3 initialVelocity();
    float3 accelerate(float3 velocity);
    float calcLifetime();
    float3 calcParticleScale();
    bool loop(float dt);

    GLuint blendFunc = GL_ONE;

    void setLooping(bool value);
private:
};


#endif //BUBBA_3D_FIREPARTICLE_H
