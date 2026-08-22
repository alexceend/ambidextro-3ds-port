#include "texture.h"
#include "assets_loader.h"

C2D_SpriteSheet atlas_dungeon = NULL;
C2D_SpriteSheet atlas_purple_wizard = NULL;
C2D_SpriteSheet atlas_yellow_wizard = NULL;
C2D_SpriteSheet atlas_staff = NULL;

bool loadAssets()
{
    atlas_dungeon = C2D_SpriteSheetLoad("romfs:/gfx/atlas.t3x");
    atlas_purple_wizard = C2D_SpriteSheetLoad("romfs:/gfx/purple_wizard_atlas.t3x");
    atlas_yellow_wizard = C2D_SpriteSheetLoad("romfs:/gfx/yellow_wizard_atlas.t3x");
    atlas_staff = C2D_SpriteSheetLoad("romfs:/gfx/staff_atlas.t3x");

    if (!atlas_dungeon)
    {
        printf("ERROR: Failed to load dungeon atlas\n");
        return false;
    }

    if (!atlas_purple_wizard)
    {
        printf("ERROR: Failed to load purple wizard atlas\n");
        return false;
    }

    if (!atlas_yellow_wizard)
    {
        printf("ERROR: Failed to load yellow wizard atlas\n");
        return false;
    }

    if (!atlas_staff)
    {
        printf("ERROR: Failed to load staff atlas\n");
        return false;
    }

    return true;
}