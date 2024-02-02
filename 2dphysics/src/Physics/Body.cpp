#include "Body.h"
#include <iostream>

Body::Body(float x, float y, float mass):
    radius(4*mass), position{Vec2(x, y)}, mass{mass}
{
    if(mass != 0.0)
        invMass = 1.0 / mass;
    else
        invMass = 0.0;

    std::cout << "Body constructor called!" <<std::endl;
}

Body::~Body() {
    std::cout << "Body destructor called!" << std::endl;
}

void Body::AddForce(const Vec2& force) {
    sumForces += force;
}

void Body::ClearForces() {
    sumForces = Vec2(0.0, 0.0);
}

void Body::Integrate(float dt) {
    // Find the acceleration based on the forces that are being applied and the mass
    acceleration = sumForces * invMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;

    // Integrate the velocity to find the acceleration
    position += velocity * dt;

    ClearForces();
}

