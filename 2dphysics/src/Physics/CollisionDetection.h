#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Body.h"

namespace CollisionDetection
{
    bool IsColliding(const Body& a, const Body& b);
    bool IsCollidingCircleCircle(const Body& a, const Body& b);
} // namespace CollissionDetection


#endif // COLLISIONDETECTION_H