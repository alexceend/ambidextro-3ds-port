#include "texture.h"


C2D_Image getAtlasTexture(C2D_SpriteSheet atlas, int atlas_index)
{
    C2D_Image empty = {0};
    if (!atlas) return empty;

    size_t count = C2D_SpriteSheetCount(atlas);
    if (atlas_index < 0 || atlas_index >= (int)count)
        return empty;
    return C2D_SpriteSheetGetImage(atlas, atlas_index);
}


bool renderTexture(C2D_Image image, int row, int col)
{
    return C2D_DrawImageAt(image, row * TILE_SIZE, col * TILE_SIZE, 0.0f, NULL, 1.0f, 1.0f);
}

bool renderTexturePixel(C2D_Image image, float pos_x, float pos_y)
{
    return C2D_DrawImageAt(image, pos_x, pos_y, 0.0f, NULL, 1.0f, 1.0f);
}
