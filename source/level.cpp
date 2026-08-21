#include <stdio.h>

#include "sprites.h"
#include "physics.h"
#include "texture.h"
#include "scene.h"
#include <list>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define GRID_COLS (SCREEN_WIDTH / TILE_SIZE)
#define GRID_ROWS (SCREEN_HEIGHT / TILE_SIZE)

#define EMPTY_TILE -1

static C3D_RenderTarget* top = NULL;

static C2D_SpriteSheet currentAtlas;
static C2D_SpriteSheet atlas_purple_wizard;
static C2D_SpriteSheet atlas_yellow_wizard;


bool testInit(C3D_RenderTarget* target)
{
    
}

void loadPhysics()
{
    b2Vec2 gravity(0.0f, 9.8);
    world = createWorld(gravity);
    world->SetContactListener(&contactListener);
    world->SetDebugDraw(&fooDrawInstance);

    for (int i = 0; i < SCREEN_WIDTH / TILE_SIZE; i++)
    {
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