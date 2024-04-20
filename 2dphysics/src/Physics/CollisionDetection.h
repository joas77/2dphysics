#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Body.h"
#include "Contact.h"

namespace CollisionDetection
{
    bool IsColliding(Body& a, Body& b, Contact& contact);
    bool IsCollidingCircleCircle(Body& a, Body& b, Contact& contact);
} // namespace CollissionDetection


#endif // COLLISIONDETECTION_H