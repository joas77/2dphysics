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
        auto p = Particle(x + i*restLength, y+ i*restLength, 4.0);
        p.radius = 5;
        particles.push_back(std::move(p));
    }
}

void SoftBody::SetSpringForce(float restLength, float k) {
    this->restLength = restLength;
    this->k = k;
}

void SoftBody::Update(float deltaTime) {

   for(std::size_t i = 0; i < particles.size(); i++) {
       for(std::size_t j = 0; j < particles.size(); j++) {
           // Apply a spring force to the particles
            if(i!=j)
                particles[i].AddForce(Force::GenerateSpringForce(particles[i], particles[j], restLength, k));
       }
   }
    
    for(auto& p: particles) {
        // Apply weigth
        p.AddForce(Vec2(0.0, p.mass * G_ACCEL * PIXELS_PER_METER));

        p.AddForce(Force::GenerateDragForce(p, 0.05));
    }

    for(auto& p: particles)
        p.Integrate(deltaTime);

    checkBounce();
}

void SoftBody::Render() {
    for(auto& p: particles) {
        Graphics::DrawFillCircle(p.position.x, p.position.y, p.radius, 0xFFFFFFFF);
    }
    // draw the spring "rope"
    for(std::size_t i = 0; i < particles.size(); i++) {
        for(std::size_t j = i+1; j < particles.size(); j++) {
            Graphics::DrawLine(
                    particles[i].position.x, particles[i].position.y,
                    particles[j].position.x, particles[j].position.y,
                    0xFF313131);
        }
    }
}

void SoftBody::checkBounce() {

    for(auto& particle: particles) {
        // Nasty hardcoded flip in velocity if it touches the limits of the screen window
        if(particle.position.x - particle.radius <=0) {
            particle.position.x = particle.radius;
            particle.velocity.x *= -0.9;
        } else if(particle.position.x + particle.radius >= Graphics::Width()) {
            particle.position.x = Graphics::Width() - particle.radius;
            particle.velocity.x *= -0.9;
        }

        if(particle.position.y - particle.radius <=0) {
            particle.position.y = particle.radius;
            particle.velocity.y *= -0.9;
        } else if(particle.position.y + particle.radius >= Graphics::Height()){
            particle.position.y = Graphics::Height() - particle.radius;
            particle.velocity.y *= -0.9;
        }
    }
}

void SoftBody::ApplyForce(const Vec2& force) {
    for(auto& p: particles)
        p.AddForce(force);
}
