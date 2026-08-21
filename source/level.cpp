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
    uint8_t tiles[16][28];
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
                block->row = i;
                block->col = j;
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
    
    while(getline(file, line) && i < 16){
        std::stringstream ss(line);
        int tile;
        int j = 0;
        while(ss >> tile && j < 28){
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