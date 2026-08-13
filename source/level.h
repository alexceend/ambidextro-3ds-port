#ifndef LEVEL_H
#define LEVEL_H

#include <citro2d.h>
#include <3ds.h>
#include <stdbool.h>
#include "scene.h"

uint8_t tiles[];

bool levelInit(C3D_RenderTarget* target, int levelIndex);
SceneId levelUpdate(u32 kDown);
void levelDraw(void);
void levelExit(void);

#endif