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
    float width;
    float height;
    float velocity;
    b2Body* body;
    int numFootContacts;
    size_t move_frames = 5;
    size_t jump_frames = 6;
    size_t current_step = 0;
    float pos[2] = {
        metersToPixels(body->GetPosition().x) - width / 2 ,
        metersToPixels(body->GetPosition().y) - height / 2
    };
} Wizard;


class ContactListener : public b2ContactListener
{
    public:
        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
    private:
        void checkFootSensor(b2Fixture* fixture, int delta);
};

class FooDraw : public b2Draw
  {
  public:
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {}
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {}
    void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {}
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}
    void DrawTransform(const b2Transform& xf) {}
    void DrawPoint(const b2Vec2& p, float size, const b2Color& color) {}
  };

extern ContactListener contactListener;
extern std::unique_ptr<b2World> world;

std::unique_ptr<b2World> createWorld(b2Vec2 gravity);

float pixelsToMeters(float pixels);
float metersToPixels(float meters);

void loadGroundBox(int pos_x, int pos_y, int width, int height, int offset_x, int offset_y);
void loadWizardHitbox(float pos_x, float pos_y, Wizard* wizard);

void updatePhysics();

void loadCharacters();

#endif