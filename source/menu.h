#include <citro2d.h>
#include <stdbool.h>
#include "scene.h"

#ifndef MENU_H
#define MENU_H

#ifdef __cplusplus
extern "C"
{
#endif
    bool menuInit(C3D_RenderTarget* target);
    Scene menuUpdate(u32 kDown);
    void menuDraw(void);
    void menuExit(void);
#ifdef __cplusplus
}
#endif

#endif