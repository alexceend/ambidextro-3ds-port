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
    C2D_Image* getAtlasTexture(C2D_SpriteSheet atlas, int atlas_index);
    bool renderTexture(C2D_Image* image, int row, int col);
    bool renderTexturePixel(C2D_Image* image, float pos_x, float pos_y);
    #ifdef __cplusplus
}
#endif





#endif