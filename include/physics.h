#ifndef PHYSICS_H
#define PHYSICS_H

#include <3ds.h>
#include <box2d/box2d.h>
#include <memory>
#include <stdio.h>
#include "objects.h"
#include <array>
#include "game_constants.h"
#include "game_structs.h"

#define DEG_TO_RAD(degrees) (degrees * M_PI / 180)


struct Wizard;
struct Staff;


class ContactListener : public b2ContactListener
{
    public:
        void BeginContact(b2Contact* contact) override;
        void EndContact(b2Contact* contact) override;
    private:
        void checkFootSensor(b2Fixture* fixture, int delta);
};

class RayCastCallback : public b2RayCastCallback
{
  private:
    
  public:
    float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
									const b2Vec2& normal, float fraction) override;

    b2Body* ignoredBody = nullptr;
    b2Fixture* m_fixture;
    b2Vec2 m_point;
    b2Vec2 m_normal;
    float m_fraction;
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
void loadStaff(float staff_x, float staff_y, Staff* staff, b2PolygonShape* dynamicBox, b2FixtureDef* fixtureDef, b2Body* body);
void loadStaffHitbox(float staff_x, float staff_y, Staff *staff);

void updatePhysics();

void loadCharacters();

std::array<Segment, CIRCLE_STEPS> circularRayCast(b2Vec2 p1, float radius);

bool fixtureIsWizard(b2Fixture *fixture);



#endif