#include <iostream>
#include <cmath>
#include "Body.h"

Body::Body(const Shape &shape, float x, float y, float mass) : position{Vec2(x, y)},
                                                               velocity{Vec2(0, 0)},
                                                               acceleration{Vec2(0, 0)},
                                                               restitution{1.0},
                                                               shape{shape.Clone()},
                                                               rotation{0}, angularVelocity{0}, angularAcceleration{0},
                                                               sumForces{Vec2(0, 0)}, sumTorque{0}, mass{mass}
{
    if (mass != 0.0)
        invMass = 1.0 / mass;
    else
        invMass = 0.0;

    I = shape.GetMomentOfInertia() * mass;
    if (I != 0.0)
        invI = 1.0 / I;
    else
        invI = 0.0;

    std::cout << "Body constructor called!" << std::endl;
}

Body::~Body()
{
    std::cout << "Body destructor called!" << std::endl;
}

void Body::AddForce(const Vec2 &force)
{
    sumForces += force;
}

void Body::AddTorque(float torque)
{
    sumTorque += torque;
}

void Body::ClearForces()
{
    sumForces = Vec2(0.0, 0.0);
}

void Body::ClearTorque()
{
    sumTorque = 0.0;
}

void Body::Integrate(float dt)
{
    IntegrateLinear(dt);
    IntegrateAngular(dt);
}

void Body::Update(float dt)
{
    Integrate(dt);
    const bool isPolygon = GetShape().GetType() == ShapeType::POLYGON || GetShape().GetType() == ShapeType::BOX;
    if (isPolygon)
    {
        auto &polygonShape = dynamic_cast<PolygonShape &>(GetShape());
        polygonShape.UpdateVertices(GetRotation(), position);
    }
}

void Body::ApplyImpulse(const Vec2 &j)
{
    if (IsStatic())
        return;

    velocity += j * invMass;
}

void Body::IntegrateLinear(float dt)
{
    if (IsStatic())
        return;
    // Find the acceleration based on the forces that are being applied and the mass
    acceleration = sumForces * invMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;

    // Integrate the velocity to find the acceleration
    position += velocity * dt;

    ClearForces();
}

void Body::IntegrateAngular(float dt)
{
    if (IsStatic())
        return;
    // Find the angular acceleration based on the torque that is being applied and the moment of inertia
    angularAcceleration = sumTorque * invI;

    // Integrate the angular acceleration to find the new rotation angle
    angularVelocity += angularAcceleration * dt;

    // Integrate the angular velocity to find the rotation angle
    rotation += angularVelocity * dt;

    // Clear all the torque acting on the object before the next physics step
    ClearTorque();
}

float Body::GetRotation() const
{
    return rotation;
}

const Shape &Body::GetShape() const
{
    return *shape;
}

Shape &Body::GetShape()
{
    return *shape;
}

float Body::GetInvMass() const
{
    return invMass;
}

float Body::GetMass() const
{
    return mass;
}

bool Body::IsStatic() const
{
    static constexpr float epsilon = 0.0001f;
    return std::fabs(invMass - 0.0) < epsilon;
}
