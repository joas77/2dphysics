#include "Application.h"
#include "Physics/Constants.h"
#include "Graphics.h"


bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();
    
    auto smallBall = Particle(50, 100, 1.0);
    smallBall.radius = 4;
    particles.push_back(std::move(smallBall));

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

    for(auto& particle: particles) {
        // Appply "wind" force to my particle
        Vec2 wind = Vec2(0.2 * PIXELS_PER_METER, 0.0);
        particle.AddForce(wind);

        // Apply a "weight" force to my particles
        Vec2 weight = Vec2(0.0, particle.mass * 9.8 * PIXELS_PER_METER);
        particle.AddForce(weight);

        // Apply a "push" force to my particles
        particle.AddForce(pushForce);
        
        // integrate the acceleration and the velocity to find the new position
        particle.Integrate(deltaTime);
    
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

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);
    for(auto& particle: particles) {
        Graphics::DrawFillCircle(particle.position.x, particle.position.y, particle.radius, 0xFFFFFFFF);
    }
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    Graphics::CloseWindow();
}
