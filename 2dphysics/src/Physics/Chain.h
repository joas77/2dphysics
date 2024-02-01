#ifndef CHAIN_H
#define CHAIN_H
#include <vector>
#include <toml++/toml.hpp>
#include "./Particle.h"
#include "./Force.h"
#include "./Vec2.h"
#include "../Graphics.h"

using Particles = std::vector<Particle>;

class Chain {
public:
    void Setup(const toml::table& config);
    void Update(float deltaTime);
    void Render();
    void ApplyImpulse(const Vec2& impulseVelocity);
    void ApplyForce(const Vec2& force);
    const Vec2& TailPosition() const;

private:
    Particles chain;    
    Vec2 anchor;
    float k;
    float restLength;
    void checkBounce();
};

#endif

