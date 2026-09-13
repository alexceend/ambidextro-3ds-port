#include "texture.h"
#include "assets_loader.h"

C2D_SpriteSheet atlas_dungeon = NULL;
C2D_SpriteSheet atlas_purple_wizard_static = NULL;
C2D_SpriteSheet atlas_purple_wizard_jump = NULL;
C2D_SpriteSheet atlas_yellow_wizard_static = NULL;
C2D_SpriteSheet atlas_yellow_wizard_jump = NULL;
C2D_SpriteSheet atlas_staff_static = NULL;
C2D_SpriteSheet atlas_staff_jump = NULL;

bool loadAssets()
{
    atlas_dungeon = C2D_SpriteSheetLoad("romfs:/gfx/atlas.t3x");
    atlas_purple_wizard_static = C2D_SpriteSheetLoad("romfs:/gfx/purple_wizard_static_atlas.t3x");
    atlas_purple_wizard_jump = C2D_SpriteSheetLoad("romfs:/gfx/purple_wizard_jump_atlas.t3x");
    atlas_yellow_wizard_static = C2D_SpriteSheetLoad("romfs:/gfx/yellow_wizard_static_atlas.t3x");
    atlas_yellow_wizard_jump = C2D_SpriteSheetLoad("romfs:/gfx/yellow_wizard_jump_atlas.t3x");
    atlas_staff_static = C2D_SpriteSheetLoad("romfs:/gfx/staff_static_atlas.t3x");
    atlas_staff_jump = C2D_SpriteSheetLoad("romfs:/gfx/staff_jump_atlas.t3x");

    if (!atlas_dungeon)
    {
        printf("ERROR: Failed to load dungeon atlas\n");
        return false;
    }

    if (!atlas_purple_wizard_jump)
    {
        printf("ERROR: Failed to load purple wizard jump atlas\n");
        return false;
    }

    if (!atlas_purple_wizard_static)
    {
        printf("ERROR: Failed to load purple wizard static atlas\n");
        return false;
    }

    if (!atlas_yellow_wizard_jump)
    {
        printf("ERROR: Failed to load yellow wizard jump atlas\n");
        return false;
    }

    if (!atlas_yellow_wizard_static)
    {
        printf("ERROR: Failed to load yellow wizard static atlas\n");
        return false;
    }

    if (!atlas_staff_static)
    {
        printf("ERROR: Failed to load staff static atlas\n");
        return false;
    }

    if (!atlas_staff_jump)
    {
        printf("ERROR: Failed to load staff jump atlas\n");
        return false;
    }

    return true;
}

void clearAssets()
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
    if (atlas_staff_static)
    {
        C2D_SpriteSheetFree(atlas_staff_static);
        atlas_staff_static = NULL;
    }
    if (atlas_staff_jump)
    {
        C2D_SpriteSheetFree(atlas_staff_jump);
        atlas_staff_jump = NULL;
    }
}