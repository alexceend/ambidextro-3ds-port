#ifndef LEVEL_H
#define LEVEL_H

#include <3ds.h>
#include <citro2d.h>
#include <stdbool.h>
#include "scene.h"
#include "game_manager.h"

bool levelInit(C3D_RenderTarget *target);
void levelDraw(void);
void levelUpdate(void);
void levelCleanup(void);

class LevelClass : public IObserver
{
    public:
        LevelClass(ISubject &subject);
        void Update(EventType event, void* callback);
    private:
        ISubject& subject_;
};

#endif