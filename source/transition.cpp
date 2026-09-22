#include "transition.h"
#include <string>
#include <array>

static C3D_RenderTarget* top = nullptr;
static C3D_RenderTarget* bottom = nullptr;

static C2D_Font font;
static C2D_TextBuf textBuf;

static C2D_Text currentLevelLabel[LEVEL_TEXT_UNITS];
static C2D_Text slashLabel;
static C2D_Text lastLevelLabel[LEVEL_TEXT_UNITS];

static float labelX[LEVEL_TEXT_UNITS];
static float labelY[LEVEL_TEXT_UNITS];

u64 time_start;
u64 time_elapsed;
u64 time_stop;

using namespace std;

string numberToString(size_t number)
{
    size_t padding;
    string result;

    if (number < 10)
    {
        padding = 2;
    }
    else if (number >= 10 && number < 100)
    {
        padding = 1;
    }
    else padding = 0;

    for (size_t i = 0; i < padding; i++)
    {
        result.append("0");
    }

    result.append(to_string(number));

    return result;
}

bool transitionInit(C3D_RenderTarget* targetTop, C3D_RenderTarget* targetBottom, uint8_t currentLevel, uint8_t nextLevel)
{
    targetTop = top;
    targetBottom = bottom;
    string currentLevelString;
    string lastLevelString;

    textBuf = C2D_TextBufNew(10);

    if (!textBuf)
    {
        return false;
    }

    font = C2D_FontLoad("romfs:/fonts/byte_bounce/bytebounce.medium.bcfnt");

    currentLevelString = numberToString(currentLevel);
    lastLevelString = numberToString(LAST_LEVEL);


    for (size_t i = 0; i < LEVEL_TEXT_UNITS; i++)
    {
        if (!C2D_TextFontParse(&currentLevelLabel[i], font, textBuf, &currentLevelString.c_str()[i]))
        {
            return false;
        }
        C2D_TextOptimize(&currentLevelLabel[i]);
    }

    C2D_TextBufClear(textBuf);

    if (!C2D_TextFontParse(&slashLabel, font, textBuf, "/")) return false;
    C2D_TextOptimize(&slashLabel);

    C2D_TextBufClear(textBuf);

    for (size_t i = 0; i < LEVEL_TEXT_UNITS; i++)
    {
        if (!C2D_TextFontParse(&lastLevelLabel[i], font, textBuf, &lastLevelString.c_str()[i]))
        {
            return false;
        }
        C2D_TextOptimize(&lastLevelLabel[i]);
    }

    C2D_TextBufClear(textBuf);

    time_start = osGetTime();
}

Scene transitionUpdate()
{
    time_stop = osGetTime();
    time_elapsed = time_stop - time_start;

    if (time_elapsed >= TRANSITION_TIMER)
    {
        return SCENE_MENU;
    }
}

void transitionDraw()
{
    for (size_t i = 0; i < LEVEL_TEXT_UNITS; i++)
    {
        C2D_DrawText(
            &currentLevelLabel[i],
            C2D_WithColor,
            
        )
    }
}

