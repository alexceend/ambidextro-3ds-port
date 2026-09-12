#include "physics.h"
#include "objects.h"
#include <memory>
#include <inttypes.h>
#include <iostream>
#include <math.h>


std::unique_ptr<b2World> world = NULL;
ContactListener contactListener;
float timeStep = 1.0f / 60.0f;
// RayCastCallback rayCastCallback = {};

enum _entityCategory {
    WIZARD_BITS_ = 0x0001,
    WIZARD_FOOT_BITS_ = 0x0002,
    STAFF_BITS_ = 0x0004,
    GROUND_BITS_ = 0x0008,
};

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
    b2FixtureDef fixtureDef;
    fixtureDef.density = 1.0f;
    fixtureDef.shape = &groundBox;
    fixtureDef.filter.categoryBits = GROUND_BITS_;
    fixtureDef.filter.maskBits = WIZARD_BITS_ | WIZARD_FOOT_BITS_;

    groundBody->CreateFixture(&fixtureDef);
}

void loadWizardFootSensor(float footSensorX, float footSensorY, FootSensor* foot_sensor, b2PolygonShape *dynamicBox, b2FixtureDef *fixtureDef, b2Body *body)
{
    dynamicBox->SetAsBox(
        pixelsToMeters(footSensorX / 2),
        pixelsToMeters(footSensorY / 2),
        b2Vec2(pixelsToMeters(foot_sensor->offset_x), pixelsToMeters(foot_sensor->offset_y)),
        0);

    fixtureDef->isSensor = true;
    fixtureDef->userData.pointer = reinterpret_cast<uintptr_t>(&foot_sensor->entity);
    fixtureDef->filter.categoryBits = WIZARD_FOOT_BITS_;
    fixtureDef->filter.maskBits = GROUND_BITS_ | WIZARD_BITS_ | STAFF_BITS_;
    
    body->CreateFixture(fixtureDef);
}

void loadStaffHitbox(float staff_x, float staff_y, Staff *staff)
{

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = false;
    bodyDef.allowSleep = false;
    bodyDef.position.Set(pixelsToMeters(staff_x), pixelsToMeters(staff_y));
    staff->body = world->CreateBody(&bodyDef);
    b2Body* body = staff->body;


    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(pixelsToMeters(staff->entity.body_properties.width / 2), pixelsToMeters(staff->entity.body_properties.height / 2));

    b2FixtureDef fixtureDef;

    fixtureDef.density = 0.1f;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.friction = .0f;
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(&staff->entity);
    fixtureDef.filter.categoryBits = STAFF_BITS_;
    fixtureDef.filter.maskBits = STAFF_BITS_;

    body->CreateFixture(&fixtureDef);

}

void loadWizardHitbox(float pos_x, float pos_y, Wizard *wizard)
{
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.allowSleep = false;
    bodyDef.position.Set(pixelsToMeters(pos_x), pixelsToMeters(pos_y));
    b2Body *body = world->CreateBody(&bodyDef);
    wizard->body = body;
    
    //WIZARD HITBOX
    b2PolygonShape dynamicBox;

    dynamicBox.SetAsBox(pixelsToMeters(wizard->entity.body_properties.width / 2), pixelsToMeters(wizard->entity.body_properties.height / 2));

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.userData.pointer = reinterpret_cast<uintptr_t>(&wizard->entity);
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.0f;
    fixtureDef.filter.categoryBits = WIZARD_BITS_;
    fixtureDef.filter.maskBits = WIZARD_BITS_ | GROUND_BITS_;

    body->CreateFixture(&fixtureDef);

    // Foot sensor

    loadWizardFootSensor(
        wizard->foot_sensor.entity.body_properties.width,
        wizard->foot_sensor.entity.body_properties.height,
        &wizard->foot_sensor,
        &dynamicBox,
        &fixtureDef,
        body
    );

    // STAFF

    float staff_x_pos = pos_x + wizard->staff.offset_x;
    float staff_y_pos = pos_y + wizard->staff.offset_y;

    loadStaffHitbox(staff_x_pos, staff_y_pos, &wizard->staff);

    //loadWizardFootSensor(wizard->foot_sensor.entity.body_properties.width, wizard->foot_sensor.entity.body_properties.height, &wizard->foot_sensor, &dynamicBox, &fixtureDef, body);
    //loadStaff(staff_x, staff_y, &wizard->staff, &dynamicBox, &fixtureDef, body);
    //loadStaffHitbox(staff_x, staff_y, &wizard->staff);
}

void preSolve(b2Contact *contact)
{
    b2WorldManifold worldManifold;
    contact->GetWorldManifold(&worldManifold);

    const uintptr_t body_a = contact->GetFixtureA()->GetUserData().pointer;
    const uintptr_t body_b = contact->GetFixtureB()->GetUserData().pointer;

    b2Body *wizard_body;
    b2Vec2 normal = worldManifold.normal;
    Entity *entity_a = reinterpret_cast<Entity *>(body_a);
    Entity *entity_b = reinterpret_cast<Entity *>(body_b);

    if (entity_a != nullptr && entity_a->entity_type == WIZARD_)
    {
        Wizard *wizard = static_cast<Wizard *>(entity_a->sub_struct);
        wizard_body = wizard->body;
    }
    else if (entity_b != nullptr && entity_b->entity_type == WIZARD_)
    {
        Wizard *wizard = static_cast<Wizard *>(entity_b->sub_struct);
        wizard_body = wizard->body;
        normal = -normal;
    }
    else
        return;

    if (b2Dot(normal, wizard_body->GetLinearVelocity()) >= 0.0f)
    {
        size_t size = (sizeof(worldManifold.points) / sizeof(*worldManifold.points));
        if (size > 1)
        {
            constexpr float overlap_slop = 4.0f * b2_linearSlop;
            float contact_area = (worldManifold.points[0] - worldManifold.points[1]).Length();

            if (contact_area < overlap_slop)
            {
                contact->SetEnabled(false);
            }
        }
    }
}

void updatePhysics()
{
    world->Step(timeStep, 6, 2);
}

void checkFootSensor(Wizard *wizard, int delta)
{
    wizard->num_foot_contacts += delta;
}

void manageSensorContact(uintptr_t data, bool beginContact)
{
    Entity *entity = reinterpret_cast<Entity *>(data);
    if (entity != nullptr && entity->entity_type == WIZARD_FOOT_)
    {
        FootSensor *wizard_sensor = static_cast<FootSensor*>(entity->sub_struct);
        int delta = beginContact ? 1 : -1;
        checkFootSensor(wizard_sensor->wizard, delta);
    }
}

void ContactListener::BeginContact(b2Contact *contact)
{
    preSolve(contact);
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

float RayCastCallback::ReportFixture(b2Fixture *fixture, const b2Vec2 &point,
                                     const b2Vec2 &normal, float fraction)
{
    m_fixture = fixture;
    m_point = point;
    m_normal = normal;
    m_fraction = fraction;

    if (fixture->IsSensor())
    {
        return -1;
    }
    
    return 0;
}

bool fixtureIsWizard(b2Fixture *fixture)
{
    if (fixture == nullptr)
    {
        return false;
    }
    if (fixture->GetUserData().pointer > 0)
    {
        Entity *entity = reinterpret_cast<Entity *>(fixture->GetUserData().pointer);
        return entity->entity_type == WIZARD_;
    }
    return false;
}
