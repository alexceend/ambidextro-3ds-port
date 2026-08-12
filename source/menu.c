#include "menu.h"

#include <stdio.h>

#include "sprites.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define NUM_OPTIONS 4

static C3D_RenderTarget* top = NULL;

static C2D_SpriteSheet spriteSheet;

static C2D_Sprite wizardsUnited;
static C2D_Sprite titleLogo;
static C2D_Sprite selectionArrow;

static C2D_TextBuf textBuf;
static C2D_Font font;

static C2D_Text labels[NUM_OPTIONS];

static float labelX[NUM_OPTIONS];
static float labelY[NUM_OPTIONS];
static float labelH[NUM_OPTIONS];

static int selectedIndex = 0;

bool menuInit(C3D_RenderTarget* target)
{
    top = target;

    spriteSheet =
        C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");

    if (!spriteSheet)
    {
        printf("ERROR: no se pudo cargar sprites.t3x\n");
        return false;
    }

    C2D_SpriteFromSheet(
        &wizardsUnited,
        spriteSheet,
        sprites_wizards_united_idx
    );

    C2D_SpriteFromSheet(
        &titleLogo,
        spriteSheet,
        sprites_ambidextro_logo_pixel_idx
    );

    C2D_SpriteFromSheet(
        &selectionArrow,
        spriteSheet,
        sprites_menu_selection_arrow_idx
    );

    C2D_SpriteSetCenter(
        &wizardsUnited,
        0.5f,
        0.5f
    );

    C2D_SpriteSetPos(
        &wizardsUnited,
        SCREEN_WIDTH / 2,
        90
    );

    C2D_SpriteSetCenter(
        &titleLogo,
        0.5f,
        0.5f
    );

    C2D_SpriteSetPos(
        &titleLogo,
        SCREEN_WIDTH / 2,
        40
    );

    C2D_SpriteSetCenter(
        &selectionArrow,
        0.5f,
        0.5f
    );

    textBuf = C2D_TextBufNew(256);

    if (!textBuf)
    {
        C2D_SpriteSheetFree(spriteSheet);
        spriteSheet = NULL;
        return false;
    }

    font = C2D_FontLoadSystem(CFG_REGION_USA);

    const char* labelStrings[NUM_OPTIONS] =
    {
        "PLAY",
        "LEVELS",
        "SETTINGS",
        "EXIT"
    };

    for (int i = 0; i < NUM_OPTIONS; i++)
    {
        C2D_TextFontParse(
            &labels[i],
            font,
            textBuf,
            labelStrings[i]
        );

        C2D_TextOptimize(&labels[i]);
    }

    float startY = 140.0f;

    for (int i = 0; i < NUM_OPTIONS; i++)
    {
        float w;
        float h;

        C2D_TextGetDimensions(
            &labels[i],
            1.0f,
            1.0f,
            &w,
            &h
        );

        labelX[i] = SCREEN_WIDTH / 2 - w / 2;
        labelY[i] = startY + i * 20.0f;
        labelH[i] = h;
    }

    selectedIndex = 0;

    return true;
}

bool menuUpdate(u32 kDown)
{
    if (kDown & KEY_DOWN)
    {
        selectedIndex =
            (selectedIndex + 1) % NUM_OPTIONS;
    }

    if (kDown & KEY_UP)
    {
        selectedIndex =
            (selectedIndex + NUM_OPTIONS - 1)
            % NUM_OPTIONS;
    }

    if (kDown & KEY_A)
    {
        switch (selectedIndex)
        {
            case 0:
                printf("PLAY selected\n");
                break;
            case 1:
                printf("LEVELS selected\n");
                break;
            case 2:
                printf("SETTINGS selected\n");
                break;
            case 3:
                printf("EXIT selected\n");
                return true;
        }
    }


    float arrowX =
        labelX[selectedIndex] - 20.0f;

    float arrowY =
        labelY[selectedIndex]
        + labelH[selectedIndex] / 2.0f;

    C2D_SpriteSetPos(
        &selectionArrow,
        arrowX,
        arrowY
    );
}


void menuDraw(void)
{
    C2D_TargetClear(
        top,
        C2D_Color32(20, 20, 40, 255)
    );

    C2D_SceneBegin(top);

    C2D_DrawSprite(&wizardsUnited);
    C2D_DrawSprite(&titleLogo);

    for (int i = 0; i < NUM_OPTIONS; i++)
    {
        C2D_DrawText(
            &labels[i],
            C2D_WithColor,
            labelX[i],
            labelY[i],
            0.0f,
            1.0f,
            1.0f,
            C2D_Color32(230, 230, 230, 255)
        );
    }

    C2D_DrawSprite(&selectionArrow);

    C2D_Flush();
}

void menuExit(void)
{
    if (textBuf)
    {
        C2D_TextBufDelete(textBuf);
        textBuf = NULL;
    }

    if (spriteSheet)
    {
        C2D_SpriteSheetFree(spriteSheet);
        spriteSheet = NULL;
    }

    top = NULL;
}