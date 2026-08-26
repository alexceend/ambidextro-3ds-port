#ifndef SPRITE_ANIMATION_MANAGER_H
#define SPRITE_ANIMATION_MANAGER_H

#include <citro2d.h>
#include <3ds.h>
#include <stdbool.h>
#include "event_system.h"
#include "game_constants.h"

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
    size_t current_frame_index;
    size_t num_of_sprites;
    bool loop_once;
} sprite_frame_info_t;

typedef struct animation
{
    C2D_SpriteSheet sprite_sheet;

    C2D_Sprite sprites[MAX_SPRITES];

    sprite_refresh_info_t refresh_info;
    sprite_frame_info_t frame_info;

    sprite_pivot_t pivot;

} animation_t;

typedef struct object_2d
{
    sprite_position_t position;

    float rotation;
    float rotation_velocity;

    animation_t animations[MAX_SPRITE_SHEETS];
    C2D_Image static_animation;
} object_2d_t;

void initialize_object(
    object_2d_t *object,
    size_t num_sprite_sheets,
    const C2D_SpriteSheet spriteSheets[MAX_SPRITE_SHEETS],
    const uint64_t refresh_times[MAX_SPRITE_SHEETS],
    float pos_x, float pos_y,
    C2D_Image static_animation
);

void deinitialize_object(object_2d_t* object);

void update_object(object_2d_t* object);

void draw_sprite_static(object_2d_t* object);

void draw_sprite_animation(object_2d_t *object, size_t animation_index);

void draw_sprite(object_2d_t* object, size_t* animation_index = NULL);

class SpriteAnimation : public IObserver
{
    private:
        ISubject& subject_;
    public:
        SpriteAnimation(ISubject& subject);
        virtual ~SpriteAnimation();
        void Update(EventType event, void* callback);
};

#endif