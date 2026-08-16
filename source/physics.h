#ifndef PHYSICS_H
#define PHYSICS_H

#include <3ds.h>
#include <box2d/box2d.h>
#include <memory>
#include <stdio.h>

#define PIXELS_PER_METER 32.0f


typedef enum
{
    PURPLE,
    YELLOW,
} WizardType;

typedef struct
{
    WizardType wizard_type;
    float velocity;
    b2Body* body;
    int numFootContacts;
} Wizard;


class ContactListener : public b2ContactListener
{
    public:
        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
    private:
        void checkFootSensor(b2Fixture* fixture, int delta);
};

extern ContactListener contactListener;
extern std::unique_ptr<b2World> world;

std::unique_ptr<b2World> createWorld(b2Vec2 gravity);

float pixelsToMeters(float pixels);
float metersToPixels(float meters);

void loadGroundBox(int pos_x, int pos_y, int width, int height);
void loadWizardHitbox(float pos_x, float pos_y, Wizard* wizard);

void updatePhysics(Wizard* wizard);

void loadCharacters();

#endif