#ifndef BODY_H
#define BODY_H

#include <memory>
#include "Vec2.h"
#include "Shape.h"

class Body {
public:
    bool isColliding {false};
    // Linear motion
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    /**
     * @brief Body Constructor
     * @param shape
     * @param x position
     * @param y position
     * @param mass body mass, pass 0 if you want to simulate an infinite mass object
    */
    Body(const Shape& shape, float x, float y, float mass);
    Body()=delete;
    ~Body();

    void AddForce(const Vec2& force);
    void AddTorque(float torque);

    void Integrate(float dt);
    void Update(float dt);

    float GetRotation() const;
    const Shape& GetShape() const;
    Shape& GetShape();
    float GetMass() const;
    float GetInvMass() const;
    bool IsStatic() const;
private:
    std::unique_ptr<Shape> shape;
    // Angular rotation
    float rotation;
    float angularVelocity;
    float angularAcceleration;

    // Forces and torque
    Vec2 sumForces;
    float sumTorque;

    float mass;
    float invMass;

    float I;
    float invI;
    void ClearForces();
    void ClearTorque();

    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);
};
#endif
