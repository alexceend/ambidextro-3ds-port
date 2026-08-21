#ifndef LEVEL_H
#define LEVEL_H

#include <3ds.h>
#include <citro2d.h>
#include <stdbool.h>
#include "scene.h"

bool levelInit(C3D_RenderTarget *target);
void levelDraw(void);
void levelCleanup(void);

#endif