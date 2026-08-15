#include "menu.h"

#include <stdio.h>

#include "sprites.h"

#include "menu.h"

#include "texture.h"
#include "physics.h"
#include "scene.h"
#include "test.h"
#include "movement.h"
#include "audio_core.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define GRID_COLS (SCREEN_WIDTH / TILE_SIZE)
#define GRID_ROWS (SCREEN_HEIGHT / TILE_SIZE)

#define EMPTY_TILE -1

static C3D_RenderTarget *top = NULL;

static C2D_SpriteSheet atlas_dungeon;
static C2D_SpriteSheet atlas_wizard;

Wizard w = {PURPLE, 100, NULL, 0};
static MoveState moveState = MS_STOP;

static int level[GRID_ROWS][GRID_COLS];


void loadPhysics()
{
    b2Vec2 gravity(0.0f, 100.0f);
    world = createWorld(gravity);
    //TODO --> Check if this works
     world->SetContactListener(&contactListener);

    for (int i = 0; i < SCREEN_WIDTH; i += 16)
    {
        loadGroundBox(i, SCREEN_HEIGHT, 16, 16);
    }

    loadWizardHitbox(152.0f, 0.0f, &w);
}

bool testInit(C3D_RenderTarget *target)
{
    audioExit();
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
    printf("FIRST Body Pos: %4.2f, %4.2f\n", w.body->GetPosition().x, w.body->GetPosition().y);

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

void testUpdate(Scene *nextScene, u32 kDown, u32 kHeld)
{
    updatePhysics(&w);

    if (kDown & KEY_B)
    {
        *nextScene = SCENE_MENU;
    }

    move(&w, &moveState, kHeld);
    if (w.numFootContacts >= 1)
    {
        jump(&w, kDown);
    }
    
    // printf("Body Pos: %4.2f, %4.2f\n", w.body->GetPosition().x, w.body->GetPosition().y);
    // printf("Can I jump here? %s\n", w.numFootContacts > 0 ? "yes" : "no");

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
    renderAtlasTexturePixel(atlas_wizard, 1, (float)w.body->GetPosition().x, (float)w.body->GetPosition().y);

    C2D_Flush();
}