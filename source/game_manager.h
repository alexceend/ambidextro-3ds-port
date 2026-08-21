#include <map>

typedef enum
{
    WIN,
    PUASE,
    DEATH,
    MOVE_RIGHT,
    MOVE_LEFT,
    JUMP
} EventType;


class ISubject
{
    public:
        virtual ~ISubject(){};
        virtual void Subscribe(EventType event, IObserver* observer) = 0;
        virtual void Unsubscribe(EventType event, IObserver* observer) = 0;
        virtual void Notify(EventType event, void* callback) = 0;
};

class IObserver
{
    public:
        virtual ~IObserver(){};
        virtual void Update(EventType event, void* callback) = 0;
};