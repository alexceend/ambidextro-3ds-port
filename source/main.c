#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sprites.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240

int main(int argc, char** argv)
{
    romfsInit();
    gfxInitDefault();

    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

    C3D_RenderTarget* top =
        C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

    if (!top)
    {
        gfxExit();
        return -1;
    }

    C2D_SpriteSheet spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if (!spriteSheet) svcBreak(USERBREAK_PANIC);

    C2D_Sprite wizardsUnited, titleLogo, selectionArrow;
    C2D_SpriteFromSheet(&wizardsUnited, spriteSheet, sprites_wizards_united_idx);
    C2D_SpriteFromSheet(&titleLogo,     spriteSheet, sprites_ambidextro_logo_pixel_idx);
    C2D_SpriteFromSheet(&selectionArrow, spriteSheet, sprites_menu_selection_arrow_idx);

    C2D_SpriteSetCenter(&wizardsUnited, 0.5f, 0.5f);
    C2D_SpriteSetPos(&wizardsUnited, SCREEN_WIDTH / 2, 90);

    C2D_SpriteSetCenter(&titleLogo, 0.5f, 0.5f);
    C2D_SpriteSetPos(&titleLogo, SCREEN_WIDTH / 2, 40);

    C2D_SpriteSetPos(&selectionArrow, SCREEN_WIDTH / 2 - 60, 140);

    C2D_TextBuf textBuf = C2D_TextBufNew(256);
    C2D_Font font = C2D_FontLoadSystem(CFG_REGION_USA);

    const char* labelStrings[4] = { "PLAY", "LEVELS", "SETTINGS", "EXIT" };
    C2D_Text labels[4];
    for (int i = 0; i < 4; i++)
    {
        C2D_TextFontParse(&labels[i], font, textBuf, labelStrings[i]);
        C2D_TextOptimize(&labels[i]);
    }

    while (aptMainLoop())
    {
        hidScanInput();

        if (hidKeysDown() & KEY_START)
            break;

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        C2D_TargetClear(
            top,
            C2D_Color32(20, 20, 40, 255)
        );

        C2D_SceneBegin(top);

        C2D_DrawSprite(&wizardsUnited);
        C2D_DrawSprite(&titleLogo);

        float labelY = 140.0f;

        for (int i = 0; i < 4; i++)
        {
            float w, h;
            C2D_TextGetDimensions(&labels[i], 1.0f, 1.0f, &w, &h);
            float x = SCREEN_WIDTH / 2 - w / 2;
            C2D_DrawText(&labels[i], C2D_WithColor, x, labelY, 0.0f, 1.0f, 1.0f,
                         C2D_Color32(230, 230, 230, 255));
            labelY += 20.0f;
        }

        C2D_DrawSprite(&selectionArrow);
        C2D_Flush();

        C3D_FrameEnd(0);
    }

    C2D_SpriteSheetFree(spriteSheet);
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    romfsExit();

    return 0;
}