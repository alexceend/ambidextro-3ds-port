#ifndef PHYSICS_H
#define PHYSICS_H

#include <3ds.h>
#include <box2d/box2d.h>
#include <memory>
#include <stdio.h>

std::unique_ptr<b2World> createWorld(b2Vec2 gravity);



static void loadGroundBox(int pos_x, int pos_y, int width, int height);

void loadCharacters();

#endif