#include "CollisionDetection.h"

namespace CollisionDetection
{
    bool IsColliding(Body& a, Body& b, Contact& contact)
    {
        const auto& shapeA = a.GetShape().GetType();
        const auto& shapeB = b.GetShape().GetType();

        if(shapeA == ShapeType::CIRCLE && shapeB == ShapeType::CIRCLE)
        {
            return IsCollidingCircleCircle(a, b, contact);
        }

        return false;
    }

    bool IsCollidingCircleCircle(Body &a, Body &b, Contact& contact)
    {
        const auto& circleShapeA = dynamic_cast<const CircleShape&>(a.GetShape());
        const auto& circleShapeB = dynamic_cast<const CircleShape&>(b.GetShape());

        const Vec2 ab = b.position - a.position;
        const float radiusSum = circleShapeA.radius + circleShapeB.radius;
        bool isColliding = ab.MagnitudeSquared() <= (radiusSum*radiusSum);

        if (!isColliding) return isColliding;
        
        contact.a = &a;
        contact.b = &b;

        contact.normal = ab;
        contact.normal.Normalize();

        contact.start = b.position - contact.normal * circleShapeB.radius;
        contact.end = a.position + contact.normal * circleShapeA.radius ;
        contact.depth = (contact.end - contact.start).Magnitude();

        return isColliding;
    }

    // TODO: implement other methods to check collision between different shapes

} // namespace CollisionDetection
