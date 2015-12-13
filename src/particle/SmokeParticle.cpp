#include "SmokeParticle.h"


float3 SmokeParticle::calcPosition(float3 genPos) {
    float rand = getRand(0.0f, 360.0f);
    float rand2  = getRand(0.0f, 1.0f);
    return make_vector(genPos.x + (float)cos(rand) * rand2,
                       genPos.y + (float)sin(rand) * rand2,
                       genPos.z + (float)sin(rand) * rand2);
}

float3 SmokeParticle::getVelocity() {
    return make_vector(getRand(-.1f, .1f), getRand(-.1f, .1f), getRand(-.1f, .1f));
}

float3 SmokeParticle::accelerate(float3 velocity) {
    return make_vector(velocity.x, velocity.y, velocity.z);
}

float SmokeParticle::getLife() {
    return getRand(0.0f, 2000.0f);
}

