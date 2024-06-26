#include <algorithm>
#include "./Force.h"

Vec2 Force::GenerateDragForce(const Body& body, float k) {
    Vec2 dragForce = Vec2(0, 0);

    if (body.velocity.MagnitudeSquared() > 0) {
        // Calculate the drag direction (inverse of velocity unit vector)
        Vec2 dragDirection = body.velocity.UnitVector() * -1.0;

        // Calculate the drag magnitude, k * |v|^2
        float dragMagnitude = k * body.velocity.MagnitudeSquared();

        // Generate the final drag force with direction and magnitude
        dragForce = dragDirection * dragMagnitude;
    }

    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Body& body, float k) {
    return body.velocity.UnitVector() * -k;
}

Vec2 Force::GenerateGravitationalForce(
        const Body& a,
        const Body& b,
        float G, float minDistance, float maxDistance) {
    // Calculate the distance between the two objects
    Vec2 d = b.position - a.position;

    float distanceSquared = d.MagnitudeSquared();

    // Clamp the values of the distance to allow for some interesting visual effects
    distanceSquared = std::clamp(distanceSquared, minDistance, maxDistance);

    // Calculate the direction of the attraction force
    Vec2 attractionDirection = d.UnitVector();

    // Calculate the strength of the attraction force
    float attractionMagnitude = G  / (a.GetInvMass() * b.GetInvMass() * distanceSquared);

    // Calculate the final resulting attraction force vector
    return attractionDirection * attractionMagnitude;
}

Vec2 Force::GenerateSpringForce(const Body& body, const Vec2& anchor, float restLength, float k) {
    // Calculate the distance between the anchor and the object
    auto d = body.position - anchor;

    // Find the spring displacement considering the rest length
    auto displacement = d.Magnitude() - restLength;

    // Calculate the direction and the magnitude of the spring force
    return  d.UnitVector() * ( -k * displacement );
}

Vec2 Force::GenerateSpringForce(const Body& a, const Body& b, float restLength, float k) {
    // Calculate the distance between the anchor and the object
    auto d = a.position - b.position;

    // Find the spring displacement considering the rest length
    auto displacement = d.Magnitude() - restLength;

    // Calculate the direction and the magnitude of the spring force
    return  d.UnitVector() * ( -k * displacement );
}
