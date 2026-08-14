#include "movement.h"

void applyVelocity(MoveState* moveState, b2Body* body, float speed)
{
    b2Vec2 vel = body->GetLinearVelocity();
    switch (*moveState)
    {
        case MS_LEFT:   vel.x = -speed; break;
        case MS_RIGHT:  vel.x =  speed; break;     
        case MS_STOP:   vel.x =      0; break;
    }
    body->SetLinearVelocity(vel);
}

void move(Wizard* wizard, MoveState* moveState, u32 kHeld)
{
    if (kHeld & KEY_LEFT) *moveState = MS_LEFT;
    else if (kHeld & KEY_RIGHT) *moveState = MS_RIGHT;
    else *moveState = MS_STOP;

    applyVelocity(moveState, wizard->body, wizard->velocity);
}