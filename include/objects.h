#ifndef OBJECTS_H
#define OBJECTS_H

#include "sprite_animation_manager.h"
#include "assets_loader.h"
#include "game_constants.h"
#include "animation_types.h"
#include <physics.h>
#include <map>

typedef enum
{
    WIZARD_,
    STAFF_
} EntityType;

typedef enum
{
    PURPLE,
    YELLOW
} WizardType;


typedef struct sprite_info
{
    AnimationType prevAnimationType;
    AnimationType currentAnimationType;
    size_t num_animations;
    C2D_SpriteSheet spriteSheets[MAX_SPRITE_SHEETS];
    uint64_t animations_refresh_ms_time[MAX_SPRITE_SHEETS];
} sprite_info_t;


typedef struct body_properties
{
    float width;
    float height;
    float velocity;
    
} body_properties_t;

typedef struct Entity
{
    EntityType entity_type;
    void* sub_struct;
    body_properties_t body_properties;
    sprite_info_t sprite_info;
    object_2d_t* object;
    std::map<AnimationType, size_t> animation_map;
} Entity;

typedef struct Wizard
{
    Entity entity;
    WizardType wizard_type;
    b2Body* body;
    int num_foot_contacts;
    bool x_flip;
    bool y_flip;
    bool current_air;
    bool prev_air;
} Wizard;

typedef struct Staff
{
    b2Body* body;
    Entity entity;
    float offset_x;
    float offset_y;
} Staff;

#endif