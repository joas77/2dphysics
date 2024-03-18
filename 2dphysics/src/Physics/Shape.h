#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <memory>
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
    virtual std::unique_ptr<Shape> Clone() const = 0;
};

class CircleShape: public Shape {
public:
    CircleShape(const float radius);
    CircleShape() = delete;
    virtual ~CircleShape();
    ShapeType GetType() const override;
    
    float radius;
    float GetMomentOfInertia() const override;
    std::unique_ptr<Shape> Clone() const override;
};

class PoligonShape: public Shape {
public:
    //PoligonShape() = delete;
    PoligonShape(const std::vector<Vec2>& vertices);
    ShapeType GetType() const override;
    virtual ~PoligonShape();
    float GetMomentOfInertia() const override;
    std::unique_ptr<Shape> Clone() const override;
private:
    std::vector<Vec2> vertices;
};

class BoxShape: public PoligonShape {
public:
    //BoxShape() = delete;
    BoxShape(float width, float height);

    ShapeType GetType() const override;
    float GetMomentOfInertia() const override;
    std::unique_ptr<Shape> Clone() const override;
private:
    float width;
    float height;
};

#endif
