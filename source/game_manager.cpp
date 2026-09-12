#include <iostream>
#include <list>
#include <string>
#include <map>
#include <list>
#include "physics.h"
#include "game_manager.h"
#include "movement.h"
#include "sprite_animation_manager.h"
#include "objects.h"

std::array<Segment, CIRCLE_STEPS> segments;

Wizard yellowWizard;
Wizard purpleWizard;
RayCastCallback rayCastCallback = {};

bool purple_prev_air;
bool purple_current_air;
bool yellow_prev_air;
bool yellow_current_air;

bool raycast_wizard_prev = false;

C2D_Sprite purple_sprite;
C2D_Sprite yellow_sprite;

void createWizards()
{
    purpleWizard = {
        {WIZARD_,
         &purpleWizard,
         {WIZARD_WIDTH,
          WIZARD_HEIGHT,
          WIZARD_SPEED},

         {STATIC_ANIMATION,
          STATIC_ANIMATION,
          2,
          {atlas_purple_wizard_static, atlas_purple_wizard_jump},
          {20, 20}},

         new object_2d_t{},

         {{STATIC_ANIMATION, MAX_SPRITE_SHEETS}, {MOVE_ANIMATION, 0}, {JUMP_ANIMATION, 1}}},
        {{
             STAFF_,
             &purpleWizard.staff,
             {STAFF_WIDTH,
              STAFF_HEIGHT,
              WIZARD_SPEED},
             {STATIC_ANIMATION,
              STATIC_ANIMATION,
              2,
              {atlas_staff_static, atlas_staff_jump},
              {20, 20}},
             new object_2d_t{},
             {{STATIC_ANIMATION, MAX_SPRITE_SHEETS}, {MOVE_ANIMATION, 0}, {JUMP_ANIMATION, 1}},

         },
         NULL,
         1.0f,
         1.0f},

        {{WIZARD_FOOT_,
          &purpleWizard.foot_sensor,
          {WIZARD_WIDTH / 4,
           2.0f,
           WIZARD_SPEED},
          {},
          NULL,
          {}},
         &purpleWizard,
         NULL,
         0,
         WIZARD_HEIGHT / 2},

        PURPLE,
        NULL,
        0,
        true,
        false,
        false,
        false};
    yellowWizard = {
        {WIZARD_,
         &yellowWizard,
         {WIZARD_WIDTH,
          WIZARD_HEIGHT,
          WIZARD_SPEED},

         {STATIC_ANIMATION,
          STATIC_ANIMATION,
          2,
          {atlas_yellow_wizard_static, atlas_yellow_wizard_jump},
          {20, 20}},

         new object_2d_t{},

         {{STATIC_ANIMATION, MAX_SPRITE_SHEETS}, {MOVE_ANIMATION, 0}, {JUMP_ANIMATION, 1}}},
        {{STAFF_,
          &yellowWizard.staff,
          {STAFF_WIDTH,
           STAFF_HEIGHT,
           WIZARD_SPEED},
          {STATIC_ANIMATION,
           STATIC_ANIMATION,
           2,
           {atlas_staff_static, atlas_staff_jump},
           {20, 20}},
          new object_2d_t{},
          {{STATIC_ANIMATION, MAX_SPRITE_SHEETS}, {MOVE_ANIMATION, 0}, {JUMP_ANIMATION, 1}}},
         NULL,
         1.0f,
         1.0f},
        {{WIZARD_FOOT_,
          &yellowWizard.foot_sensor,
          {WIZARD_WIDTH / 4,
           2.0f,
           WIZARD_SPEED},
          {},
          NULL,
          {}},
         &yellowWizard,
         NULL,
         0,
         WIZARD_HEIGHT / 2},
        YELLOW,
        NULL,
        0,
        false,
        false,
        false,
        false};
}

Subject::Subject()
{
    createWizards();

    /*initialize_object(
        purpleWizard.entity.object, purpleWizard.entity.sprite_info.num_animations,
        purpleWizard.entity.sprite_info.spriteSheets, purpleWizard.entity.sprite_info.animations_refresh_ms_time,
        metersToPixels(purpleWizard.body->GetPosition().x) - purpleWizard.entity.body_properties.width / 2,
        metersToPixels(purpleWizard.body->GetPosition().y) - purpleWizard.entity.body_properties.height / 2,
        purpleWizard.x_flip, purpleWizard.y_flip);
    initialize_object(
        yellowWizard.entity.object, yellowWizard.entity.sprite_info.num_animations,
        yellowWizard.entity.sprite_info.spriteSheets, yellowWizard.entity.sprite_info.animations_refresh_ms_time,
        metersToPixels(yellowWizard.body->GetPosition().x) - yellowWizard.entity.body_properties.width / 2,
        metersToPixels(yellowWizard.body->GetPosition().y) - yellowWizard.entity.body_properties.height / 2,
        yellowWizard.x_flip, yellowWizard.y_flip);
    initialize_object(
        purpleWizard.staff.entity.object, purpleWizard.staff.entity.sprite_info.num_animations,
        purpleWizard.staff.entity.sprite_info.spriteSheets, purpleWizard.staff.entity.sprite_info.animations_refresh_ms_time,
        metersToPixels(purpleWizard.body->GetPosition().x) - purpleWizard.entity.body_properties.width / 2 - pixelsToMeters(purpleWizard.staff.offset_x),
        metersToPixels(purpleWizard.body->GetPosition().y) - purpleWizard.entity.body_properties.height / 2 + pixelsToMeters(purpleWizard.staff.offset_y),
        purpleWizard.x_flip, purpleWizard.y_flip
    );
    initialize_object(
        yellowWizard.staff.entity.object, yellowWizard.staff.entity.sprite_info.num_animations,
        yellowWizard.staff.entity.sprite_info.spriteSheets, yellowWizard.staff.entity.sprite_info.animations_refresh_ms_time,
        metersToPixels(yellowWizard.body->GetPosition().x) - yellowWizard.entity.body_properties.width / 2,
        metersToPixels(yellowWizard.body->GetPosition().y) - yellowWizard.entity.body_properties.height / 2,
        yellowWizard.x_flip, yellowWizard.y_flip
    );
    */

    initialize_object(
        purpleWizard.entity.object, purpleWizard.entity.sprite_info.num_animations,
        purpleWizard.entity.sprite_info.spriteSheets, purpleWizard.entity.sprite_info.animations_refresh_ms_time,
        0.0f, 0.0f,
        purpleWizard.x_flip, purpleWizard.y_flip);
    initialize_object(
        yellowWizard.entity.object, yellowWizard.entity.sprite_info.num_animations,
        yellowWizard.entity.sprite_info.spriteSheets, yellowWizard.entity.sprite_info.animations_refresh_ms_time,
        0.0f, 0.0f,
        yellowWizard.x_flip, yellowWizard.y_flip);
    initialize_object(
        purpleWizard.staff.entity.object, purpleWizard.staff.entity.sprite_info.num_animations,
        purpleWizard.staff.entity.sprite_info.spriteSheets, purpleWizard.staff.entity.sprite_info.animations_refresh_ms_time,
        0.0f, 0.0f,
        purpleWizard.x_flip, purpleWizard.y_flip);
    initialize_object(
        yellowWizard.staff.entity.object, yellowWizard.staff.entity.sprite_info.num_animations,
        yellowWizard.staff.entity.sprite_info.spriteSheets, yellowWizard.staff.entity.sprite_info.animations_refresh_ms_time,
        0.0f, 0.0f,
        yellowWizard.x_flip, yellowWizard.y_flip);

    purpleWizard.prev_air = purple_current_air == true ? false : true;
    yellowWizard.prev_air = yellow_current_air == true ? false : true;
}

Subject::~Subject() {}

void Subject::Subscribe(EventType event, IObserver *observer)
{
    bool subscribed = false;
    std::list<IObserver *>::iterator it = observers[event].begin();
    while (it != observers[event].end())
    {
        if (*it == observer)
        {
            subscribed = true;
            break;
        }
        else
            it++;
    }
    if (!subscribed)
        observers[event].push_back(observer);
}

void Subject::Unsubscribe(EventType event, IObserver *observer)
{
    std::list<IObserver *>::iterator it = observers[event].begin();
    while (it != observers[event].end())
    {
        if (*it == observer)
        {
            it = observers[event].erase(it);
        }
        else
            it++;
    }
}

void Subject::Notify(EventType event, void *callback)
{
    for (IObserver *observer : observers[event])
    {
        observer->Update(event, callback);
    }
}

void Subject::Erase()
{
    for (auto &[event, observerList] : observers)
    {
        observerList.clear();
    }
    observers.clear();
}
void Subject::ManageGame(u32 kHeld, u32 kDown, u32 kUp)
{
    keyLogger(kHeld, kDown, kUp);
    airbornLogger();
    segments = wizardDetectionLogger(
        {purpleWizard.body->GetPosition().x,
         purpleWizard.body->GetPosition().y},
        1.0f, purpleWizard.body);
}

void Subject::keyLogger(u32 kHeld, u32 kDown, u32 kUp)
{
    movementLogger(kHeld, kDown, kUp);
    jumpLogger(kDown);
    exitLogger(kDown);
    debugLogger(kDown);
    pauseLogger(kDown);
}

void Subject::pauseLogger(u32 kDown)
{
    if (kDown & KEY_START)
    {
        printf("Notified pause\n");
        Notify(PUASE, nullptr);
    }
}

void Subject::movementLogger(u32 kHeld, u32 kDown, u32 kUp)
{
    if (kHeld & KEY_LEFT && kHeld & KEY_RIGHT)
    {
        Notify(MOVE_STOP, &purpleWizard.entity);
    }
    else if (kHeld & KEY_LEFT)
    {
        Notify(MOVE_LEFT, &purpleWizard.entity);
    }
    else if (kHeld & KEY_RIGHT)
    {
        Notify(MOVE_RIGHT, &purpleWizard.entity);
    }

    if (kDown & KEY_LEFT)
    {
        Notify(ANIMATE_LEFT, &purpleWizard.entity);
    }
    else if (kDown & KEY_RIGHT)
    {
        Notify(ANIMATE_RIGHT, &purpleWizard.entity);
    }

    if (kUp & KEY_RIGHT)
    {
        if (kHeld & KEY_LEFT)
        {
            Notify(ANIMATE_LEFT, &purpleWizard.entity);
        }
        else
        {
            Notify(MOVE_STOP, &purpleWizard.entity);
        }
    }

    if (kUp & KEY_LEFT)
    {
        if (kHeld & KEY_RIGHT)
        {
            Notify(MOVE_RIGHT, &purpleWizard.entity);
        }
        else
        {
            Notify(MOVE_STOP, &purpleWizard.entity);
        }
    }

    if (kHeld & KEY_A && kHeld & KEY_Y)
    {
        Notify(MOVE_STOP, &yellowWizard.entity);
    }
    else if (kHeld & KEY_A)
    {
        Notify(MOVE_RIGHT, &yellowWizard.entity);
    }
    else if (kHeld & KEY_Y)
    {
        Notify(MOVE_LEFT, &yellowWizard.entity);
    }

    if (kDown & KEY_A)
    {
        Notify(ANIMATE_RIGHT, &yellowWizard.entity);
    }
    else if (kDown & KEY_Y)
    {
        Notify(ANIMATE_LEFT, &yellowWizard.entity);
    }

    if (kUp & KEY_A)
    {
        if (kHeld & KEY_Y)
        {
            Notify(ANIMATE_LEFT, &yellowWizard.entity);
        }
        else
        {
            Notify(MOVE_STOP, &yellowWizard.entity);
        }
    }

    if (kUp & KEY_Y)
    {
        if (kHeld & KEY_A)
        {
            Notify(ANIMATE_RIGHT, &yellowWizard.entity);
        }
        else
        {
            Notify(MOVE_STOP, &yellowWizard.entity);
        }
    }
}

void Subject::jumpLogger(u32 kDown)
{
    if (kDown & KEY_UP && purpleWizard.num_foot_contacts >= 1)
    {
        Notify(JUMP, &purpleWizard.entity);
        purple_current_air = true;
    }
    if (kDown & KEY_X && yellowWizard.num_foot_contacts >= 1)
    {
        Notify(JUMP, &yellowWizard.entity);
        yellow_current_air = true;
    }
}

void Subject::airbornLogger()
{
    if (purpleWizard.num_foot_contacts < 1)
    {
        purple_current_air = true;
    }
    else
        purple_current_air = false;
    if (yellowWizard.num_foot_contacts < 1)
    {
        yellow_current_air = true;
    }
    else
        yellow_current_air = false;

    if (purple_prev_air != purple_current_air)
    {
        if (purple_prev_air == false)
        {
            Notify(AIRBORN, &purpleWizard.entity);
            purple_prev_air = true;
        }
        else
        {
            Notify(LAND, &purpleWizard.entity);
            purple_prev_air = false;
        }
    }

    if (yellow_prev_air != yellow_current_air)
    {
        if (yellow_prev_air == false)
        {
            Notify(AIRBORN, &yellowWizard.entity);
            yellow_prev_air = true;
        }
        else
        {
            Notify(LAND, &yellowWizard.entity);
            yellow_prev_air = false;
        }
    }
}

void Subject::exitLogger(u32 kDown)
{
    if (kDown & KEY_START)
    {
        Notify(EXIT, NULL);
    }
}

void Subject::debugLogger(u32 kDown)
{
    if (kDown & KEY_DOWN)
    {
        Notify(DEBUG, NULL);
    }
}

std::array<Segment, CIRCLE_STEPS> Subject::wizardDetectionLogger(b2Vec2 p1, float radius, b2Body *ignoredBody)
{
    std::array<Segment, CIRCLE_STEPS> segments;
    bool wizard_detected = false;

    for (int i = 0; i < CIRCLE_STEPS; i++)
    {
        float radians = (2.0f * M_PI * i) / CIRCLE_STEPS;
        b2Vec2 p2 = p1 + radius * b2Vec2(sinf(radians), cosf(radians));

        rayCastCallback.m_fixture = nullptr;
        rayCastCallback.m_point = p2;
        rayCastCallback.ignoredBody = ignoredBody;

        world->RayCast(&rayCastCallback, p1, p2);
        segments[i] = {p1, rayCastCallback.m_point};
        if (fixtureIsWizard(rayCastCallback.m_fixture))
        {
            wizard_detected = true;
        }
    }

    if (wizard_detected && !raycast_wizard_prev)
    {
        Notify(WIZARD_DETECTED, nullptr);
        printf("Wizard detected!\n");
    }

    if (!wizard_detected && raycast_wizard_prev)
    {
        Notify(WIZARD_UNDETECTED, nullptr);
        printf("Wizrad undetected\n");
    }

    raycast_wizard_prev = wizard_detected;

    return segments;
}
