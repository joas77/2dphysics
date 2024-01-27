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

    anchor = Vec2(Graphics::Width() / 2, Graphics::Height()/2);//30);

    auto bob = std::make_unique<Particle>(Graphics::Width() / 2, Graphics::Height() / 2, 2.0);
    bob->radius = 10;
    particles.push_back(std::move(bob));
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
                    auto impulseDirection = (particles[0]->position - mouseCursor).UnitVector();
                    auto impulseMagnitude = (particles[0]->position - mouseCursor).Magnitude() * 5.0;
                    particles[0]->velocity = impulseDirection * impulseMagnitude;
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

    // Apply forces to the particles
    for(auto& particle: particles) {
        // Apply a "push" force to my particles
        particle->AddForce(pushForce);

        // Apply a drag force to my particles
        particle->AddForce(Force::GenerateDragForce(*particle, 0.001));

        // Apply weight force
        particle->AddForce(Vec2(0.0, particle->mass * 9.8 * PIXELS_PER_METER));
    }

    // Apply a spring force to the particle connected to the anchor
    particles[0]->AddForce(Force::GenerateSpringForce(*particles[0], anchor, restLength, k));

    for(auto& particle: particles) {
        // integrate the acceleration and the velocity to find the new position
        particle->Integrate(deltaTime);

        // Nasty hardcoded flip in velocity if it touches the limits of the screen window
        if(particle->position.x - particle->radius <=0) {
            particle->position.x = particle->radius;
            particle->velocity.x *= -0.9;
        } else if(particle->position.x + particle->radius >= Graphics::Width()) {
            particle->position.x = Graphics::Width() - particle->radius;
            particle->velocity.x *= -0.9;
        }

        if(particle->position.y - particle->radius <=0) {
            particle->position.y = particle->radius;
            particle->velocity.y *= -0.9;
        } else if(particle->position.y + particle->radius >= Graphics::Height()){
            particle->position.y = Graphics::Height() - particle->radius;
            particle->velocity.y *= -0.9;
        }
    }

}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF0F0721);

    if(leftMouseButtonDown) {
        Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, mouseCursor.x, mouseCursor.y, 0xFF0000FF);
    }
    // Draw the spring
    Graphics::DrawLine(anchor.x, anchor.y, particles[0]->position.x, particles[0]->position.y, 0xFF313131);

    // Draw the anchor
    Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155);

    // Draw the bob
    Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFFFFFFF);

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    Graphics::CloseWindow();
}
