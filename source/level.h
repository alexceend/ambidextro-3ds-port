#ifndef LEVEL_H
#define LEVEL_H

#include <3ds.h>
#include <citro2d.h>
#include <stdbool.h>
#include "scene.h"

#ifdef __cplusplus
extern "C"
{
#endif
    bool levelInit(C3D_RenderTarget* target);
    void levelDraw(void);
    void levelCleanup(void);
    
#ifdef __cplusplus
}
#endif

#endif