#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#include <box2d/box2d.h>
#include <variant>

struct Rectangle
{
    float width;
    float height;
};

struct Triangle
{
    b2Vec2 vertices[3];
};

struct Circle
{
    float radius;
};

typedef  struct
{
  b2Vec2 p1;
  b2Vec2 p2;
} Segment;


using CollisionGeometry = std::variant<
    Rectangle,
    Triangle,
    Circle
>;

struct TileInfo
{
    int offsetX;
    int offsetY;
    CollisionGeometry geometry;
};


#endif
