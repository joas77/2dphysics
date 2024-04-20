#include "Contact.h"

void Contact::ResolvePenetration()
{
    if (a->IsStatic() && b->IsStatic())
        return;

    auto da = depth / (a->GetInvMass() + b->GetInvMass()) * a->GetInvMass();
    auto db = depth / (a->GetInvMass() + b->GetInvMass()) * b->GetInvMass();

    a->position -= normal * da;
    b->position += normal * db;
}

void Contact::ResolveCollision()
{
    // Apply positional correction using the projection method
    ResolvePenetration();

    // Define elasticity (coefficient of restitution e)
    const float e = std::min(a->restitution, b->restitution);

    // Calculate the relative velocity between the two objects
    const auto vrel = (a->velocity - b->velocity);

    // Calculate the relative velocity along the normal collision vector
    const auto velDotNormal = vrel.Dot(normal);

    // Calculate collision impuls
    const Vec2 impulseDirection = normal;
    const float impulseMagnitude = -(1 + e) * velDotNormal / (a->GetInvMass() + b->GetInvMass());

    Vec2 jn = impulseDirection * impulseMagnitude;

    // Apply the impulse vector to both obejects in opposite direction
    a->ApplyImpulse(jn);
    b->ApplyImpulse(-jn);
}