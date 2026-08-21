#include "scene.h"
#include "menu.h"
#include "test.h"
#include "level.h"
#include "texture.h"
#include "physics.h"

bool sceneInit(Scene* current, C3D_RenderTarget* target)
{
    switch (*current)
    {
        case SCENE_MENU: return menuInit(target); break;
        case SCENE_TEST: return testInit(target); break;
        case SCENE_LEVEL: return levelInit(target); break;
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
        case SCENE_LEVEL: break;
        default: break;
    }
}

void sceneDraw(Scene* current)
{
    switch (*current)
    {
        case SCENE_MENU: menuDraw(); break;
        case SCENE_TEST: testDraw(); break;
        case SCENE_LEVEL: levelDraw(); break;
        default: break;
    }
}

void sceneExit(Scene* current)
{
    switch (*current)
    {
        case SCENE_MENU: menuExit(); break;
        case SCENE_TEST: testExit(); break;
        case SCENE_LEVEL: levelCleanup(); break;
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

void loadStaticObject(C2D_Image img, Block* block)
{
    loadGroundBox(block->col * TILE_SIZE, block->row * TILE_SIZE, block->width, block->height);
}