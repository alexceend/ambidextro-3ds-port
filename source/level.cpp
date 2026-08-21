#include <stdio.h>

#include "sprites.h"
#include "physics.h"
#include "texture.h"
#include "scene.h"
#include <list>
#include <fstream>
#include <string> 
#include <sstream>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define GRID_COLS (SCREEN_WIDTH / TILE_SIZE)
#define GRID_ROWS (SCREEN_HEIGHT / TILE_SIZE)

#define LEVEL_WIDTH 28
#define LEVEL_HEIGHT 16

#define OFFSET_X 4;
#define OFFSET_Y 8;


#define EMPTY_TILE -1

static C3D_RenderTarget* top = NULL;

static C2D_SpriteSheet currentAtlas;
static C2D_SpriteSheet atlas_purple_wizard;
static C2D_SpriteSheet atlas_yellow_wizard;

typedef enum {
    TILE_EMPTY = -1,
    TILE_FLOOR_SLAB = 0,
    TILE_WALL = 15,
} TileType;

typedef struct {
    int spawnX;
    int spawnY;
} Entity;

typedef struct {
    uint8_t tiles[LEVEL_HEIGHT][LEVEL_WIDTH];
    Entity entities[2];
} Level;

Wizard purpleWizard = {PURPLE, 12.0f, 15.0f, 3, NULL, 0};
Wizard yellowWizard = {YELLOW, 12.0f, 15.0f, 3, NULL, 0};

bool levelInit(C3D_RenderTarget* target, int levelIndex)
{
    audioExit();
    top = target;

    atlas_dungeon =
        C2D_SpriteSheetLoad("romfs:/gfx/atlas.t3x");

    atlas_wizard =
        C2D_SpriteSheetLoad("romfs:/gfx/wizard_atlas.t3x");

    Level level;
    loadLevelFromFile("romfs:/levels/level1.txt", level);

    // Set spawn points for entities
    while (getline(file, line)) {
        if (line.find("spawnPurple") != string::npos) {
            int x, y;
            sscanf(line.c_str(), "spawnPurple %d %d", &x, &y);
            level.entities[0].spawnX = x;
            level.entities[0].spawnY = y;
        }else if (line.find("spawnYellow") != string::npos) {
            int x, y;
            sscanf(line.c_str(), "spawnYellow %d %d", &x, &y);
            level.entities[1].spawnX = x;
            level.entities[1].spawnY = y;
        }
    }

    return true;
}

void loadPhysics()
{
    b2Vec2 gravity(0.0f, 9.8);
    world = createWorld(gravity);
    world->SetContactListener(&contactListener);
    world->SetDebugDraw(&fooDrawInstance);

    for (int i = 0; i < GRID_ROWS; i++)
    {
        for (int j = 0; j < GRID_COLS; j++)
        {
            if (level.tiles[i][j] != TILE_EMPTY)
            {
                Block* block = new Block;
                block->row = i + OFFSET_Y;
                block->col = j + OFFSET_X;
                block->width = 14.0f;
                block->height = 14.0f;
                blockList.push_front(block);
                loadStaticObject(getAtlasTexture(atlas_dungeon, level.tiles[i][j]), block);
            }
        }
    }

    loadWizardHitbox(level.entities[0], 0.0f, purpleWizard);
    loadWizardHitbox(level.entities[1], 0.0f, yellowWizard);
}


bool loadLevelFromFile(const std::string& filename, Level& level) {

    ifstream file("romfs:/levels/level" + to_string(levelIndex) + ".txt");

    if(!file){
        printf("ERROR: Could not open level file\n");
        return false;
    }
    
    while(getline(file, line) && i < LEVEL_HEIGHT){
        std::stringstream ss(line);
        int tile;
        int j = 0;
        while(ss >> tile && j < LEVEL_WIDTH){
            switch (tile){
                case -1:
                    level.tiles[i][j] = TILE_EMPTY;
                    break;
                case 0:
                    level.tiles[i][j] = TILE_FLOOR_SLAB;
                    break;
                case 15:
                    level.tiles[i][j] = TILE_WALL;
                    break;
                default:
                    level.tiles[i][j] = TILE_EMPTY;
                    break;
            }
            j++;
        }
    }
}

void levelCleanup() {
    if(atlas_dungeon) {
        C2D_SpriteSheetFree(atlas_dungeon);
        atlas_dungeon = NULL;
    }
    if(atlas_wizard) {
        C2D_SpriteSheetFree(atlas_wizard);
        atlas_wizard = NULL;
    }
    for (Block* block : blockList) {
        delete block;
    }
    blockList.clear();

    if (purpleWizard.body) {
        world->DestroyBody(purpleWizard.body);
        purpleWizard.body = NULL;
    }
    if (yellowWizard.body) {
        world->DestroyBody(yellowWizard.body);
        yellowWizard.body = NULL;
    }
    if (world) {
        world.reset();
    }
    top = NULL;
}

void levelUpdate(Scene* nextScene, u32 kDown, u32 kHeld) {
    updatePhysics(&purpleWizard);
    updatePhysics(&yellowWizard);

    if(kDown & KEY_B) {
        *nextScene = SCENE_MENU;
    }

    move(&purpleWizard, &moveState, kHeld);
    if(purpleWizard.numFootContacts >= 1) {
        jump(&purpleWizard, kDown);
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

void levelDraw(){
    b2Vec2 wizardPurple = purpleWizard.body->GetPosition();
    b2Vec2 wizardYellow = yellowWizard.body->GetPosition();

    float wizardPurplePos[2] = {metersToPixels(wizardPurple.x), metersToPixels(wizardPurple.y)};
    float wizardYellowPos[2] = {metersToPixels(wizardYellow.x), metersToPixels(wizardYellow.y)};

    C2D_TargetClear(top, C2D_Color32(20,20,40,255));
    C2D_SceneBegin(top);

    for(Block* block : blockList) {
        C2D_DrawImageAt(
            getAtlasTexture(atlas_dungeon, 
                level.tiles[block->row - OFFSET_Y][block->col - OFFSET_X]), 
                block->col * TILE_SIZE, 
                block->row * TILE_SIZE, 
                0.0f, NULL,
                1.0f, 1.0f);
    }

    renderTexturePixel(getAtlasTexture(atlas_wizard, 1), wizardPurplePos[0], wizardPurplePos[1]);
    renderTexturePixel(getAtlasTexture(atlas_wizard, 1), wizardYellowPos[0], wizardYellowPos[1]);

    fooDrawInstance.setFlags(b2Draw::e_shapeBit);
    world->DebugDraw();

    C2D_Flush();
}   