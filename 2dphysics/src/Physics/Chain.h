#ifndef CHAIN_H
#define CHAIN_H
#include <vector>
#include <toml++/toml.hpp>
#include "./Body.h"
#include "./Force.h"
#include "./Vec2.h"
#include "../Graphics.h"

using Bodies = std::vector<Body>;

class Chain {
public:
    void Setup(const toml::table& config);
    void Update(float deltaTime);
    void Render();
    void ApplyImpulse(const Vec2& impulseVelocity);
    void ApplyForce(const Vec2& force);
    const Vec2& TailPosition() const;

private:
    Bodies chain;    
    Vec2 anchor;
    float k;
    float restLength;
    void checkBounce();
};

#endif

