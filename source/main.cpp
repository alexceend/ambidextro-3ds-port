#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sprites.h"
#include "audio_core.h"
#include "menu.h"
#include "assets_loader.h"
#include "game_manager.h"

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

    /* ASSETS LOADER */

    init_assets();

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

    Subject gameManager;

    while (aptMainLoop())
    {
        hidScanInput();

        u32 kDown = hidKeysDown();

        u32 kHeld = hidKeysHeld();

        if (kDown & KEY_START) break;

        Scene nextScene = SCENE_NONE;

        gameManager.ManageGame(kDown, kHeld);
    
        sceneUpdate(&currentScene, &nextScene, kDown, kHeld);

        sceneChange(&currentScene, &nextScene, top);

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        sceneDraw(&currentScene);

        C3D_FrameEnd(0);
    }

    sceneExit(&currentScene);

    /* CLEANUP */

    cfguExit();
    audioExit();

    C2D_Fini();
    C3D_Fini();

    gfxExit();
    romfsExit();

    return 0;
}