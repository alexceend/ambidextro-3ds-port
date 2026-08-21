#include <map>
#include "physics.h"

#define WIZARD_HEIGHT 15.0f
#define WIZARD_WIDTH 12.0f
#define WIZARD_SPEED 3.0f
#define GRAVITY (0.0f, 9.8f)

extern Wizard purpleWizard;
extern Wizard yellowWizard;

void manage_game(u32 kDown, u32 kHeld);

typedef enum
{
    WIN,
    PUASE,
    DEATH,
    MOVE_RIGHT,
    MOVE_LEFT,
    JUMP,
    EXIT
} EventType;

class IObserver
{
    public:
        virtual ~IObserver(){};
        virtual void Update(EventType event, void* callback) = 0;
};

class ISubject
{
    public:
        virtual ~ISubject(){};
        virtual void Subscribe(EventType event, IObserver* observer) = 0;
        virtual void Unsubscribe(EventType event, IObserver* observer) = 0;
        virtual void Notify(EventType event, void* callback) = 0;
};