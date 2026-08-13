#include <3ds.h>
#include <box2d.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef PHYSICS_H
#define PHYSICS_H
#define TIME_STEP 1.0f / 60.0

std::unique_ptr<b2World> createWorld(b2Vec2 gravity);

static void loadGroundBox(int pos_x, int pos_y, int width, int height);

void loadCharacters();

#endif