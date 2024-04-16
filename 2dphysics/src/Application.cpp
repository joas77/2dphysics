#include "Application.h"
#include "./Utils.h"
#include "Physics/Constants.h"
#include "Physics/Force.h"
#include "Physics/CollisionDetection.h"

bool Application::IsRunning()
{
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup()
{
    running = Graphics::OpenWindow();

    bodies.push_back(std::make_unique<Body>(CircleShape(100), 100, 100, 1.0));
    bodies.push_back(std::make_unique<Body>(CircleShape(50), 500, 100, 1.0));
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
            // ...
            break;
        default:
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update()
{
    // Check if we are too fast, and if so waste some millisecons
    // until we reach the MILLISECS_PER_FRAME
    static int timePreviousFrame;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0)
        SDL_Delay(timeToWait);

    // Calculate the deltatime in seconds
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016)
        deltaTime = 0.016;

    // Set the time of the current frame to be used in the next one
    timePreviousFrame = SDL_GetTicks();

    for (auto &body : bodies)
    {
        // Apply the weight force
        body->AddForce(Vec2(0.0, body->mass * G_ACCEL * PIXELS_PER_METER));

        // Apply the wind force
        body->AddForce(Vec2(20.0 * PIXELS_PER_METER, 0.0));

        body->Update(deltaTime);
        checkBounce(*body);
    }

    for (size_t i = 0; i < bodies.size() - 1; i++)
    {
        for (size_t j = i + 1; j < bodies.size(); j++)
        {
            auto &a = bodies[i];
            auto &b = bodies[j];
            if (CollisionDetection::IsColliding(*a, *b))
            {
                a->isColliding = true;
                b->isColliding = true;
            }
            else
            {
                a->isColliding = false;
                b->isColliding = false;
            }
                }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render()
{
    Graphics::ClearScreen(0xFF0F0721);

    for (auto &body : bodies)
    {
        Uint32 color = body->isColliding ? 0xFF0000FF : 0xFFFFFFFF;
        // TODO consider using typeid instead enum shapes
        // See: https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-dynamic_cast
        if (body->GetShape().GetType() == ShapeType::CIRCLE)
        {
            const auto &circleShape = dynamic_cast<const CircleShape &>(body->GetShape());
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape.radius, body->GetRotation(), color);
        }
        else if (body->GetShape().GetType() == ShapeType::BOX)
        {
            const auto &boxShape = dynamic_cast<const BoxShape &>(body->GetShape());
            Graphics::DrawPolygon(body->position.x, body->position.y, boxShape.GetVertices(), 0xFFFFFFFF);
        }
        else
        {
            // TODO: Draw other types of shapes
        }
    }

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy()
{
    Graphics::CloseWindow();
}

///////////////////////////////////////////////////////////////////////////////
// Helper methods
///////////////////////////////////////////////////////////////////////////////
void Application::checkBounce(Body &body)
{
    // Nasty hardcoded flip in velocity if it touches the limits of the screen window
    if (body.GetShape().GetType() == ShapeType::CIRCLE)
    {
        auto circleShape = dynamic_cast<CircleShape &>(body.GetShape());
        if (body.position.x - circleShape.radius <= 0)
        {
            body.position.x = circleShape.radius;
            body.velocity.x *= -0.9;
        }
        else if (body.position.x + circleShape.radius >= Graphics::Width())
        {
            body.position.x = Graphics::Width() - circleShape.radius;
            body.velocity.x *= -0.9;
        }

        if (body.position.y - circleShape.radius <= 0)
        {
            body.position.y = circleShape.radius;
            body.velocity.y *= -0.9;
        }
        else if (body.position.y + circleShape.radius >= Graphics::Height())
        {
            body.position.y = Graphics::Height() - circleShape.radius;
            body.velocity.y *= -0.9;
        }
    }
}
