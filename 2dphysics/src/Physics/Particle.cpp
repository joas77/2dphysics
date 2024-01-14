#include "Particle.h"
#include <iostream>

Particle::Particle(float x, float y, float mass):
    position{Vec2(x, y)}, mass{mass}
{
    std::cout << "Particle constructor called!" <<std::endl;
}

Particle::~Particle() {
    std::cout << "Particle destructor called!" << std::endl;
}
