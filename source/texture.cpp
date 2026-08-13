#include "texture.h"


bool renderAtlasTexture(C2D_SpriteSheet atlas, int atlas_index, int row, int col)
{
    if (!atlas) return false;

    size_t count = C2D_SpriteSheetCount(atlas);
    if (atlas_index < 0 || atlas_index >= (int)count)
        return false;
    
    C2D_Image img = C2D_SpriteSheetGetImage(atlas, atlas_index);

    return C2D_DrawImageAt(img, row * TILE_SIZE, col * TILE_SIZE, 0.0f, NULL, 1.0f, 1.0f);
}

