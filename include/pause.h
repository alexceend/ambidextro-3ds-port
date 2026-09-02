#include <citro2d.h>
#include <stdbool.h>
#include "scene.h"

#ifndef PAUSE_H
#define PAUSE_H

bool pauseInit(C3D_RenderTarget* target);
Scene pauseUpdate(u32 kDown);
void pauseDraw(void);
void pauseExit(void);

#endif