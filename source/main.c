#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sprites.h"
#include "audio_core.h"
#include "menu.h"

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

    /* MAIN LOOP */

    bool running = true;

    while (aptMainLoop() && running)
    {
        hidScanInput();

        u32 kDown = hidKeysDown();

        if (kDown & KEY_START)
            break;

        running = !menuUpdate(kDown);

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        menuDraw();

        C3D_FrameEnd(0);
    }

    /* CLEANUP */

    audioExit();
    menuExit();

    C2D_Fini();
    C3D_Fini();

    gfxExit();
    romfsExit();

    return 0;
}