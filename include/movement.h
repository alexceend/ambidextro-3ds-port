#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <3ds.h>
#include <box2d/box2d.h>
#include <memory>
#include <stdio.h>
#include "physics.h"
#include "game_manager.h"

typedef enum
{
    MS_STOP,
    MS_LEFT,
    MS_RIGHT
} MoveState;

void jump(b2Body* body);

class Movement : public IObserver
{
    public:
        Movement(ISubject &subject);
        virtual ~Movement();

        void Update(EventType event, void* callback);
    private:
        ISubject& subject_;
};
#endif


