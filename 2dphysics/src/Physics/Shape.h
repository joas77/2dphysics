#ifndef SHAPE_H
#define SHAPE_H

#include <vector>
#include <memory>
#include "Vec2.h"

enum class ShapeType {
    CIRCLE,
    POLYGON,
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


class PolygonShape: public Shape {
public:
    PolygonShape(const std::vector<Vec2>& vertices);
    PolygonShape(const PolygonShape&)=delete;
    PolygonShape& operator=(const PolygonShape&)=delete;
    
    ShapeType GetType() const override;
    float GetMomentOfInertia() const override;
    std::unique_ptr<Shape> Clone() const override;

    const std::vector<Vec2>& GetVertices() const;

    // Function to rotate and translate the polygon vertices from "local space" to "world space"
    void UpdateVertices(float angle, const Vec2& position);
protected:
    std::vector<Vec2> localVertices;
    std::vector<Vec2> worldVertices;
};

class BoxShape: public PolygonShape {
public:
    BoxShape(float width, float height);

    ShapeType GetType() const override;
    float GetMomentOfInertia() const override;
    std::unique_ptr<Shape> Clone() const override;
private:
    float width;
    float height;
};

#endif
