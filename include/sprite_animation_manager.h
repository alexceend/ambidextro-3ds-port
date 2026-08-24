#ifndef SPRITE_ANIMATION_MANAGER_H
#define SPRITE_ANIMATION_MANAGER_H

#include <citro2d.h>
#include <3ds.h>
#include <stdbool.h>

#define MAX_SPRITE_SHEETS 10

typedef struct sprite_pivot
{
    float x;
    float y;
} sprite_pivot_t;

typedef struct sprite_position
{
    float x;
    float y;
} sprite_position_t;

typedef struct sprite_refresh_info
{
    uint64_t start;
    uint64_t stop;
    uint64_t elapsed;
    uint64_t refresh_time;
} sprite_refresh_info_t;

typedef struct sprite_frame_info
{
    int current_frame_index;
    size_t num_of_sprites;
    bool loop_once;
} sprite_frame_info_t;

typedef struct animation
{
    C2D_SpriteSheet sprite_sheet;
    sprite_refresh_info_t refresh_info;
    sprite_frame_info_t frame_info;
    sprite_pivot_t pivot;
    float rotation;
    float rotation_velocity;
} animation_t;

typedef struct object_2d
{
    C2D_Sprite* object_sprite;
    animation_t animations[MAX_SPRITE_SHEETS];
} object_2d_t;

void initialize_object(
    object_2d_t* object, C2D_Sprite* sprites, size_t num_sprite_sheets, const C2D_SpriteSheet[MAX_SPRITE_SHEETS]
);

void deinitialize_object(object_2d_t* object);

void update_object(object_2d_t* object);

void draw_sprite_animation(object_2d_t* object);

#endif