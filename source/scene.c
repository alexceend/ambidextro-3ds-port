#include "scene.h"
#include "menu.h"
#include "test.h"

bool sceneInit(Scene* current, C3D_RenderTarget* target)
{
    switch (*current)
    {
        case SCENE_MENU: return menuInit(target); break;
        case SCENE_TEST: return testInit(target); break;
        default: break;
    }
    return false;
}

void sceneUpdate(Scene* current, Scene* nextScene, u32 kDown, u32 kHeld)
{
    switch (*current)
    {
        case SCENE_MENU: *nextScene = menuUpdate(kDown); break;
        case SCENE_TEST: testUpdate(nextScene, kDown, kHeld); break;
        default: break;
    }
}

void sceneDraw(Scene* current)
{
    switch (*current)
    {
        case SCENE_MENU: menuDraw(); break;
        case SCENE_TEST: testDraw(); break;
        default: break;
    }
}

void sceneExit(Scene* current)
{
    switch (*current)
    {
        case SCENE_MENU: menuExit(); break;
        case SCENE_TEST: testExit(); break;
        default: break;
    }
}

void sceneChange(Scene* current, Scene* nextScene, C3D_RenderTarget* target)
{
    if (*nextScene != SCENE_NONE && *nextScene != *current)
        {
            sceneInit(nextScene, target);

            switch (*current)
            {
                case SCENE_MENU: menuExit(); break;
                case SCENE_TEST: testExit(); break;
                default: break;
            }
            sceneExit(current);
            *current = *nextScene;
        }
}