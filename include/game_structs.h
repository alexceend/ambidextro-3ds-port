#ifndef GAME_STRUCTS_H
#define GAME_STRUCTS_H

#include <box2d/box2d.h>

typedef  struct
{
  b2Vec2 p1;
  b2Vec2 p2;
} Segment;


enum CollisionShape
{
    RECTANGLE_SHAPE,
    TRIANGLE_SHAPE,
};

struct TileInfo
{
    int width;
    int height;
    int offsetX;
    int offsetY;
    CollisionShape collisionShape;
};


#endif
