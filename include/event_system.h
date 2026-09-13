#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include <3ds.h>
#include <map>
#include <list>
#include <box2d/box2d.h>
#include "game_constants.h"
#include "game_structs.h"

typedef enum
{
    WIN,
    PUASE,
    DEATH,
    MOVE_RIGHT,
    ANIMATE_RIGHT,
    MOVE_LEFT,
    ANIMATE_LEFT,
    MOVE_STOP,
    JUMP,
    LAND,
    AIRBORN,
    EXIT,
    DEBUG,
    WIZARD_DETECTED,
    WIZARD_UNDETECTED
} EventType;

class IObserver
{
public:
    virtual ~IObserver() {};
    virtual void Update(EventType event, void *callback) = 0;
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
    Subject();
    ~Subject();
    void Subscribe(EventType event, IObserver *observer);
    void Unsubscribe(EventType event, IObserver *observer);
    void Notify(EventType event, void *callback);
    void Erase();
    void ManageGame(u32 kHeld, u32 kDown, u32 kUp);
    void keyLogger(u32 kHeld, u32 kDown, u32 kUp);
    void movementLogger(u32 kHeld, u32 kDown, u32 kUp);
    void jumpLogger(u32 kDown);
    void pauseLogger(u32 kDown);
    void airbornLogger();
    void exitLogger(u32 kDown);
    void debugLogger(u32 kDown);
    void victoryLogger();
    std::array<Segment, CIRCLE_STEPS> wizardDetectionLogger(b2Vec2 p1, float radius);

private:
    std::map<EventType, std::list<IObserver *>> observers;
};

#endif