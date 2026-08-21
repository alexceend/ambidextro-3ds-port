#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <3ds.h>
#include <box2d/box2d.h>
#include <memory>
#include <stdio.h>
#include "physics.h"

typedef enum
{
    MS_STOP,
    MS_LEFT,
    MS_RIGHT
} MoveState;

void jump(b2Body* body);
#endif

