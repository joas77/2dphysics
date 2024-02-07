#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include "Vec2.h"

enum class ShapeType {
    CIRCLE,
    POLIGON,
    BOX
};

class Shape {
public:
    virtual ~Shape() = default;
    virtual ShapeType GetType() const = 0;
    virtual float GetMomentOfInertia() const = 0;
};

class CircleShape: public Shape {
public:
    CircleShape(const float radius);
    virtual ~CircleShape();
    ShapeType GetType() const override;
    
    float radius;
    float GetMomentOfInertia() const override;
};

class PoligonShape: public Shape {
public:
    PoligonShape() = default;
    PoligonShape(const std::vector<Vec2>& vertices);
    ShapeType GetType() const override;
    virtual ~PoligonShape();
    float GetMomentOfInertia() const override;
private:
    std::vector<Vec2> vertices;
};

class BoxShape: public PoligonShape {
public:
    BoxShape(float width, float height);
    virtual ~BoxShape();
    ShapeType GetType() const override;
    float GetMomentOfInertia() const override;

private:
    float width;
    float height;
};

#endif
