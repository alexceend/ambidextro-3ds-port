#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include <3ds.h>
#include <map>
#include <list>

typedef enum
{
    WIN,
    PUASE,
    DEATH,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_STOP,
    JUMP,
    EXIT,
    DEBUG
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
    void debugLogger(u32 kDown);

private:
    std::map<EventType, std::list<IObserver *>> observers;
};

#endif