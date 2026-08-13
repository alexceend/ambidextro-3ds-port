#include "audio_core.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static ndspWaveBuf musicBuf;
static void* musicData = NULL;


bool audioInit(void)
{
    Result res = ndspInit();

    if (R_FAILED(res)) {
        printf("ndspInit failed: 0x%08lX\n", res);
        return false;
    }

    ndspSetOutputMode(NDSP_OUTPUT_STEREO);

    return true;
}

static bool loadWav(
    const char* path,
    u32* outSampleRate,
    u16* outChannels,
    ndspWaveBuf* outMusicBuf,
    void** outMusicData
)
{
    FILE* fp = fopen(path, "rb");

    if (!fp)
        return false;

    char riff[4];

    fread(riff, 1, 4, fp);

    if (memcmp(riff, "RIFF", 4) != 0)
    {
        fclose(fp);
        return false;
    }

    fseek(fp, 4, SEEK_CUR);

    char wave[4];

    fread(wave, 1, 4, fp);

    if (memcmp(wave, "WAVE", 4) != 0)
    {
        fclose(fp);
        return false;
    }

    u16 channels = 0;
    u16 bitsPerSample = 0;
    u32 sampleRate = 0;
    u32 dataSize = 0;

    long dataOffset = -1;

    while (!feof(fp))
    {
        char chunkId[4];
        u32 chunkSize;

        if (fread(chunkId, 1, 4, fp) != 4)
            break;

        if (fread(&chunkSize, 4, 1, fp) != 1)
            break;

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

        fseek(
            fp,
            chunkDataPos + chunkSize + (chunkSize & 1),
            SEEK_SET
        );
    }

    if (dataOffset < 0 || bitsPerSample != 16 || dataSize == 0)
    {
        fclose(fp);
        return false;
    }

    *outMusicData = linearAlloc(dataSize);

    if (!*outMusicData)
    {
        fclose(fp);
        return false;
    }

    fseek(fp, dataOffset, SEEK_SET);

    fread(*outMusicData, 1, dataSize, fp);

    fclose(fp);

    memset(outMusicBuf, 0, sizeof(*outMusicBuf));

    outMusicBuf->data_vaddr = *outMusicData;
    outMusicBuf->nsamples =
        dataSize / (bitsPerSample / 8) / channels;

    outMusicBuf->looping = true;

    *outSampleRate = sampleRate;
    *outChannels = channels;

    return true;
}

bool audioPlayMusic(const char* path)
{
    u32 sampleRate;
    u16 channels;

    if (!loadWav(
        path,
        &sampleRate,
        &channels,
        &musicBuf,
        &musicData))
    {
        return false;
    }

    ndspChnSetInterp(0, NDSP_INTERP_LINEAR);
    ndspChnSetRate(0, sampleRate);

    ndspChnSetFormat(
        0,
        channels == 2
            ? NDSP_FORMAT_STEREO_PCM16
            : NDSP_FORMAT_MONO_PCM16
    );

    float mix[12];
    memset(mix, 0, sizeof(mix));

    mix[0] = 1.0f;
    mix[1] = 1.0f;

    ndspChnSetMix(0, mix);
    ndspChnWaveBufAdd(0, &musicBuf);

    return true;
}


void audioExit(void)
{
    if (musicData)
    {
        linearFree(musicData);
        musicData = NULL;
    }

    ndspExit();
}