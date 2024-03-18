#include <iostream>
#include "Shape.h"

CircleShape::CircleShape(float  radius):
    radius{radius}
{
    std::cout << "CicleShape constructor called!" <<  std::endl;
}

CircleShape::~CircleShape() {
    std::cout << "CicleShape destructor called!" <<  std::endl;
}

std::unique_ptr<Shape> CircleShape::Clone() const {
    return std::make_unique<CircleShape>(radius);
}

ShapeType CircleShape::GetType() const {
    return ShapeType::CIRCLE; 
}

float CircleShape::GetMomentOfInertia() const {
    // this still needs to be multiplied by the rigidbody's mass
    return 0.5 * radius * radius;
}

PoligonShape::PoligonShape(const std::vector<Vec2>& vertices) {
    // TODO: ...
}

PoligonShape::~PoligonShape() {
    // TODO: ...
}

std::unique_ptr<Shape> PoligonShape::Clone() const {
    return std::make_unique<PoligonShape>(vertices);
}

float PoligonShape::GetMomentOfInertia() const {
    // TODO...
    return 0.0;
}

ShapeType PoligonShape::GetType() const {
    return ShapeType::POLIGON;
}

// BoxShape::BoxShape(float width, float height) : PoligonShape() {
//     // TODO: ...
// }


// std::unique_ptr<Shape> BoxShape::Clone() const {
//     return std::make_unique<Shape>(width, height);
// }

// ShapeType BoxShape::GetType() const {
//     return ShapeType::BOX;
// }

// float BoxShape::GetMomentOfInertia() const {
//     return 0.083333 * (width * width + height * height);
// }
