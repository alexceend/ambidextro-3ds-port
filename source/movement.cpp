#include "movement.h"

void applyVelocity(MoveState moveState, b2Body *body, float speed)
{
    b2Vec2 vel = body->GetLinearVelocity();
    switch (moveState)
    {
    case MS_LEFT:
        vel.x = -speed;
        break;
    case MS_RIGHT:
        vel.x = speed;
        break;
    case MS_STOP:
        vel.x = 0;
        break;
    default: break;
    }
    body->SetLinearVelocity(vel);
}

void jump(b2Body *body)
{
    float impulse = body->GetMass() * 4;
    body->ApplyLinearImpulse(b2Vec2(0, -impulse), body->GetWorldCenter(), true);
}


Movement::Movement(ISubject &subject) : subject_(subject)
{
    this->subject_.Subscribe(MOVE_RIGHT, this);
    this->subject_.Subscribe(MOVE_LEFT, this);
    this->subject_.Subscribe(JUMP, this);
}
Movement::~Movement() {}

void Movement::Update(EventType event, void *callback)
{
    switch (event)
    {
        Wizard *wizard;
    case MOVE_RIGHT:
        wizard = (Wizard *)callback;
        applyVelocity(MS_RIGHT, wizard->body, wizard->velocity);
        break;
    case MOVE_LEFT:
        wizard = (Wizard *)callback;
        applyVelocity(MS_LEFT, wizard->body, wizard->velocity);
        break;
    case JUMP:
        wizard = (Wizard *)callback;
        jump(wizard->body);
        break;
    default: break;
    }
}
