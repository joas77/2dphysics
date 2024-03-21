#include <iostream>
#include <algorithm>
#include "Shape.h"

CircleShape::CircleShape(float radius) : radius{radius}
{
    std::cout << "CicleShape constructor called!" << std::endl;
}

CircleShape::~CircleShape()
{
    std::cout << "CicleShape destructor called!" << std::endl;
}

std::unique_ptr<Shape> CircleShape::Clone() const
{
    return std::make_unique<CircleShape>(radius);
}

ShapeType CircleShape::GetType() const
{
    return ShapeType::CIRCLE;
}

float CircleShape::GetMomentOfInertia() const
{
    // this still needs to be multiplied by the rigidbody's mass
    return 0.5 * radius * radius;
}

PolygonShape::PolygonShape(const std::vector<Vec2> &vertices)
    : localVertices{vertices}, worldVertices{vertices}
{
}

std::unique_ptr<Shape> PolygonShape::Clone() const
{
    return std::make_unique<PolygonShape>(localVertices);
}

float PolygonShape::GetMomentOfInertia() const
{
    // TODO...
    return 0.0;
}

ShapeType PolygonShape::GetType() const
{
    return ShapeType::POLYGON;
}

const std::vector<Vec2> &PolygonShape::GetVertices() const
{
    return worldVertices;
}

void PolygonShape::UpdateVertices(float angle, const Vec2 &position)
{
    // Loop all the vertices, transforming from local to world space
    std::transform(localVertices.cbegin(), localVertices.cend(), worldVertices.begin(),
    [angle, &position](const Vec2& vertex){
        return vertex.Rotate(angle) + position;
    });
}

BoxShape::BoxShape(float width, float height)
    : PolygonShape({Vec2(-width / 2.0, -height / 2.0),
                    Vec2(+width / 2.0, -height / 2.0),
                    Vec2(+width / 2.0, +height / 2.0),
                    Vec2(-width / 2.0, +height / 2.0)}),
      width{width}, height{height} {}

std::unique_ptr<Shape> BoxShape::Clone() const
{
    return std::unique_ptr<Shape>(new BoxShape(width, height));
}

ShapeType BoxShape::GetType() const
{
    return ShapeType::BOX;
}

float BoxShape::GetMomentOfInertia() const
{
    return 0.083333 * (width * width + height * height);
}
