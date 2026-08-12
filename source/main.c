#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sprites.h"

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240
#define NUM_OPTIONS 4

static ndspWaveBuf musicBuf;
static void* musicData = NULL;

static bool loadWav(const char* path, u32* outSampleRate, u16* outChannels)
{
    FILE* fp = fopen(path, "rb");
    if (!fp) return false;

    char riff[4];
    fread(riff, 1, 4, fp);
    if (memcmp(riff, "RIFF", 4) != 0) { fclose(fp); return false; }

    fseek(fp, 4, SEEK_CUR);
    char wave[4];
    fread(wave, 1, 4, fp);
    if (memcmp(wave, "WAVE", 4) != 0) { fclose(fp); return false; }

    u16 channels = 0, bitsPerSample = 0;
    u32 sampleRate = 0, dataSize = 0;
    long dataOffset = -1;

    while (!feof(fp))
    {
        char chunkId[4];
        u32 chunkSize;
        if (fread(chunkId, 1, 4, fp) != 4) break;
        if (fread(&chunkSize, 4, 1, fp) != 1) break;

        long chunkDataPos = ftell(fp);

        if (memcmp(chunkId, "fmt ", 4) == 0)
        {
            u16 audioFormat;
            fread(&audioFormat, 2, 1, fp);
            fread(&channels, 2, 1, fp);
            fread(&sampleRate, 4, 1, fp);
            fseek(fp, 6, SEEK_CUR);
            fread(&bitsPerSample, 2, 1, fp);
        }
        else if (memcmp(chunkId, "data", 4) == 0)
        {
            dataSize = chunkSize;
            dataOffset = chunkDataPos;
        }
        fseek(fp, chunkDataPos + chunkSize + (chunkSize & 1), SEEK_SET);
    }

    if (dataOffset < 0 || bitsPerSample != 16 || dataSize == 0)
    {
        fclose(fp);
        return false;
    }

    musicData = linearAlloc(dataSize);
    if (!musicData) { fclose(fp); return false; }

    fseek(fp, dataOffset, SEEK_SET);
    fread(musicData, 1, dataSize, fp);
    fclose(fp);

    memset(&musicBuf, 0, sizeof(musicBuf));
    musicBuf.data_vaddr = musicData;
    musicBuf.nsamples = dataSize / (bitsPerSample / 8) / channels;
    musicBuf.looping = true;

    *outSampleRate = sampleRate;
    *outChannels = channels;
    return true;
}

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

    C2D_SpriteSetCenter(&selectionArrow, 0.5f, 0.5f);

    C2D_TextBuf textBuf = C2D_TextBufNew(256);
    C2D_Font font = C2D_FontLoadSystem(CFG_REGION_USA);

    const char* labelStrings[NUM_OPTIONS] = { "PLAY", "LEVELS", "SETTINGS", "EXIT" };
    C2D_Text labels[NUM_OPTIONS];
    for (int i = 0; i < NUM_OPTIONS; i++)
    {
        C2D_TextFontParse(&labels[i], font, textBuf, labelStrings[i]);
        C2D_TextOptimize(&labels[i]);
    }

    float labelX[NUM_OPTIONS], labelY[NUM_OPTIONS], labelH[NUM_OPTIONS];
    float startY = 140.0f;

    for (int i = 0; i < NUM_OPTIONS; i++)
    {
        float w, h;
        C2D_TextGetDimensions(&labels[i], 1.0f, 1.0f, &w, &h);
        labelX[i] = SCREEN_WIDTH / 2 - w / 2;
        labelY[i] = startY + i * 20.0f;
        labelH[i] = h;
    }

    int selectedIndex = 0;

    ndspInit();
    ndspSetOutputMode(NDSP_OUTPUT_STEREO);

    u32 sampleRate;
    u16 channels;

    if (loadWav("romfs:/audio/menu.wav", &sampleRate, &channels))
    {
        printf("Musica cargada: %lu Hz, %u canales, %lu samples\n",
           sampleRate, channels, musicBuf.nsamples);
        
        ndspChnSetInterp(0, NDSP_INTERP_LINEAR);
        ndspChnSetRate(0, sampleRate);
        ndspChnSetFormat(0, channels == 2 ? NDSP_FORMAT_STEREO_PCM16 : NDSP_FORMAT_MONO_PCM16);

        float mix[12];
        memset(mix, 0, sizeof(mix));
        mix[0] = 1.0f;
        mix[1] = 1.0f;
        ndspChnSetMix(0, mix);

        ndspChnWaveBufAdd(0, &musicBuf);
    }
    else
    {
        printf("ERROR: no se pudo cargar romfs:/audio/menu.wav\n");
    }

    while (aptMainLoop())
    {
        hidScanInput();
        u32 kDown = hidKeysDown();

        if (kDown & KEY_START)
            break;
        
        if (kDown & KEY_DOWN)
        {
            selectedIndex = (selectedIndex + 1) % NUM_OPTIONS;
        }

        if (kDown & KEY_UP)
        {
            selectedIndex = (selectedIndex + NUM_OPTIONS - 1) % NUM_OPTIONS;
        }

        float arrowX = labelX[selectedIndex] - 20.0f;
        float arrowY = labelY[selectedIndex] + labelH[selectedIndex] / 2.0f;
        C2D_SpriteSetPos(&selectionArrow, arrowX, arrowY);

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

        C2D_TargetClear(
            top,
            C2D_Color32(20, 20, 40, 255)
        );

        C2D_SceneBegin(top);

        C2D_DrawSprite(&wizardsUnited);
        C2D_DrawSprite(&titleLogo);

        for (int i = 0; i < NUM_OPTIONS; i++)
        {
            C2D_DrawText(&labels[i], C2D_WithColor, labelX[i], labelY[i], 0.0f, 1.0f, 1.0f,
                         C2D_Color32(230, 230, 230, 255));
        }

        C2D_DrawSprite(&selectionArrow);
        C2D_Flush();

        C3D_FrameEnd(0);
    }

    if (musicData) linearFree(musicData);
    ndspExit();

    C2D_SpriteSheetFree(spriteSheet);
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    romfsExit();

    return 0;
}