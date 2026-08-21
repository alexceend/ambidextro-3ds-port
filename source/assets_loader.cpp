#include "texture.h"
#include "assets_loader.h"

C2D_SpriteSheet atlas_dungeon = NULL;
C2D_SpriteSheet atlas_wizard = NULL;

bool loadAssets()
{
    atlas_dungeon = C2D_SpriteSheetLoad("romfs:/gfx/atlas.t3x");
    atlas_wizard = C2D_SpriteSheetLoad("romfs:/gfx/wizard_atlas.t3x");

    if (!atlas_dungeon)
    {
        printf("ERROR: Failed to load dungeon atlas\n");
        return false;
    }

    if (!atlas_wizard)
    {
        printf("ERROR: Failed to load wizard atlas\n");
        return false;
    }

    printf("Dungeon atlas: %p\n", (void*)atlas_dungeon);
    printf("Wizard atlas: %p\n", (void*)atlas_wizard);

    return true;
}