#include "sprite_animation_manager.h"

void initialize_object(
    object_2d_t* object, C2D_Sprite* sprites, const char* filename,
    const sprite_pivot_t pivot, const sprite_position_t position, const float rotation,
    uint64_t animation_refesh_time, bool loop_once
)
{
    object->sprite_sheet = C2D_SpriteSheetLoad(filename);
    if (object->sprite_sheet == NULL)
    {
        svcBreak(USERBREAK_PANIC);
    }

    object->object_sprite = sprites;

    object->frame_info.num_of_sprites = C2D_SpriteSheetCount(object->sprite_sheet);

    object->frame_info.current_frame_index = 0;

    object->position.x = position.x;
    object->position.y = position.y;

    object->rotation_velocity = 0.0f;

    object->pivot.x = pivot.x;
    object->pivot.y = pivot.y;

    object->rotation = rotation;

    object->frame_info.loop_once = loop_once;

    for (size_t index = 0; index < object->frame_info.num_of_sprites; index++)
    {
        C2D_SpriteFromSheet(&object->object_sprite[index], object->sprite_sheet, index);

        C2D_SpriteSetCenter(&object->object_sprite[index], object->pivot.x, object->pivot.y);

        C2D_SpriteSetPos(&object->object_sprite[index], object->position.x, object->position.y);

        C2D_SpriteSetRotationDegrees(&object->object_sprite[index], object->rotation);
    }

    object->refresh_info.start = osGetTime();
    object->refresh_info.elapsed = 0;

    object->refresh_info.refresh_time = animation_refesh_time;
}

void deinitialize_object(object_2d_t* object)
{
    C2D_SpriteSheetFree(object->sprite_sheet);
}

void update_object(object_2d_t* object)
{
    object->rotation += object->rotation_velocity;

    for (size_t index = 0; index < object->frame_info.num_of_sprites; index++)
    {
        C2D_SpriteSetPos(&object->object_sprite[index], object->position.x, object->position.y);
        C2D_SpriteSetRotationDegrees(&object->object_sprite[index], object->rotation);
    }
}

void draw_sprite_animation(object_2d_t* object)
{
    object->refresh_info.stop = osGetTime();
    object->refresh_info.elapsed = object->refresh_info.stop - object->refresh_info.start;

    if (object->refresh_info.elapsed >= object->refresh_info.refresh_time)
    {
        if (object->frame_info.loop_once == false)
        {
            object->frame_info.current_frame_index = (object->frame_info.current_frame_index + 1) % object->frame_info.num_of_sprites;
        }
        else
        {
            if (object->frame_info.current_frame_index <= object->frame_info.num_of_sprites - 1)
            {
                object->frame_info.current_frame_index++;
            }
        }
        object->refresh_info.start = osGetTime();
        C2D_DrawSprite(&object->object_sprite[object->frame_info.current_frame_index]);
    }
    else
    {
        C2D_DrawSprite(&object->object_sprite[object->frame_info.current_frame_index]);
    }
}