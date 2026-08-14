#include "physics.h"
#include <memory>

std::unique_ptr<b2World> world = NULL;
float timeStep = 1.0f / 60.0f;

std::unique_ptr<b2World> createWorld(b2Vec2 gravity)
{
    return std::make_unique<b2World>(gravity);
}

void loadGroundBox(int pos_x, int pos_y, int width, int height)
{
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(pos_x, pos_y);

    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;

    groundBox.SetAsBox(width, height);

    groundBody->CreateFixture(&groundBox, 0.0f);
}

void loadWizardHitbox(float pos_x, float pos_y, Wizard* wizard)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos_x, pos_y);
    b2Body* body = world->CreateBody(&bodyDef);
    wizard->body = body;

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(12.0f, 15.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.8f;

    body->CreateFixture(&fixtureDef);
}

void updatePhysics(Wizard* wizard)
{
    world->Step(timeStep, 6, 2);

    b2Vec2 position = wizard->body->GetPosition();

    wizard->pos_x = position.x;
    wizard->pos_y = position.y;
}