#ifndef TRANSITION_H
#define TRANSITION_H

#include <3ds.h>
#include <citro2d.h>
#include "scene.h"

constexpr size_t LEVEL_TEXT_UNITS = 3;
constexpr size_t LAST_LEVEL = 100;
constexpr uint64_t TRANSITION_TIMER = 2000;



float target_time;


bool transitionInit(C3D_RenderTarget *targetTop, C3D_RenderTarget *targetBottom);
Scene transitionUpdate(void);
void transitionDraw(void);
void transitionCleanup(void);

#endif