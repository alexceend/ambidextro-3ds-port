#include <iostream>
#include <list>
#include <string>
#include <map>
#include <list>
#include "physics.h"
#include "game_manager.h"
#include "movement.h"

Wizard purpleWizard = {PURPLE, WIZARD_WIDTH, WIZARD_HEIGHT, WIZARD_SPEED, NULL, 0};
Wizard yellowWizard = {YELLOW, WIZARD_WIDTH, WIZARD_HEIGHT, WIZARD_SPEED, NULL, 0};


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
    if (kDown & KEY_UP && purpleWizard.numFootContacts >= 1)
    {
        Notify(JUMP, &purpleWizard);
    }
    else if(kDown & KEY_X && yellowWizard.numFootContacts >= 1)
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