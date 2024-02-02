#include "./Chain.h"
#include "./Constants.h"

void Chain::Setup(const toml::table& config) {
    int nodes = config.at("nodes").value<int>().value();
    restLength = config.at("distance_between_nodes").value<int>().value();
    k = config.at("k").value<float>().value();
    int x = config.at("x").value<int>().value();
    int y = config.at("y").value<int>().value();

    anchor = Vec2(x, y);

    for(int i=1; i<=nodes; i++) {
        auto p = Body(x, y+ i*restLength, 4.0);
        p.radius = 5;
        chain.push_back(std::move(p));
    }
}

void Chain::Update(float deltaTime) {
    for(auto& body: chain ) {
        // Apply weight force
        body.AddForce(Vec2(0.0, body.mass * G_ACCEL * PIXELS_PER_METER));
        
        // Apply a drag force to my bodies
        body.AddForce(Force::GenerateDragForce(body, 0.001));
    }

    // Apply springforce
    // Apply a spring force to the body connected to the anchor
    chain[0].AddForce(Force::GenerateSpringForce(chain[0], anchor, restLength, k));
    for(auto it = chain.rbegin(); it!= chain.rend() -1; ++it) {
        auto current = it;
        auto prev = it+1;

        auto springForce = Force::GenerateSpringForce(*current, *prev, restLength, k);
        current->AddForce(springForce);
        prev->AddForce(-springForce);
    }


    for(auto& body: chain )
        body.Integrate(deltaTime);


    checkBounce();
}

void Chain::Render() {
    // Draw the spring line
    auto lineColor = 0xFF313131u;
    Graphics::DrawLine(anchor.x, anchor.y, chain[0].position.x, chain[0].position.y, lineColor);

    for(auto it = chain.begin(); it!= chain.end() -1; ++it) {
        auto current = it;
        auto next = it+1;
        Graphics::DrawLine(current->position.x, current->position.y, next->position.x, next->position.y, lineColor);
    }

    Graphics::DrawFillCircle(anchor.x, anchor.y, 4, 0xFF1111FF);

    for(auto& body: chain) {
        Graphics::DrawFillCircle(body.position.x, body.position.y, body.radius, 0xFFFFFFFF);
    }
}

void Chain::checkBounce() {
    for(auto& p: chain) {
        // Nasty hardcoded flip in velocity if it touches the limits of the screen window
        if(p.position.x - p.radius <=0) {
            p.position.x = p.radius;
            p.velocity.x *= -0.9;
        } else if(p.position.x + p.radius >= Graphics::Width()) {
            p.position.x = Graphics::Width() - p.radius;
            p.velocity.x *= -0.9;
        }

        if(p.position.y - p.radius <=0) {
            p.position.y = p.radius;
            p.velocity.y *= -0.9;
        } else if(p.position.y + p.radius >= Graphics::Height()){
            p.position.y = Graphics::Height() - p.radius;
            p.velocity.y *= -0.9;
        }
    } 
}

void Chain::ApplyImpulse(const Vec2& impulseVelocity) {
    chain.back().velocity = impulseVelocity;
}

const Vec2& Chain::TailPosition() const {
    return chain.back().position;
}

void Chain::ApplyForce(const Vec2& force) {
    chain.back().AddForce(force);
}