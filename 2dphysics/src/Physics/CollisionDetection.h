#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Body.h"
#include "Contact.h"

namespace CollisionDetection
{
    bool IsColliding(const Body& a, const Body& b, Contact& contact);
    bool IsCollidingCircleCircle(const Body& a, const Body& b, Contact& contact);
} // namespace CollissionDetection


#endif // COLLISIONDETECTION_H