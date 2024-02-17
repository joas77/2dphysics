#include "Application.h"
#include "./Utils.h"
#include "Physics/Constants.h"
#include "./Physics/Force.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    auto body = std::make_unique<Body>(std::move(std::make_unique<CircleShape>(50)), 300, 100, 1.0);
    bodies.push_back(std::move(body));
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_UP)
                   pushForce.y = -50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_RIGHT)
                   pushForce.x = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_DOWN)
                   pushForce.y = 50 * PIXELS_PER_METER;
                if (event.key.keysym.sym == SDLK_LEFT)
                   pushForce.x = -50 * PIXELS_PER_METER;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                   pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_RIGHT)
                   pushForce.x = 0;
                if (event.key.keysym.sym == SDLK_DOWN)
                   pushForce.y = 0;
                if (event.key.keysym.sym == SDLK_LEFT)
                   pushForce.x = 0;
                break;
            case SDL_MOUSEMOTION:
                mouseCursor.x = event.motion.x;
                mouseCursor.y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true;
                    mouseCursor.x = event.button.x;
                    mouseCursor.y = event.button.y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false;
                    auto impulseDirection = (bodies[0]->position - mouseCursor).UnitVector();
                    auto impulseMagnitude = (bodies[0]->position - mouseCursor).Magnitude() * 5.0;

                }
                break;
            default:
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    // Check if we are too fast, and if so waste some millisecons
    // until we reach the MILLISECS_PER_FRAME
    static int timePreviousFrame;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if(timeToWait > 0 ) SDL_Delay(timeToWait);

    // Calculate the deltatime in seconds
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if(deltaTime > 0.016)
        deltaTime = 0.016;

    // Set the time of the current frame to be used in the next one
    timePreviousFrame = SDL_GetTicks();

    for(auto& body : bodies ) {
        body->AddForce(pushForce);
        body->AddForce(Vec2(0.0, body->mass * G_ACCEL * PIXELS_PER_METER));
        body->AddForce(Force::GenerateDragForce(*body, 0.001));

        float torque = 100;
        body->AddTorque(torque);

        body->Integrate(deltaTime);

        checkBounce(*body);
    }

}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF0F0721);
    
    for(auto& body : bodies ) {
        if(body->GetShape().GetType() == ShapeType::CIRCLE) {
            auto circleShape = dynamic_cast<CircleShape&>(body->GetShape());
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape.radius, body->GetRotation(), 0xFF11FF11);
        } else {
            // TODO: Draw other types of shapes
        }
    }

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    Graphics::CloseWindow();
}


///////////////////////////////////////////////////////////////////////////////
// Helper methods
///////////////////////////////////////////////////////////////////////////////
void Application::checkBounce(Body& body) {
    // Nasty hardcoded flip in velocity if it touches the limits of the screen window
    if(body.GetShape().GetType() == ShapeType::CIRCLE) {
        auto circleShape = dynamic_cast<CircleShape&>(body.GetShape());
        if(body.position.x - circleShape.radius <=0) {
            body.position.x = circleShape.radius;
            body.velocity.x *= -0.9;
        } else if(body.position.x + circleShape.radius >= Graphics::Width()) {
            body.position.x = Graphics::Width() - circleShape.radius;
            body.velocity.x *= -0.9;
        }

        if(body.position.y - circleShape.radius <=0) {
            body.position.y = circleShape.radius;
            body.velocity.y *= -0.9;
        } else if(body.position.y + circleShape.radius >= Graphics::Height()){
            body.position.y = Graphics::Height() - circleShape.radius;
            body.velocity.y *= -0.9;
        }
    }
}
