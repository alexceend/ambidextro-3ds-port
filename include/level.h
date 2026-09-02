#ifndef LEVEL_H
#define LEVEL_H

#include <3ds.h>
#include <citro2d.h>
#include <stdbool.h>
#include "scene.h"
#include "game_manager.h"
#include "sprite_animation_manager.h"

extern bool paused;

bool levelInit(C3D_RenderTarget *target);
void levelDraw(void);
Scene levelUpdate(u32 kDown);
void levelCleanup(void);

void restartLevel(C3D_RenderTarget *target);

class LevelClass : public IObserver
{
    public:
        LevelClass(ISubject &subject);
        void Update(EventType event, void* callback);
    private:
        ISubject& subject_;
};

#endif