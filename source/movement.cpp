#include "movement.h"
#include <iostream>

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
    this->subject_.Subscribe(MOVE_STOP, this);
    this->subject_.Subscribe(JUMP, this);
}
Movement::~Movement() {}

void Movement::Update(EventType event, void *callback)
{
    Wizard* wizard = static_cast<Wizard *>(callback);
    switch (event)
    {
    case MOVE_RIGHT:
        applyVelocity(MS_RIGHT, wizard->body, wizard->velocity);
        break;
    case MOVE_LEFT:
        applyVelocity(MS_LEFT, wizard->body, wizard->velocity);
        break;
    case MOVE_STOP:
        wizard = (Wizard *) callback;
        applyVelocity(MS_STOP, wizard->body, wizard->velocity);
        break;
    case JUMP:
        jump(wizard->body);
        break;
    default: break;
    }
}
