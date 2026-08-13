#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sprites.h"
#include "audio_core.h"
#include "test.h"
#include "menu.h"
#include "scene.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240


int main(int argc, char** argv)
{
    romfsInit();
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);

    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

    C3D_RenderTarget* top =
        C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    if (!top)
    {
        C2D_Fini();
        C3D_Fini();
        gfxExit();
        romfsExit();
        return -1;
    }

    /* AUDIO */

    if (!audioInit())
    {
        printf("ERROR: no se pudo inicializar el audio\n");
    }else if (!audioPlayMusic("romfs:/audio/menu.wav"))
    {
        printf("ERROR: no se pudo reproducir romfs:/audio/menu.wav\n");
    }

    /* MENU */

    Scene currentScene = SCENE_MENU;
    if (!menuInit(top))
    {
        printf("ERROR: no se pudo inicializar el menu\n");

        audioExit();

        C2D_Fini();
        C3D_Fini();

        gfxExit();
        romfsExit();

        return -1;
    }


    while (aptMainLoop())
    {
        hidScanInput();

        u32 kDown = hidKeysDown();

        if (kDown & KEY_START)
            break;

        Scene nextScene = SCENE_NONE;
        switch (currentScene)
        {
            case SCENE_MENU: nextScene = menuUpdate(kDown); break;
            case SCENE_TEST: nextScene = testUpdate(kDown); break;
            default: break;
        }

        if (nextScene != SCENE_NONE && nextScene != currentScene)
        {
            bool initOk = false;
            switch (nextScene)
            {
                case SCENE_MENU: initOk = menuInit(top); break;
                case SCENE_TEST: initOk = testInit(top); break;
                default: break;
            }

            if (initOk)
            {
                switch (currentScene)
                {
                    case SCENE_MENU: menuExit(); break;
                    case SCENE_TEST: testExit(); break;
                    default: break;
                }
                currentScene = nextScene;
            }
        }
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        switch (currentScene)
        {
            case SCENE_MENU: menuDraw(); break;
            case SCENE_TEST: testDraw(); break;
            default: break;
        }

        C3D_FrameEnd(0);
    }

    switch (currentScene)
    {
        case SCENE_MENU: menuExit(); break;
        case SCENE_TEST: testExit(); break;
        default: break;
    }

    /* CLEANUP */

    audioExit();

    C2D_Fini();
    C3D_Fini();

    gfxExit();
    romfsExit();

    return 0;
}