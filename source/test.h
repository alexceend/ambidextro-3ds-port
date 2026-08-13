#ifndef TEST_H
#define TEST_H

#include <3ds.h>
#include <citro2d.h>
#include <stdbool.h>
#include "scene.h"

bool testInit(C3D_RenderTarget* target);
Scene testUpdate(u32 kDown);
void testDraw(void);
void testExit(void);

#endif