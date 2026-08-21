#include "texture.h"

static C2D_SpriteSheet atlas_dungeon;
static C2D_SpriteSheet atlas_wizard;
//static C2D_SpriteSheet atlas_purple_wizard;
//static C2D_SpriteSheet atlas_yellow_wizard;


void init_assets(){

    atlas_dungeon =
        C2D_SpriteSheetLoad("romfs:/gfx/atlas.t3x");

    atlas_wizard =
        C2D_SpriteSheetLoad("romfs:/gfx/wizard_atlas.t3x");
}