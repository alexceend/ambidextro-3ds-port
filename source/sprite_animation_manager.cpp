#include "sprite_animation_manager.h"
#include "objects.h"


void initialize_object(
    object_2d_t *object,
    size_t num_sprite_sheets,
    const C2D_SpriteSheet spriteSheets[MAX_SPRITE_SHEETS],
    const uint64_t refresh_times[MAX_SPRITE_SHEETS],
    float pos_x, float pos_y, bool x_flip, bool y_flip)
{
    object->position.x = pos_x;
    object->position.y = pos_y;
    object->rotation = 0.0f;
    object->rotation_velocity = 0.0f;
    object->reset_animation = false;

    for (size_t animation_index = 0; animation_index < num_sprite_sheets; animation_index++)
    {
        animation_t &animation = object->animations[animation_index];
        animation.sprite_sheet = spriteSheets[animation_index];
        animation.frame_info.num_of_sprites = C2D_SpriteSheetCount(animation.sprite_sheet);
        animation.frame_info.current_frame_index = 0;
        animation.frame_info.loop_once = false;

        animation.pivot.x = 0.0f;
        animation.pivot.y = 0.0f;

        for (size_t sprite_index = 0; sprite_index < animation.frame_info.num_of_sprites; sprite_index++)
        {
            C2D_Sprite *sprite = &animation.sprites[sprite_index];

            C2D_SpriteFromSheet(sprite, animation.sprite_sheet, sprite_index);
            C2D_SpriteSetCenter(sprite, animation.pivot.x, animation.pivot.y);
            C2D_SpriteSetPos(sprite, object->position.x, object->position.y);
            C2D_SpriteSetRotationDegrees(sprite, object->rotation);
            if (x_flip)
            {
                C2D_SpriteSetScale(sprite, -1.0f, 1.0f);
            }
            if (y_flip)
            {
                C2D_SpriteSetScale(sprite, 1.0f, -1.0f);
            }
        }
        C2D_SpriteFromSheet(&object->static_animation, object->animations[0].sprite_sheet, 0);
        if (x_flip)
        {
            C2D_SpriteSetScale(&object->static_animation, -1.0f, 1.0f);
        }
        if (y_flip)
        {
            C2D_SpriteSetScale(&object->static_animation, 1.0f, -1.0f);
        }

        animation.refresh_info.start = osGetTime();
        animation.refresh_info.stop = animation.refresh_info.start;
        animation.refresh_info.elapsed = 0;
        animation.refresh_info.refresh_time = refresh_times[animation_index];
    }
}

void deinitialize_object(object_2d_t *object)
{
    for (animation_t &animation : object->animations)
    {
        C2D_SpriteSheetFree(animation.sprite_sheet);
    }
}

void update_animation(object_2d_t *object, size_t animation_index)
{
    if (animation_index < MAX_SPRITE_SHEETS)
    {
        animation_t *animation = &object->animations[animation_index];
        object->rotation += object->rotation_velocity;

        for (size_t sprite_index = 0; sprite_index < animation->frame_info.num_of_sprites; sprite_index++)
        {
            C2D_SpriteSetPos(&(animation->sprites[sprite_index]), object->position.x, object->position.y);
            C2D_SpriteSetRotationDegrees(&(animation->sprites[sprite_index]), object->rotation);
        }
    }
}

void update_static(object_2d_t *object)
{
    C2D_SpriteSetPos(&object->static_animation, object->position.x, object->position.y);
}

void update_object(object_2d_t *object, size_t animation_index)
{
    if (animation_index < MAX_SPRITE_SHEETS)
    {
        update_animation(object, animation_index);
    }
    else
    {
        update_static(object);
    }
}

void draw_sprite_animation(object_2d_t *object, size_t animation_index)
{
    animation_t &animation = object->animations[animation_index];

    animation.refresh_info.stop = osGetTime();
    animation.refresh_info.elapsed = animation.refresh_info.stop - animation.refresh_info.start;

    if (object->reset_animation == true)
    {
        animation.frame_info.current_frame_index = 0;
        object->reset_animation = false;
    }

    else if (animation.refresh_info.elapsed >= animation.refresh_info.refresh_time)
    {
        if (animation.frame_info.loop_once == false)
        {
            animation.frame_info.current_frame_index = (animation.frame_info.current_frame_index + 1) % animation.frame_info.num_of_sprites;
        }
        else if (animation.frame_info.current_frame_index < animation.frame_info.num_of_sprites - 1)
        {
            animation.frame_info.current_frame_index++;
        }
        animation.refresh_info.start = osGetTime();
    }

    C2D_DrawSprite(&animation.sprites[animation.frame_info.current_frame_index]);
}

void draw_sprite_static(object_2d_t *object)
{
    C2D_DrawSprite(&object->static_animation);
}

void draw_sprite(object_2d_t *object, size_t animation_index)
{
    if (animation_index < MAX_SPRITE_SHEETS)
    {
        draw_sprite_animation(object, animation_index);
    }
    else
    {
        draw_sprite_static(object);
    }
}

SpriteAnimation::SpriteAnimation(ISubject &subject) : subject_(subject)
{
    this->subject_.Subscribe(MOVE_STOP, this);
    this->subject_.Subscribe(MOVE_RIGHT, this);
    this->subject_.Subscribe(MOVE_LEFT, this);
    this->subject_.Subscribe(AIRBORN, this);
    this->subject_.Subscribe(LAND, this);
}

SpriteAnimation::~SpriteAnimation() {}

void SpriteAnimation::Update(EventType event, void *callback)
{
    Wizard *wizard = (Wizard *)callback;

    if (event == AIRBORN)
    {
        wizard->sprite_info.currentAnimationType = JUMP_ANIMATION;
    }
    else if (event == LAND)
    {
        if (wizard->body->GetLinearVelocity().x == 0)
        {
            wizard->sprite_info.currentAnimationType = STATIC_ANIMATION;
        }
        else
        {
            wizard->sprite_info.currentAnimationType = MOVE_ANIMATION;
        }
    }
    else if (event == MOVE_STOP && wizard->body_properties.num_foot_contacts > 0)
    {
        wizard->sprite_info.currentAnimationType = STATIC_ANIMATION;
    }
    else if ((event == MOVE_LEFT || event == MOVE_RIGHT) && wizard->body_properties.num_foot_contacts > 0)
    {
        wizard->sprite_info.currentAnimationType = MOVE_ANIMATION;
    }

    wizard->object->reset_animation = true;
}