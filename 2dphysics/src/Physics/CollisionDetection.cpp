#include "CollisionDetection.h"

namespace CollisionDetection
{
    bool IsColliding(const Body& a, const Body& b)
    {
        const auto& shapeA = a.GetShape().GetType();
        const auto& shapeB = b.GetShape().GetType();

        if(shapeA == ShapeType::CIRCLE && shapeB == ShapeType::CIRCLE)
        {
            return IsCollidingCircleCircle(a, b);
        }

        return false;
    }

    bool IsCollidingCircleCircle(const Body &a, const Body &b)
    {
        const auto& circleShapeA = dynamic_cast<const CircleShape&>(a.GetShape());
        const auto& circleShapeB = dynamic_cast<const CircleShape&>(b.GetShape());

        const Vec2 ab = b.position - a.position;
        const float radiusSum = circleShapeA.radius + circleShapeB.radius;
        return ab.MagnitudeSquared() <= (radiusSum*radiusSum);
    }

    // TODO: implement other methods to check collision between different shapes

} // namespace CollisionDetection
