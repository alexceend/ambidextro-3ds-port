#ifndef PHYSICS_H
#define PHYSICS_H

#include <3ds.h>
#include <box2d/box2d.h>
#include <memory>
#include <stdio.h>


typedef enum
{
    PURPLE,
    YELLOW,
} WizardType;

typedef struct
{
    WizardType wizard_type;
    int pos_x;
    int pos_y;
    float velocity;
    b2Body* body;
} Wizard;



extern std::unique_ptr<b2World> world;

std::unique_ptr<b2World> createWorld(b2Vec2 gravity);


void loadGroundBox(int pos_x, int pos_y, int width, int height);
void loadWizardHitbox(float pos_x, float pos_y, Wizard* wizard);

void updatePhysics(Wizard* wizard);

void loadCharacters();

#endif