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


Subject::Subject()
{
    purpleWizard = {
        PURPLE,
        {
            WIZARD_WIDTH, 
            WIZARD_HEIGHT,
            WIZARD_SPEED,
            0
        },
        NULL,
        {
            STATIC_ANIMATION,
            2,
            {atlas_purple_wizard_static, atlas_purple_wizard_jump},
            {20, 20}
        },  
        new object_2d_t{},
        {{STATIC_ANIMATION, NULL}, {MOVE_ANIMATION, 0}, {JUMP_ANIMATION, 1}}
    };
    yellowWizard = {
        YELLOW, 
        {
            WIZARD_WIDTH, 
            WIZARD_HEIGHT,
            WIZARD_SPEED,
            0
        },
        NULL,
        {
            STATIC_ANIMATION,
            2,
            {atlas_yellow_wizard_static, atlas_yellow_wizard_jump},
            {20, 20}
        },
        new object_2d_t{},
        {{STATIC_ANIMATION, NULL}, {MOVE_ANIMATION, 0}, {JUMP_ANIMATION, 1}}
    };

    initialize_object(
        purpleWizard.object, purpleWizard.sprite_info.num_animations,
        purpleWizard.sprite_info.spriteSheets, purpleWizard.sprite_info.animations_refresh_ms_time,
        purpleWizard.object->position.x, purpleWizard.object->position.y,
        getAtlasTexture(atlas_purple_wizard_static, 0)
    );
    initialize_object(
        yellowWizard.object, yellowWizard.sprite_info.num_animations,
        yellowWizard.sprite_info.spriteSheets, yellowWizard.sprite_info.animations_refresh_ms_time,
        yellowWizard.object->position.x, yellowWizard.object->position.y,
        getAtlasTexture(atlas_yellow_wizard_static, 0)
    );
}

Subject::~Subject(){}

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
    for (IObserver* observer : observers[event])
    {
        observer->Update(event, callback);
    }
}

void Subject::Erase()
{
    for (auto& [event, observerList] : observers)
    {
        observerList.clear();
    }
    observers.clear();
}
void Subject::ManageGame(u32 kDown, u32 kHeld, u32 kUp)
{
    keyLogger(kDown, kHeld, kUp);
}

void Subject::keyLogger(u32 kDown, u32 kHeld, u32 kUp)
{
    movementLogger(kHeld, kUp);
    jumpLogger(kDown);
    exitLogger(kDown);
    debugLogger(kDown);
}

void Subject::movementLogger(u32 kHeld, u32 kUp)
{
    if (kHeld & KEY_LEFT)
    {
        Notify(MOVE_LEFT, &purpleWizard);
    }
    else if (kHeld & KEY_RIGHT)
    {
        Notify(MOVE_RIGHT, &purpleWizard);
    }
    else if (kUp & KEY_RIGHT || kUp & KEY_LEFT)
    {
        Notify(MOVE_STOP, &purpleWizard);
    }
    else if (kHeld & KEY_A)
    {
        Notify(MOVE_RIGHT, &yellowWizard);
    }
    else if(kHeld & KEY_Y)
    {
        Notify(MOVE_LEFT, &yellowWizard);
    }
    else if(kUp & KEY_A || kUp & KEY_Y)
    {
        Notify(MOVE_STOP, &yellowWizard);
    }
}

void Subject::jumpLogger(u32 kDown)
{
    if (kDown & KEY_UP && purpleWizard.body_properties.num_foot_contacts >= 1)
    {
        Notify(JUMP, &purpleWizard);
    }
    else if(kDown & KEY_X && yellowWizard.body_properties.num_foot_contacts >= 1)
    {
        Notify(JUMP, &yellowWizard);
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