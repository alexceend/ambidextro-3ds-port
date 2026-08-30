#include "scene.h"
#include "menu.h"
#include "level.h"
#include "texture.h"
#include "physics.h"
#include "pause.h"

bool sceneInit(Scene* current, C3D_RenderTarget* target)
{
    switch (*current)
    {
        case SCENE_MENU: return menuInit(target); break;
        case SCENE_LEVEL: return levelInit(target); break;
        case SCENE_PAUSE: return pauseInit(target); break;
        default: break;
    }
    return false;
}

void sceneUpdate(Scene* current, Scene* nextScene, u32 kDown)
{
    switch (*current)
    {
        case SCENE_MENU:
            *nextScene = menuUpdate(kDown);
            break;

        case SCENE_LEVEL:
            if (kDown & KEY_START)
            {
                *nextScene = SCENE_PAUSE;
            }
            else
            {
                levelUpdate();
            }
            break;

        case SCENE_PAUSE:
            *nextScene = pauseUpdate(kDown);
            break;

        default:
            break;
    }
}

void sceneDraw(Scene* current)
{
    switch (*current)
    {
        case SCENE_MENU: menuDraw(); break;
        case SCENE_LEVEL: levelDraw(); break;
        case SCENE_PAUSE: pauseDraw(); break;
        default: break;
    }
}

void sceneExit(Scene* current)
{
    switch (*current)
    {
        case SCENE_MENU: menuExit(); break;
        case SCENE_LEVEL: levelCleanup(); break;
        case SCENE_PAUSE: pauseExit(); break;
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
                case SCENE_LEVEL: levelCleanup(); break;
                case SCENE_PAUSE: pauseExit(); break;
                default: break;
            }
            sceneExit(current);
            *current = *nextScene;
        }
}

void loadStaticObject(C2D_Image img, Block* block, int offset_x, int offset_y)
{
    loadGroundBox(block->col * TILE_SIZE + OFFSET_X, block->row * TILE_SIZE + OFFSET_Y, block->width, block->height, offset_x, offset_y);
}