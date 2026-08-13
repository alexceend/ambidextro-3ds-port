#ifndef MENU_H
#define MENU_H

#include <3ds.h>
#include <citro2d.h>
#include <stdbool.h>
#include "scene.h"


bool menuInit(C3D_RenderTarget* target);
Scene menuUpdate(u32 kDown);
void menuDraw(void);
void menuExit(void);

#endif