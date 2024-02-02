#include "SoftBody.h"
#include "Force.h"
#include "../Graphics.h"
#include <iostream>

void SoftBody::Setup(const toml::table& config){
    // TODO make sure nodes is always > 0
    int nodes = config.at("nodes").value<int>().value();
    restLength = config.at("distance_between_nodes").value<int>().value();
    k = config.at("k").value<float>().value();
    int x = config.at("x").value<int>().value();
    int y = config.at("y").value<int>().value();

    for(int i=1; i<=nodes; i++) {
        auto p = Body(x + i*restLength, y+ i*restLength, 4.0);
        p.radius = 5;
        bodies.push_back(std::move(p));
    }
}

void SoftBody::SetSpringForce(float restLength, float k) {
    this->restLength = restLength;
    this->k = k;
}

void SoftBody::Update(float deltaTime) {

   for(std::size_t i = 0; i < bodies.size(); i++) {
       for(std::size_t j = 0; j < bodies.size(); j++) {
           // Apply a spring force to the bodies
            if(i!=j)
                bodies[i].AddForce(Force::GenerateSpringForce(bodies[i], bodies[j], restLength, k));
       }
   }
    
    for(auto& p: bodies) {
        // Apply weigth
        p.AddForce(Vec2(0.0, p.mass * G_ACCEL * PIXELS_PER_METER));

        p.AddForce(Force::GenerateDragForce(p, 0.05));
    }

    for(auto& p: bodies)
        p.Integrate(deltaTime);

    checkBounce();
}

void SoftBody::Render() {
    for(auto& p: bodies) {
        Graphics::DrawFillCircle(p.position.x, p.position.y, p.radius, 0xFFFFFFFF);
    }
    // draw the spring "rope"
    for(std::size_t i = 0; i < bodies.size(); i++) {
        for(std::size_t j = i+1; j < bodies.size(); j++) {
            Graphics::DrawLine(
                    bodies[i].position.x, bodies[i].position.y,
                    bodies[j].position.x, bodies[j].position.y,
                    0xFF313131);
        }
    }
}

void SoftBody::checkBounce() {

    for(auto& body: bodies) {
        // Nasty hardcoded flip in velocity if it touches the limits of the screen window
        if(body.position.x - body.radius <=0) {
            body.position.x = body.radius;
            body.velocity.x *= -0.9;
        } else if(body.position.x + body.radius >= Graphics::Width()) {
            body.position.x = Graphics::Width() - body.radius;
            body.velocity.x *= -0.9;
        }

        if(body.position.y - body.radius <=0) {
            body.position.y = body.radius;
            body.velocity.y *= -0.9;
        } else if(body.position.y + body.radius >= Graphics::Height()){
            body.position.y = Graphics::Height() - body.radius;
            body.velocity.y *= -0.9;
        }
    }
}

void SoftBody::ApplyForce(const Vec2& force) {
    for(auto& p: bodies)
        p.AddForce(force);
}
