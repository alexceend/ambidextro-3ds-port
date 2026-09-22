#include "scene.h"
#include "level.h"
#include "menu.h"
#include "pause.h"
#include "physics.h"
#include "texture.h"
#include "transition.h"

bool sceneInit(Scene *current, C3D_RenderTarget *targetTop,
               C3D_RenderTarget *targetBottom) {
  switch (*current) {
  case SCENE_MENU:
    return menuInit(targetTop, targetBottom);
    break;
  case SCENE_LEVEL:
    return levelInit(targetTop, targetBottom);
    break;
  // case SCENE_PAUSE: return pauseInit(targetTop, targetBottom); break;
  case SCENE_TRANSITION:
    return transitionInit(targetTop, targetBottom, currentLevel,
                          currentLevel + 1);
    break;
  default:
    break;
  }
  return false;
}

void sceneUpdate(Scene *current, Scene *nextScene, u32 kDown) {
  switch (*current) {
  case SCENE_MENU:
    *nextScene = menuUpdate(kDown);
    break;
  case SCENE_LEVEL:
    *nextScene = levelUpdate(kDown);
    break;
  // case SCENE_PAUSE: *nextScene = pauseUpdate(kDown); break;
  case SCENE_TRANSITION:
    *nextScene = transitionUpdate();
    break;
  default:
    break;
  }
}

void sceneDraw(Scene *current) {
  switch (*current) {
  case SCENE_MENU:
    menuDraw();
    break;
  case SCENE_LEVEL:
    levelDraw();
    break;
  // case SCENE_PAUSE: pauseDraw(); break;
  case SCENE_TRANSITION:
    transitionDraw();
    break;
  default:
    break;
  }
}

void sceneExit(Scene *current) {
  switch (*current) {
  case SCENE_MENU:
    menuExit();
    break;
  case SCENE_LEVEL:
    levelCleanup();
    break;
  // case SCENE_PAUSE: pauseExit(); break;
  case SCENE_TRANSITION:
    transitionCleanup();
    break;
  default:
    break;
  }
}

void sceneChange(Scene *current, Scene *nextScene, C3D_RenderTarget *targetTop,
                 C3D_RenderTarget *targetBottom) {
  if (*nextScene != SCENE_NONE && *nextScene != *current) {
    sceneInit(nextScene, targetTop, targetBottom);

    switch (*current) {
    case SCENE_MENU:
      menuExit();
      break;
    case SCENE_LEVEL:
      levelCleanup();
      break;
    // case SCENE_PAUSE: pauseExit(); break;
    case SCENE_TRANSITION:
      transitionCleanup();
      break;
    default:
      break;
    }
    sceneExit(current);
    *current = *nextScene;
  }
}

void loadStaticObject(C2D_Image img, Block* block, int offset_x, int offset_y, CollisionShape shape)
{
    switch (shape)
    {
    case RECTANGLE_SHAPE:
        loadGroundBox(block->col * TILE_SIZE + OFFSET_X, block->row * TILE_SIZE + OFFSET_Y, block->width, block->height, offset_x, offset_y);
        break;
    case TRIANGLE_SHAPE:
        loadTriangleBox(block->col * TILE_SIZE + OFFSET_X, block->row * TILE_SIZE + OFFSET_Y, block->width, block->height, offset_x, offset_y);
        break;
    default:
        break;
    }
}
