#include "sprite_animation_manager.h"
#include "objects.h"

bool locked_animation_staff = false;
AnimationType staff_animation;

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

        animation.pivot.x = 0.5f;
        animation.pivot.y = 0.5f;

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
        object->static_animation_index = 0;

        if (x_flip)
        {
            C2D_SpriteSetScale(&object->animations[0].sprites[object->static_animation_index], -1.0f, 1.0f);
        }
        if (y_flip)
        {
            C2D_SpriteSetScale(&object->animations[0].sprites[object->static_animation_index], 1.0f, -1.0f);
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

        for (size_t sprite_index = 0; sprite_index < animation->frame_info.num_of_sprites; sprite_index++)
        {
            C2D_SpriteSetPos(&(animation->sprites[sprite_index]), object->position.x, object->position.y);
            C2D_SpriteSetRotationDegrees(&(animation->sprites[sprite_index]), object->rotation * 180 / M_PI);
        }
    }
}

void update_static(object_2d_t *object)
{
    C2D_SpriteSetPos(&object->animations[0].sprites[object->static_animation_index], object->position.x, object->position.y);
    C2D_SpriteSetRotationDegrees(&object->animations[0].sprites[object->static_animation_index], object->rotation * 180 / M_PI);
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
    C2D_DrawSprite(&object->animations[0].sprites[object->static_animation_index]);
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

void update_wizard(Wizard *wizard, EventType event)
{
    if (event == AIRBORN)
    {
        wizard->entity.entity_animation.sprite_info.currentAnimationType = JUMP_ANIMATION;
        staff_animation = JUMP_ANIMATION;
    }
    else if (event == LAND)
    {
        if (wizard->body->GetLinearVelocity().x == 0)
        {
            wizard->entity.entity_animation.sprite_info.currentAnimationType = STATIC_ANIMATION;
            staff_animation = STATIC_ANIMATION;
        }
        else
        {
            wizard->entity.entity_animation.sprite_info.currentAnimationType = MOVE_ANIMATION;
            staff_animation = MOVE_ANIMATION;
        }
    }
    else if (event == MOVE_STOP && wizard->num_foot_contacts > 0)
    {
        wizard->entity.entity_animation.sprite_info.currentAnimationType = STATIC_ANIMATION;
        staff_animation = STATIC_ANIMATION;
    }
    else if ((event == ANIMATE_LEFT || event == ANIMATE_RIGHT) && wizard->num_foot_contacts > 0)
    {
        wizard->entity.entity_animation.sprite_info.currentAnimationType = MOVE_ANIMATION;
        staff_animation = MOVE_ANIMATION;
    }
    else if (event == WIZARD_DETECTED && !locked_animation_staff)
    {
        locked_animation_staff = true;
        wizard->staff.entity.entity_animation.sprite_info.currentAnimationType = STATIC_ANIMATION;
        wizard->staff.entity.entity_animation.object->static_animation_index = 2;
    }
    else if (event == WIZARD_UNDETECTED)
    {
        locked_animation_staff = false;
        wizard->staff.entity.entity_animation.object->static_animation_index = 0;
    }

    wizard->entity.entity_animation.object->reset_animation = true;
    wizard->staff.entity.entity_animation.object->reset_animation = true;
}

void update_staff(Staff *staff)
{
    if (!locked_animation_staff)
    {
        staff->entity.entity_animation.sprite_info.currentAnimationType = staff_animation;
    }
}


SpriteAnimation::SpriteAnimation(ISubject &subject) : subject_(subject)
{
    this->subject_.Subscribe(MOVE_STOP, this);
    this->subject_.Subscribe(ANIMATE_LEFT, this);
    this->subject_.Subscribe(ANIMATE_RIGHT, this);
    this->subject_.Subscribe(AIRBORN, this);
    this->subject_.Subscribe(LAND, this);
    this->subject_.Subscribe(WIZARD_DETECTED, this);
    this->subject_.Subscribe(WIZARD_UNDETECTED, this);
}

SpriteAnimation::~SpriteAnimation() {}

void SpriteAnimation::Update(EventType event, void *callback)
{
    Entity *entity = static_cast<Entity *>(callback);
    if (entity == nullptr)
        return;
    switch (entity->entity_type)
    {
    case WIZARD_:
    {
        Wizard *wizard = static_cast<Wizard *>(entity->sub_struct);
        update_wizard(wizard, event);
        update_staff(&wizard->staff);
        break;
    }
    default:
        break;
    }
}