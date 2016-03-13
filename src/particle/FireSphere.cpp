#include "Utils.h"
#include "FireSphere.h"

float3 FireSphere::initialPosition(float3 genPos) {

    float theta = getRand(0.0f, 360.0f);
    float phi  =  getRand(0.0f, 360.0f);
    float r = 50.0f;
    return make_vector(r * sinf(theta)*sinf(phi),
                       r * cosf(phi),
                       r * cosf(theta)*sinf(phi));
}

float3 FireSphere::initialVelocity() {
    return make_vector(getRand(-.5f, .5f), getRand(0.0f, 1.0f), getRand(-.5f, .5f));
}

float3 FireSphere::accelerate(float3 velocity) {
    return make_vector(.9f * velocity.x, 1.06f * velocity.y, .9f * velocity.z);
}

float FireSphere::calcLifetime() {
    return getRand(0.0f, 2000.0f);
}

float3 FireSphere::calcParticleScale() {
    return make_vector(2.1f, 2.1f, 2.1f);
}

bool FireSphere::loop(float dt) {
    return true;
}