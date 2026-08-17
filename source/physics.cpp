#include "physics.h"
#include <memory>

std::unique_ptr<b2World> world = NULL;
ContactListener contactListener;
float timeStep = 1.0f / 60.0f;

float pixelsToMeters(float pixels)
{
    return pixels / PIXELS_PER_METER;
}

float metersToPixels(float meters)
{
    return meters * PIXELS_PER_METER;
}

std::unique_ptr<b2World> createWorld(b2Vec2 gravity)
{
    return std::make_unique<b2World>(gravity);
}

void loadGroundBox(int pos_x, int pos_y, int width, int height)
{
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(pixelsToMeters(pos_x + width / 2), pixelsToMeters(pos_y + height / 2));

    b2Body *groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;

    groundBox.SetAsBox(pixelsToMeters(width / 2.0f), pixelsToMeters(height / 2.0f));

    groundBody->CreateFixture(&groundBox, 1.0f);
    printf("Hitbox at x -> %.3f | y -> %.3f\n", metersToPixels(groundBody->GetPosition().x), metersToPixels(groundBody->GetPosition().y));
}

void loadWizardHitbox(float pos_x, float pos_y, Wizard *wizard)
{
    float width = (pixelsToMeters(12.0f));
    float height = (pixelsToMeters(15.0f));
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(pixelsToMeters(pos_x + 6.0f), pixelsToMeters(pos_y + 7.5f));
    b2Body *body = world->CreateBody(&bodyDef);
    wizard->body = body;

    b2PolygonShape dynamicBox;

    dynamicBox.SetAsBox(width / 2, height / 2);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;

    body->CreateFixture(&fixtureDef);

    dynamicBox.SetAsBox(pixelsToMeters(5.0f), pixelsToMeters(2.0f), b2Vec2(0, pixelsToMeters(7.5f)), 0);
    fixtureDef.isSensor = true;
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(wizard);
    b2Fixture *footSensorFixture = body->CreateFixture(&fixtureDef);

    printf("Wizard addr: %lu, sensor userData right after creation: %lu\n",
       (unsigned long)wizard,
       (unsigned long)footSensorFixture->GetUserData().pointer);
}

void updatePhysics(Wizard *wizard)
{
    world->Step(timeStep, 6, 2);
}

void ContactListener::checkFootSensor(b2Fixture* fixture, int delta)
{
    uintptr_t data = fixture->GetUserData().pointer;
    printf("Fixture Data: %lu\n", (unsigned long)data);
    if (data != 0)
    {
        Wizard* wizard = reinterpret_cast<Wizard*>(data);
        wizard->numFootContacts += delta;
    }
}

void ContactListener::BeginContact(b2Contact *contact)
{
    checkFootSensor(contact->GetFixtureA(), 1);
    checkFootSensor(contact->GetFixtureB(), 1);
}

void ContactListener::EndContact(b2Contact *contact)
{
    checkFootSensor(contact->GetFixtureA(), -1);
    checkFootSensor(contact->GetFixtureB(), -1);
}