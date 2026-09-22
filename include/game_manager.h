#ifndef GAME_MANAGER_H

#define GAME_MANAGER_H

#include "objects.h"
#include "physics.h"
#include <list>
#include <map>
#include <string>

#define WIZARD_HEIGHT 13.0f
#define WIZARD_WIDTH 11.0f
#define WIZARD_SPEED 130.0f
#define STAFF_HEIGHT 13.0f
#define STAFF_WIDTH 3.0f
#define GRAVITY b2Vec2(0.0f, 9.8f)
#define WIZARD_ANIMATIONS 2
#define STAFF_ANIMATIONS 2

extern Wizard purpleWizard;
extern Wizard yellowWizard;

extern uint8_t currentLevel;
extern int retries;

#endif
