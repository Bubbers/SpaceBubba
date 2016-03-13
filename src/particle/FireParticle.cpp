#include "Utils.h"
#include "FireParticle.h"

float3 FireParticle::initialPosition() {
    float rand = getRand(0.0f, 360.0f);
    float rand2  = getRand(0.0f, 0.3f);
	return make_vector((float)cos(rand) * rand2, (float)sin(rand) * rand2, 0.0f);
}

float3 FireParticle::initialVelocity() {
    return make_vector(getRand(-.5f, .5f), getRand(0.0f, 1.0f), getRand(-.5f, .5f));
}

float3 FireParticle::accelerate(float3 velocity) {
    return make_vector(.9f * velocity.x, 1.06f * velocity.y, .9f * velocity.z);
}

float FireParticle::calcLifetime() {
    return getRand(0.0f, 500.0f);
}

float3 FireParticle::calcParticleScale() {
    return make_vector(.1f, .1f, .1f);
}

bool FireParticle::loop(float dt) {
    return looping;
}

void FireParticle::setLooping(bool value){
    looping = value;
}