#include "menu.h"

#include <stdio.h>

#include "sprites.h"

#include "menu.h"

#include "texture.h"
#include "physics.h"
#include "scene.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define GRID_COLS (SCREEN_WIDTH / TILE_SIZE)
#define GRID_ROWS (SCREEN_HEIGHT / TILE_SIZE)

#define EMPTY_TILE -1

static C3D_RenderTarget* top = NULL;

static C2D_SpriteSheet atlas;

static int level[GRID_ROWS][GRID_COLS];

bool testInit(C3D_RenderTarget* target)
{
    top = target;

    atlas =
        C2D_SpriteSheetLoad("romfs:/gfx/atlas.t3x");

    if (!atlas)
    {
        printf("ERROR: no se pudo cargar atlas.t3x\n");
        return false;
    }

    for (int row = 0; row < GRID_ROWS; row++)
    {
        for (int col = 0; col < GRID_COLS; col++)
        {
            level[row][col] = EMPTY_TILE;
        }
    }
    return true;
}

void testExit(void)
{

    if (atlas)
    {
        C2D_SpriteSheetFree(atlas);
        atlas = NULL;
    }

    top = NULL;
}

void testUpdate(Scene* nextScene, u32 kDown)
{
    if (kDown & KEY_B)
    {
        *nextScene = SCENE_MENU;
    }
}


void testDraw()
{
    C2D_TargetClear(
        top,
        C2D_Color32(20, 20, 40, 255)
    );

    C2D_SceneBegin(top);

    for (int i = 0; i < 6; i++)
    {
        renderAtlasTexture(atlas, 2, i, 0);
    }
    renderAtlasTexture(atlas, atlas_index, pos_x, pos_y);

    C2D_Flush();
}

void loadPhysics()
{
    b2Vec2 gravity(0.0f, 100.0f);
    b2World* world = createWorld(gravity)

    for (int i = 0; i < SCREEN_WIDTH; i+=18)
    {
        loadGroundBox(i, 0, 18, 18);
    }
}