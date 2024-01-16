#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

struct Particle {
    int radius;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float mass;

    Particle(float x, float y, float mss);
    ~Particle();

    void Integrate(float dt);
};
#endif
