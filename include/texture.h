#ifndef TEXTURE_H
#define TEXTURE_H

#define ATLAS_SIZE 256
#define TILE_SIZE 14

#include <citro2d.h>
#include <stdbool.h>
#include "game_manager.h"

#ifdef __cplusplus
extern "C"
{
    #endif
    C2D_Image getAtlasTexture(C2D_SpriteSheet atlas, int atlas_index);
    bool renderTexture(C2D_Image image, int row, int col);
    bool renderTexturePixel(C2D_Image image, float pos_x, float pos_y);
    #ifdef __cplusplus
}
#endif
template <size_t imageCount> void renderAnimation(const std::array<C2D_Image, imageCount>& images, float pos_x, float pos_y);

class Texture : public IObserver
{
    public:
        Texture(ISubject& subject);
        void Update(EventType event, void* callback);
    private:
        ISubject& subject_;
};

#endif