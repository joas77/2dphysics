#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include <memory>
#include "./Graphics.h"
#include "./Physics/Body.h"
#include "./Physics/Constants.h"
#include "./Physics/SoftBody.h"
#include "./Physics/Chain.h"

class Application {
    private:
        bool running = false;
        Vec2 pushForce = Vec2(0, 0);
        Vec2 mouseCursor = Vec2(0,0);
        bool leftMouseButtonDown = false;

        std::vector<std::unique_ptr<Body>> bodies;

        void checkBounce(Body& body);

    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};

#endif
