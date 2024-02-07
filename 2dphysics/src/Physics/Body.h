#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
#include "Shape.h"

// FIXME: shape shall be a pointer in order to be polymorphic
// but should be a better maybe pass only the enum
class Body {
public:
    // Linear motion
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    float mass;
    Shape* shape;

    Body(Shape* shape, float x, float y, float mass);
    Body()=delete;
    ~Body();

    void AddForce(const Vec2& force);
    void AddTorque(float torque);

    void Integrate(float dt);

    float GetRotation() const;
private:
    // Angular rotation
    float rotation;
    float angularVelocity;
    float angularAcceleration;

    // Forces and torque
    Vec2 sumForces;
    float sumTorque;

    float invMass;

    float I;
    float invI;
    void ClearForces();
    void ClearTorque();

    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);
};
#endif
