#include <iostream>
#include <list>
#include <string>
#include <map>
#include <list>
#include "physics.h"
#include "game_manager.h"
#include "movement.h"

#define WIZARD_HEIGHT 15.0f
#define WIZARD_WIDTH 12.0f
#define WIZARD_SPEED 3.0f
#define GRAVITY (0.0f, 9.8f)


Wizard purpleWizard = {PURPLE, WIZARD_WIDTH, WIZARD_HEIGHT, WIZARD_SPEED, NULL, 0};
Wizard yellowWizard = {YELLOW, WIZARD_WIDTH, WIZARD_HEIGHT, WIZARD_SPEED, NULL, 0};


class Subject : public ISubject
{
    public:
        virtual ~Subject()
        {
        }

        void Subscribe(EventType event, IObserver* observer) override
        {
            bool subscribed = false;
            std::list<IObserver*>::iterator it = observers[event].begin();
            while (it != observers[event].end())
            {
                if (*it == observer)
                {
                    subscribed = true;
                    break;
                }
                else it++;
            }
            if (!subscribed) observers[event].push_back(observer);
        }

        void Unsubscribe(EventType event, IObserver* observer) override
        {
            std::list<IObserver*>::iterator it = observers[event].begin();
            while (it != observers[event].end())
            {
                if (*it == observer)
                {
                    it = observers[event].erase(it);
                }
                else it++;
            }
        }

        void Notify(EventType event, void* callback) override
        {
            std::list<IObserver*>::iterator it = observers[event].begin();
            while (it != observers[event].end())
            {
                (*it)->Update(event, callback);
            }
        }

        void keyLogger(u32 kDown, u32 kHeld)
{
    movementLogger(kHeld);
    jumpLogger(kDown);
}

void keyLogger(u32 kDown, u32 kHeld)
{
    movementLogger(kHeld);
    jumpLogger(kDown);
}

void movementLogger(u32 kHeld)
{
    if (kHeld & KEY_LEFT)
    {
        Notify(MOVE_LEFT, &purpleWizard);
    }
    else if (kHeld & KEY_RIGHT)
    {
        Notify(MOVE_RIGHT, &purpleWizard);
    }
}

void jumpLogger(u32 kDown)
{
    if (kDown & KEY_UP)
    {
        Notify(JUMP, &purpleWizard);
    }
}
    private:
        std::map<EventType, std::list<IObserver*>> observers;
};