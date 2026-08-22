#ifndef GAME_MANAGER_H

#define GAME_MANAGER_H

#include <map>
#include "physics.h"
#include <list>
#include <string>
#include <map>

#define WIZARD_HEIGHT 15.0f
#define WIZARD_WIDTH 12.0f
#define WIZARD_SPEED 3.0f
#define GRAVITY (0.0f, 9.8f)

extern Wizard purpleWizard;
extern Wizard yellowWizard;

typedef enum
{
    WIN,
    PUASE,
    DEATH,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_STOP,
    JUMP,
    EXIT
} EventType;

class IObserver
{
public:
    virtual ~IObserver() {};
    virtual void Update(EventType event, void *callback) = 0;
    // virtual void ManageGame(u32 kDown, u32 kHeld);
};

class ISubject
{
public:
    virtual ~ISubject() {};
    virtual void Subscribe(EventType event, IObserver *observer) = 0;
    virtual void Unsubscribe(EventType event, IObserver *observer) = 0;
    virtual void Notify(EventType event, void *callback) = 0;
};

class Subject : public ISubject
{
public:
    ~Subject();
    void Subscribe(EventType event, IObserver *observer);
    void Unsubscribe(EventType event, IObserver *observer);
    void Notify(EventType event, void *callback);
    void ManageGame(u32 kDown, u32 kHeld, u32 kUp);
    void keyLogger(u32 kDown, u32 kHeld, u32 kUp);
    void movementLogger(u32 kHeld, u32 kUp);
    void jumpLogger(u32 kDown);
    void exitLogger(u32 kDown);

private:
    std::map<EventType, std::list<IObserver *>> observers;
};

#endif