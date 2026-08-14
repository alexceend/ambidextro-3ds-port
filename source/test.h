#ifndef TEST_H
#define TEST_H

#include <3ds.h>
#include <citro2d.h>
#include <stdbool.h>
#include "scene.h"

#ifdef __cplusplus
extern "C"
{
#endif
    bool testInit(C3D_RenderTarget* target);
    void testUpdate(Scene* nextScene, u32 kDown, u32 kHeld);
    void testDraw(void);
    void testExit(void);
    
#ifdef __cplusplus
}
#endif

#endif