#include "sprite_animation_manager.h"

void initialize_object(
    object_2d_t* object, C2D_Sprite* sprites, size_t num_sprite_sheets, const C2D_SpriteSheet spriteSheets[MAX_SPRITE_SHEETS]
)
{
    for (size_t sprite_sheet_index = 0; sprite_sheet_index < num_sprite_sheets; sprite_sheet_index++)
    {
        object->animations[sprite_sheet_index].sprite_sheet = spriteSheets[sprite_sheet_index];
        object->animations[sprite_sheet_index].frame_info.num_of_sprites = C2D_SpriteSheetCount(object->animations[sprite_sheet_index].sprite_sheet);

        object->animations[sprite_sheet_index].frame_info.current_frame_index = 0;

        object->animations[sprite_sheet_index].rotation_velocity = 0.0f;
        
        object->animations[sprite_sheet_index].pivot.x = 0.0f;
        object->animations[sprite_sheet_index].pivot.y = 0.0f;

        object->animations[sprite_sheet_index].rotation = 0.0f;

        object->animations[sprite_sheet_index].frame_info.loop_once = false;

        for (size_t sprite_index = 0; sprite_index < object->animations[sprite_sheet_index].frame_info.num_of_sprites; sprite_index++)
        {
            C2D_SpriteFromSheet(&object->object_sprite[sprite_index], object->animations[sprite_sheet_index].sprite_sheet, sprite_index);

            C2D_SpriteSetCenter(&object->object_sprite[sprite_index], object->animations[sprite_sheet_index].pivot.x, object->animations[sprite_sheet_index].pivot.y);

            C2D_SpriteSetRotationDegrees(&object->object_sprite[sprite_index], object->animations[sprite_sheet_index].rotation);
        }

        object->animations[sprite_sheet_index].refresh_info.start = osGetTime();
        object->animations[sprite_sheet_index].refresh_info.elapsed = 0;

        object->animations[sprite_sheet_index].refresh_info.refresh_time = 20;
    }

    object->object_sprite = sprites;
}

void deinitialize_object(object_2d_t* object)
{
    for (animation_t& animation : object->animations)
    {
        C2D_SpriteSheetFree(animation.sprite_sheet);
    }
}

// It will probably not be used
/*
void update_object(object_2d_t* object)
{
    object->rotation += object->rotation_velocity;

    for (size_t index = 0; index < object->frame_info.num_of_sprites; index++)
    {
        C2D_SpriteSetPos(&object->object_sprite[index], object->position.x, object->position.y);
        C2D_SpriteSetRotationDegrees(&object->object_sprite[index], object->rotation);
    }
}
*/

void draw_sprite_animation(object_2d_t* object, size_t animation_index)
{
    animation_t& animation = object->animations[animation_index];

    animation.refresh_info.stop = osGetTime();
    animation.refresh_info.elapsed = animation.refresh_info.stop - animation.refresh_info.start;

    if (animation.refresh_info.elapsed >= animation.refresh_info.refresh_time)
    {
        if (animation.frame_info.loop_once == false)
        {
            animation.frame_info.current_frame_index = (animation.frame_info.current_frame_index + 1) % animation.frame_info.num_of_sprites;
        }
        else
        {
            if (animation.frame_info.current_frame_index <= animation.frame_info.num_of_sprites - 1)
            {
                animation.frame_info.current_frame_index++;
            }
        }
        animation.refresh_info.start = osGetTime();
        C2D_DrawSprite(&object->object_sprite[animation.frame_info.current_frame_index]);
    }
    else
    {
        C2D_DrawSprite(&object->object_sprite[animation.frame_info.current_frame_index]);
    }
}