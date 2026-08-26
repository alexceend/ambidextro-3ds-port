#include <stdio.h>

#include "physics.h"
#include "texture.h"
#include "scene.h"
#include <list>
#include <fstream>
#include <string>
#include <sstream>
#include "assets_loader.h"
#include <iostream>
#include "level.h"
#include <filesystem>
#include <array>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define GRID_COLS (SCREEN_WIDTH / TILE_SIZE)
#define GRID_ROWS (SCREEN_HEIGHT / TILE_SIZE)

#define LEVEL_WIDTH 28
#define LEVEL_HEIGHT 16

#define EMPTY_TILE -1

static C3D_RenderTarget *top = NULL;

FooDraw fooDrawInstance;

typedef enum
{
    TILE_EMPTY = -1,
    TILE_FLOOR_SLAB = 0,
    TILE_FLOOR = 2,
    TILE_WALL = 15,
} TileType;

std::map<TileType, std::array<int, 4>> tile = {
    {TILE_WALL, {14, 14, 0, 0}},
    {TILE_FLOOR_SLAB, {14, 3, 0, 14}},
    {TILE_FLOOR, {14, 14, 0, 0}}
};

typedef struct
{
    int spawnX;
    int spawnY;
} Entity;

typedef struct
{
    int8_t tiles[LEVEL_HEIGHT][LEVEL_WIDTH];
    Entity entities[2];
} Level;

std::list<Block *> blockList;
Level level;
int8_t currentLevel = 1;
bool showDebug = true;

using namespace std;

bool loadLevelFromFile(ifstream *file, Level* level)
{

    if (!file)
    {
        printf("ERROR: Could not open level file\n");
        return false;
    }

    int i = 0;
    string line;

    while (getline(*file, line) && i < LEVEL_HEIGHT)
    {
        std::stringstream ss(line);
        int tile;
        int j = 0;
        while (ss >> tile && j < LEVEL_WIDTH)
        {
            switch (tile)
            {
            case -1:
                level->tiles[i][j] = TILE_EMPTY;
                break;
            case 0:
                level->tiles[i][j] = TILE_FLOOR_SLAB;
                break;
            case 2:
                level->tiles[i][j] = TILE_FLOOR;
                break;
            case 15:
                level->tiles[i][j] = TILE_WALL;
                break;
            default:
                level->tiles[i][j] = TILE_EMPTY;
                break;
            }
            j++;
        }
        i++;
    }
    return i == LEVEL_HEIGHT;
}

void loadPhysics()
{
    b2Vec2 gravity(0.0f, 9.8);
    world = createWorld(gravity);
    world->SetContactListener(&contactListener);
    world->SetDebugDraw(&fooDrawInstance);

    for (int i = 0; i < LEVEL_HEIGHT; i++)
    {
        for (int j = 0; j < LEVEL_WIDTH; j++)
        {
            if (level.tiles[i][j] != TILE_EMPTY)
            {
                Block *block = new Block;
                block->row = i;
                block->col = j;
                TileType type = static_cast<TileType>(level.tiles[i][j]);
                block->width = tile.at(type).at(0);
                block->height = tile.at(type).at(1);
                blockList.push_front(block);
                loadStaticObject(getAtlasTexture
                    (atlas_dungeon, level.tiles[i][j]),
                    block,
                    tile.at(type).at(2),
                    tile.at(type).at(3)
                );
            }
        }
    }

    loadWizardHitbox(level.entities[0].spawnX, level.entities[0].spawnY, &purpleWizard);
    loadWizardHitbox(level.entities[1].spawnX, level.entities[1].spawnY, &yellowWizard);
}

bool levelInit(C3D_RenderTarget *target)
{
    top = target;

    string filePath = "romfs:levels/level" + to_string(currentLevel) + ".txt";
   
    ifstream file(filePath);

    loadLevelFromFile(&file, &level);

    // Set spawn points for entities
    string line;
    while (getline(file, line))
    {
        int x, y;
        if (line.find("spawnPurple") != string::npos)
        {
            sscanf(line.c_str(), "spawnPurple %d %d", &x, &y);
            level.entities[0].spawnX = x;
            level.entities[0].spawnY = y;
        }
        else if (line.find("spawnYellow") != string::npos)
        {
            sscanf(line.c_str(), "spawnYellow %d %d", &x, &y);
            level.entities[1].spawnX = x;
            level.entities[1].spawnY = y;
        }
    }

    loadPhysics();
  
    return true;
}

void levelCleanup()
{
    if (atlas_dungeon)
    {
        C2D_SpriteSheetFree(atlas_dungeon);
        atlas_dungeon = NULL;
    }
    if (atlas_purple_wizard_static)
    {
        C2D_SpriteSheetFree(atlas_purple_wizard_static);
        atlas_purple_wizard_static = NULL;
    }
    if (atlas_purple_wizard_jump)
    {
        C2D_SpriteSheetFree(atlas_purple_wizard_jump);
        atlas_purple_wizard_jump = NULL;
    }
    if (atlas_yellow_wizard_static)
    {
        C2D_SpriteSheetFree(atlas_yellow_wizard_static);
        atlas_yellow_wizard_static = NULL;
    }
    if (atlas_yellow_wizard_jump)
    {
        C2D_SpriteSheetFree(atlas_yellow_wizard_jump);
        atlas_yellow_wizard_jump = NULL;
    }
    if (atlas_staff)
    {
        C2D_SpriteSheetFree(atlas_staff);
        atlas_staff = NULL;
    }
    for (Block *block : blockList)
    {
        delete block;
    }
    blockList.clear();

    if (purpleWizard.body)
    {
        world->DestroyBody(purpleWizard.body);
        purpleWizard.body = NULL;
    }
    if (yellowWizard.body)
    {
        world->DestroyBody(yellowWizard.body);
        yellowWizard.body = NULL;
    }
    if (world)
    {
        world.reset();
    }
    top = NULL;
}

void levelUpdate()
{
    b2Vec2 wizardPurple = purpleWizard.body->GetPosition();
    b2Vec2 wizardYellow = yellowWizard.body->GetPosition();

    purpleWizard.object->position.x = metersToPixels(wizardPurple.x) - purpleWizard.body_properties.width / 2;
    purpleWizard.object->position.y = metersToPixels(wizardPurple.y) - purpleWizard.body_properties.height / 2;
    yellowWizard.object->position.x = metersToPixels(wizardYellow.x)- yellowWizard.body_properties.width / 2;
    yellowWizard.object->position.y = metersToPixels(wizardYellow.y)- yellowWizard.body_properties.height / 2;
    updatePhysics();
}

void FooDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
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
            0.0f);
    }

    for (int32 i = 0; i < vertexCount; i++)
    {
        const b2Vec2 &a = pixels[i];
        const b2Vec2 b = pixels[(i + 1) % vertexCount];
        C2D_DrawLine(
            a.x, a.y, lineColor,
            b.x, b.y, lineColor,
            1.0f, 0.0f);
    }
}

void levelDraw()
{

    C2D_TargetClear(top, C2D_Color32(20, 20, 40, 255));
    C2D_SceneBegin(top);

    for (Block *block : blockList)
    {
        if (level.tiles[block->row][block->col] != TILE_EMPTY)
        {
            C2D_DrawImageAt(
                getAtlasTexture(atlas_dungeon,
                                level.tiles[block->row][block->col]),
                block->col * TILE_SIZE + OFFSET_X,
                block->row * TILE_SIZE + OFFSET_Y,
                0.0f, NULL,
                1.0f, 1.0f);
        }
    }

    draw_sprite(purpleWizard.object, &(purpleWizard.animation_map[purpleWizard.sprite_info.currentAnimation]));
    draw_sprite(yellowWizard.object, &(yellowWizard.animation_map[yellowWizard.sprite_info.currentAnimation]));

    fooDrawInstance.SetFlags(b2Draw::e_shapeBit);
    if (showDebug)
    {
        world->DebugDraw();
    }
    
    C2D_Flush();
}

LevelClass::LevelClass(ISubject &subject) : subject_(subject)
{
    subject.Subscribe(WIN, this);
    subject.Subscribe(PUASE, this);
    subject.Subscribe(DEATH, this);
    subject.Subscribe(DEBUG, this);
}

void LevelClass::Update(EventType event, void* callback)
{
    switch (event)
    {
    case WIN:
        break;
    case PUASE:
        break;
    case DEATH:
        break;
    case DEBUG:
        showDebug == true ? showDebug = false : showDebug = true;
    default: break;
    }
}