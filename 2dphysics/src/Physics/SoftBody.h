#ifndef SOFT_BODY_H
#define SOFT_BODY_H

#include <memory>
#include <vector>
#include <toml++/toml.hpp>
#include "./Particle.h"
#include "./Constants.h"

using Particles = std::vector<Particle>;

class SoftBody{
public:
    void SetSpringForce(float restLength, float k); 
    void Setup(const toml::table& config);
    void Update(float deltaTime);
    void Render();
    void ApplyForce(const Vec2& force);

private:
    float k=0;
    float restLength=400;
    Particles particles;

    void checkBounce();
};

#endif
