#ifndef TEXTURE_H
#define TEXTURE_H

#define ATLAS_SIZE 256
#define TILE_SIZE 16

#include <citro2d.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
    #endif
    bool renderAtlasTexture(C2D_SpriteSheet atlas, int atlas_index, int row, int col);
    bool renderAtlasTexturePixel(C2D_SpriteSheet atlas, int atlas_index, float pos_x, float pos_y);
    #ifdef __cplusplus
}
#endif




#endif