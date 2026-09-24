#ifndef TRANSITION_H
#define TRANSITION_H

#include "scene.h"
#include <3ds.h>
#include <citro2d.h>

constexpr size_t LEVEL_TEXT_UNITS = 3;
constexpr size_t LAST_LEVEL = 100;
constexpr uint64_t TRANSITION_TIMER = 200000;

bool transitionInit(C3D_RenderTarget *targetTop, C3D_RenderTarget *targetBottom,
                    uint8_t currentLevel, uint8_t nextLevel);
Scene transitionUpdate(void);
void transitionDraw(void);
void transitionCleanup(void);

#endif
