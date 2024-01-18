#include "Particle.h"
#include <iostream>

Particle::Particle(float x, float y, float mass):
    position{Vec2(x, y)}, mass{mass}
{
    if(mass != 0.0)
        invMass = 1.0 / mass;
    else
        invMass = 0.0;

    std::cout << "Particle constructor called!" <<std::endl;
}

Particle::~Particle() {
    std::cout << "Particle destructor called!" << std::endl;
}

void Particle::AddForce(const Vec2& force) {
    sumForces += force;
}

void Particle::ClearForces() {
    sumForces = Vec2(0.0, 0.0);
}

void Particle::Integrate(float dt) {
    // Find the acceleration based on the forces that are being applied and the mass
    acceleration = sumForces * invMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;

    // Integrate the velocity to find the acceleration
    position += velocity * dt;

    ClearForces();
}

