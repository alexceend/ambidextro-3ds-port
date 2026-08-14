#include "menu.h"

#include <stdio.h>

#include "sprites.h"

#include "menu.h"

#include "texture.h"
#include "physics.h"
#include "scene.h"
#include "test.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define GRID_COLS (SCREEN_WIDTH / TILE_SIZE)
#define GRID_ROWS (SCREEN_HEIGHT / TILE_SIZE)

#define EMPTY_TILE -1

static C3D_RenderTarget *top = NULL;

static C2D_SpriteSheet atlas_dungeon;
static C2D_SpriteSheet atlas_wizard;

Wizard w = {PURPLE, 0, 0, 10, NULL};

static int level[GRID_ROWS][GRID_COLS];


void loadPhysics()
{
    b2Vec2 gravity(0.0f, 100.0f);
    world = createWorld(gravity);

    for (int i = 0; i < SCREEN_WIDTH; i += 16)
    {
        loadGroundBox(i, 0, 16, 16);
    }

    loadWizardHitbox(w.pos_x, w.pos_y, &w);
}

bool testInit(C3D_RenderTarget *target)
{
    top = target;

    atlas_dungeon =
        C2D_SpriteSheetLoad("romfs:/gfx/atlas.t3x");

    atlas_wizard =
        C2D_SpriteSheetLoad("romfs:/gfx/wizard_atlas.t3x");

    if (!atlas_dungeon)
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

    loadPhysics();

    return true;
}

void testExit(void)
{

    if (atlas_dungeon)
    {
        C2D_SpriteSheetFree(atlas_dungeon);
        atlas_dungeon = NULL;
    }

    top = NULL;
}

void testUpdate(Scene *nextScene, u32 kDown)
{
    updatePhysics(&w);

    if (kDown & KEY_B)
    {
        *nextScene = SCENE_MENU;
    }

    printf("Body Pos: %4.2f, %4.2f\nTouch to position Body!", w.pos_x, w.pos_y);

}

void testDraw()
{
    C2D_TargetClear(
        top,
        C2D_Color32(20, 20, 40, 255));

    C2D_SceneBegin(top);

    for (int i = 0; i < 25; i++)
    {
        renderAtlasTexture(atlas_dungeon, 2, i, 14);
    }

    renderAtlasTexture(atlas_wizard, 1, w.pos_x, w.pos_y);

    C2D_Flush();
}