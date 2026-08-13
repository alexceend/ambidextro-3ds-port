#ifndef SCENE_H
#define SCENE_H

typedef enum
{
    SCENE_NONE = -1,
    SCENE_MENU,
    SCENE_TEST
} Scene;

void sceneChange(Scene scene);

#endif