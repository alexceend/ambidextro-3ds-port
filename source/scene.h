#ifndef SCENE_H
#define SCENE_H

#include <3ds.h>
#include <citro2d.h>
#include <stdbool.h>
typedef enum
{
    SCENE_NONE = -1,
    SCENE_MENU,
    SCENE_TEST
} Scene;

bool sceneInit(Scene* current, C3D_RenderTarget* target);
void sceneUpdate(Scene* current, Scene* nextScene, u32 kDown, u32 kHeld);
void sceneDraw(Scene* current);
void sceneExit(Scene* current);
void sceneChange(Scene* current, Scene* nextScene, C3D_RenderTarget* target);

#endif