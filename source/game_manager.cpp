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

Wizard yellowWizard;
Wizard purpleWizard;

bool purple_prev_air;
bool purple_current_air;
bool yellow_prev_air;
bool yellow_current_air;

C2D_Sprite purple_sprite;
C2D_Sprite yellow_sprite;
Subject::Subject()
{
    purpleWizard = {
        PURPLE,
        {WIZARD_WIDTH,
         WIZARD_HEIGHT,
         WIZARD_SPEED,
         0},
        NULL,
        {STATIC_ANIMATION,
         STATIC_ANIMATION,
         2,
         {atlas_purple_wizard_static, atlas_purple_wizard_jump},
         {20, 20}},
        new object_2d_t{},
        {{STATIC_ANIMATION, MAX_SPRITE_SHEETS}, {MOVE_ANIMATION, 0}, {JUMP_ANIMATION, 1}},
        true, false,
        false, false
    };
    yellowWizard = {
        YELLOW,
        {WIZARD_WIDTH,
         WIZARD_HEIGHT,
         WIZARD_SPEED,
         0},
        NULL,
        {STATIC_ANIMATION,
         STATIC_ANIMATION,
         2,
         {atlas_yellow_wizard_static, atlas_yellow_wizard_jump},
         {20, 20}},
        new object_2d_t{},
        {{STATIC_ANIMATION, MAX_SPRITE_SHEETS}, {MOVE_ANIMATION, 0}, {JUMP_ANIMATION, 1}},
        false, false,
        false, false
    };

    initialize_object(
        purpleWizard.object, purpleWizard.sprite_info.num_animations,
        purpleWizard.sprite_info.spriteSheets, purpleWizard.sprite_info.animations_refresh_ms_time,
        metersToPixels(purpleWizard.body->GetPosition().x) - purpleWizard.body_properties.width / 2,
        metersToPixels(purpleWizard.body->GetPosition().y) - purpleWizard.body_properties.height / 2,
        purpleWizard.x_flip, purpleWizard.y_flip);
    initialize_object(
        yellowWizard.object, yellowWizard.sprite_info.num_animations,
        yellowWizard.sprite_info.spriteSheets, yellowWizard.sprite_info.animations_refresh_ms_time,
        metersToPixels(yellowWizard.body->GetPosition().x) - yellowWizard.body_properties.width / 2,
        metersToPixels(yellowWizard.body->GetPosition().y) - yellowWizard.body_properties.height / 2,
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
void Subject::ManageGame(u32 kDown, u32 kUp)
{
    keyLogger(kDown, kUp);
    airbornLogger();
}

void Subject::keyLogger(u32 kDown, u32 kUp)
{
    movementLogger(kDown, kUp);
    jumpLogger(kDown);
    exitLogger(kDown);
    debugLogger(kDown);
}

void Subject::movementLogger(u32 kDown, u32 kUp)
{

    if (kDown & KEY_LEFT)
    {
        Notify(MOVE_LEFT, &purpleWizard);
    }
    else if (kDown & KEY_RIGHT)
    {
        Notify(MOVE_RIGHT, &purpleWizard);
    }
    else if (kUp & KEY_RIGHT || kUp & KEY_LEFT)
    {
        Notify(MOVE_STOP, &purpleWizard);
    }
    else if (kDown & KEY_A)
    {
        Notify(MOVE_RIGHT, &yellowWizard);
    }
    else if (kDown & KEY_Y)
    {
        Notify(MOVE_LEFT, &yellowWizard);
    }
    else if (kUp & KEY_A || kUp & KEY_Y)
    {
        Notify(MOVE_STOP, &yellowWizard);
    }
}

void Subject::jumpLogger(u32 kDown)
{
    if (kDown & KEY_UP && purpleWizard.body_properties.num_foot_contacts >= 1)
    {
        Notify(JUMP, &purpleWizard);
        purple_current_air = true;
    }
    if (kDown & KEY_X && yellowWizard.body_properties.num_foot_contacts >= 1)
    {
        Notify(JUMP, &yellowWizard);
        yellow_current_air = true;
    }
}

void Subject::airbornLogger()
{
    if (purpleWizard.body_properties.num_foot_contacts < 1)
    {
        purple_current_air = true;
    }
    else
        purple_current_air = false;
    if (yellowWizard.body_properties.num_foot_contacts < 1)
    {
        yellow_current_air = true;
    }
    else
        yellow_current_air = false;

    if (purple_prev_air != purple_current_air)
    {
        if (purple_prev_air == false)
        {
            Notify(AIRBORN, &purpleWizard);
            purple_prev_air = true;
        }
        else
        {
            Notify(LAND, &purpleWizard);
            purple_prev_air = false;
        }
    }

    if (yellow_prev_air != yellow_current_air)
    {
        if (yellow_prev_air == false)
        {
            Notify(AIRBORN, &yellowWizard);
            yellow_prev_air = true;
        }
        else
        {
            Notify(LAND, &yellowWizard);
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
