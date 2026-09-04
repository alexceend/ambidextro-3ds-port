#include "physics.h"
#include "objects.h"
#include <memory>
#include <inttypes.h>

#define CIRCLE_STEPS 360



std::unique_ptr<b2World> world = NULL;
ContactListener contactListener;
float timeStep = 1.0f / 60.0f;
RayCastCallback rayCastCallback = {};

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

void loadGroundBox(int pos_x, int pos_y, int width, int height, int offset_x, int offset_y)
{
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(pixelsToMeters((pos_x + width / 2) + offset_x), pixelsToMeters((pos_y + height / 2) + offset_y));

    b2Body *groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;

    groundBox.SetAsBox(pixelsToMeters(width / 2.0f), pixelsToMeters(height / 2.0f));
    
    groundBody->CreateFixture(&groundBox, 1.0f);
}

void loadWizardHitbox(float pos_x, float pos_y, Wizard *wizard)
{   
    float footSensorX = wizard->entity.body_properties.width / 4;
    float footSensorY = 2.0f;
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(pixelsToMeters(pos_x), pixelsToMeters(pos_y));
    b2Body *body = world->CreateBody(&bodyDef);
    wizard->body = body;

    b2PolygonShape dynamicBox;

    dynamicBox.SetAsBox(pixelsToMeters(wizard->entity.body_properties.width / 2), pixelsToMeters(wizard->entity.body_properties.height / 2));

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;

    body->CreateFixture(&fixtureDef);

    dynamicBox.SetAsBox(
        pixelsToMeters(footSensorX), 
        pixelsToMeters(footSensorY), 
        b2Vec2(0, pixelsToMeters((wizard->entity.body_properties.height / 2) - footSensorY)), 
        0
    );

    fixtureDef.isSensor = true;
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(wizard);
    body->CreateFixture(&fixtureDef);
}


void loadStaff(float pos_x, float pos_y, Staff* staff)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position.Set(pixelsToMeters(pos_x), pixelsToMeters(pos_y));
    b2Body* body = world->CreateBody(&bodyDef);
    staff->body = body;

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(pixelsToMeters(staff->entity.body_properties.width / 2), pixelsToMeters(staff->entity.body_properties.height / 2));
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.isSensor = true;

    body->CreateFixture(&fixtureDef);
}


void updatePhysics()
{
    world->Step(timeStep, 6, 2);
}

void checkFootSensor(Wizard* wizard, int delta)
{
    wizard->num_foot_contacts += delta;
}

void manageSensorContact(uintptr_t data, bool beginContact)
{
    Entity* entity = reinterpret_cast<Entity*>(data);
    if (entity->entity_type == WIZARD_)
    {
        Wizard* wizard = (Wizard*)entity->sub_struct;
        int delta = beginContact ? 1: -1;
        checkFootSensor(wizard, delta);
    }
}
  
void ContactListener::BeginContact(b2Contact *contact)
{
    uintptr_t data_A = contact->GetFixtureA()->GetUserData().pointer;
    uintptr_t data_B = contact->GetFixtureB()->GetUserData().pointer;
    manageSensorContact(data_A, true);
    manageSensorContact(data_B, true);
}

void ContactListener::EndContact(b2Contact *contact)
{
    uintptr_t data_A = contact->GetFixtureA()->GetUserData().pointer;
    uintptr_t data_B = contact->GetFixtureB()->GetUserData().pointer;
    manageSensorContact(data_A, false);
    manageSensorContact(data_B, false);
}

float RayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point,
									const b2Vec2& normal, float fraction)
{
    return 0;
}



Segment* circularRayCast(b2Vec2 p1, int radius)
{
    Segment segments[CIRCLE_STEPS];
    for (int i = 0; i < CIRCLE_STEPS; i++)
    {
        b2RayCastInput input;
        b2Vec2 p2 = p1 + radius * b2Vec2(sinf(i), cosf(i));
        input.p1 = p1;
        input.p2 = p2;
        input.maxFraction = 1;

        world->RayCast(&rayCastCallback, p1, p2);
        segments[i] = {p1, rayCastCallback.m_point};
    }
    return segments;
}