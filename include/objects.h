#ifndef OBJECTS_H
#define OBJECTS_H

#include "sprite_animation_manager.h"
#include "assets_loader.h"
#include "physics.h"

#define MAX_SPRITES 15

typedef enum
{
    PURPLE,
    YELLOW,
} WizardType;

typedef struct Wizard
{
    WizardType wizard_type;
    float width;
    float height;
    float velocity;
    b2Body* body;
    int numFootContacts;
    uint64_t sprite_refresh_ms_time;
    float pos[2];
    object_2d_t* object;
    C2D_SpriteSheet spriteSheets[MAX_SPRITES];
} Wizard;

#endif