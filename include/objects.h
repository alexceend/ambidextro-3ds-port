#ifndef OBJECTS_H
#define OBJECTS_H

#include "sprite_animation_manager.h"
#include "assets_loader.h"
#include "game_constants.h"
#include <physics.h>

typedef enum
{
    PURPLE,
    YELLOW
} WizardType;

typedef enum
{
    STATIC_ANIMATION,
    MOVE_ANIMATION,
    JUMP_ANIMATION
} AnimationType;

typedef struct sprite_info
{
    AnimationType currentAnimation;
    C2D_Image static_animation;
    size_t num_animations;
    C2D_SpriteSheet spriteSheets[MAX_SPRITE_SHEETS];
    uint64_t animations_refresh_ms_time[MAX_SPRITE_SHEETS];
} sprite_info_t;

typedef struct body_properties
{
    float width;
    float height;
    float velocity;
    int num_foot_contacts;
    float pos[2];
} body_properties_t;

typedef struct Wizard
{
    WizardType wizard_type;
    body_properties_t body_properties;
    b2Body* body;
    sprite_info_t sprite_info;
    object_2d_t* object;
} Wizard;

#endif