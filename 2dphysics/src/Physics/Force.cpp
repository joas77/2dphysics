#include <algorithm>
#include "./Force.h"

Vec2 Force::GenerateDragForce(const Particle& particle, float k) {
    Vec2 dragForce = Vec2(0, 0);

    if (particle.velocity.MagnitudeSquared() > 0) {
        // Calculate the drag direction (inverse of velocity unit vector)
        Vec2 dragDirection = particle.velocity.UnitVector() * -1.0;

        // Calculate the drag magnitude, k * |v|^2
        float dragMagnitude = k * particle.velocity.MagnitudeSquared();

        // Generate the final drag force with direction and magnitude
        dragForce = dragDirection * dragMagnitude;
    }

    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Particle& particle, float k) {
    return particle.velocity.UnitVector() * -k;
}

Vec2 Force::GenerateGravitationalForce(
        const Particle& a,
        const Particle& b,
        float G, float minDistance, float maxDistance) {
    // Calculate the distance between the two objects
    Vec2 d = b.position - a.position;

    float distanceSquared = d.MagnitudeSquared();

    // Clamp the values of the distance to allow for some interesting visual effects
    distanceSquared = std::clamp(distanceSquared, minDistance, maxDistance);

    // Calculate the direction of the attraction force
    Vec2 attractionDirection = d.UnitVector();

    // Calculate the strength of the attraction force
    float attractionMagnitude = G * a.mass * b.mass / distanceSquared;

    // Calculate the final resulting attraction force vector
    return attractionDirection * attractionMagnitude;
}
