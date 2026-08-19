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
#include <list>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define GRID_COLS (SCREEN_WIDTH / TILE_SIZE)
#define GRID_ROWS (SCREEN_HEIGHT / TILE_SIZE)

#define EMPTY_TILE -1

static C3D_RenderTarget *top = NULL;

static C2D_SpriteSheet atlas_dungeon;
static C2D_SpriteSheet atlas_wizard;

Wizard w = {PURPLE, 12.0f, 15.0f, 3, NULL, 0};
C2D_Image floorImage;
static MoveState moveState = MS_STOP;
std::list<Block*> blockList;

static int level[GRID_ROWS][GRID_COLS];

FooDraw fooDrawInstance;



void loadPhysics()
{
    b2Vec2 gravity(0.0f, 9.8);
    world = createWorld(gravity);
    world->SetContactListener(&contactListener);
    world->SetDebugDraw(&fooDrawInstance);

    for (int i = 0; i < SCREEN_WIDTH / TILE_SIZE; i++)
    {
        // Delete? (memory)
        Block* block = new Block;
        block->row = (SCREEN_HEIGHT / TILE_SIZE) - 2;
        block->col = i;
        block->width = 16.0f;
        block->height = 16.0f;
        blockList.push_front(block);
        loadStaticObject(floorImage, block);
    }
    loadWizardHitbox(150.0f, 0.0f, &w);
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

    floorImage = getAtlasTexture(atlas_dungeon, 2);

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

    printf(w.numFootContacts <= 0 ? "Cannot jump!\n" : "Jump available!\n");

    if (kDown & KEY_B)
    {
        *nextScene = SCENE_MENU;
    }

    move(&w, &moveState, kHeld);
    if (w.numFootContacts >= 1)
    {
        jump(&w, kDown);
    }
}

void FooDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    u32 fillColor = C2D_Color32f(color.r, color.g, color.b, 0.5f);
    u32 lineColor = C2D_Color32f(color.r, color.g, color.b, 1.0f);

    b2Vec2 pixels[b2_maxPolygonVertices];
    for (int32 i = 0; i < vertexCount; i++)
    {
        pixels[i].x = metersToPixels(vertices[i].x);
        pixels[i].y = metersToPixels(vertices[i].y);
    }

    for (int32 i = 1; i < vertexCount - 1; i++)
    {
        C2D_DrawTriangle(
            pixels[0].x, pixels[0].y, fillColor,
            pixels[i].x, pixels[i].y, fillColor,
            pixels[i + 1].x, pixels[i + 1].y, fillColor,
            0.0f
        );
    }

    for (int32 i = 0; i < vertexCount; i++)
    {
        const b2Vec2& a = pixels[i];
        const b2Vec2 b = pixels[(i + 1) % vertexCount];
        C2D_DrawLine(
            a.x, a.y, lineColor,
            b.x, b.y, lineColor,
            1.0f, 0.0f
        );
    }
}

void testDraw()
{
    b2Vec2 playerPosition = w.body->GetPosition();
    float pos_x = metersToPixels(playerPosition.x) - w.width / 2;
    float pos_y = metersToPixels(playerPosition.y) - w.height / 2;
    
    C2D_TargetClear(
        top,
        C2D_Color32(20, 20, 40, 255));

    C2D_SceneBegin(top);
    for (Block* block : blockList)
    {
        C2D_DrawImageAt(floorImage, (float)block->col * TILE_SIZE, (float)block->row * TILE_SIZE, 0.0f, NULL, 1.0f, 1.0f);
    }
    
    renderTexturePixel(getAtlasTexture(atlas_wizard, 1), pos_x, pos_y);

    fooDrawInstance.SetFlags( b2Draw::e_shapeBit);

    world->DebugDraw();

    C2D_Flush();
}