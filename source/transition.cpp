#include "transition.h"
#include <string>
#include "log.h"
#include "game_constants.h"

static C3D_RenderTarget *top = nullptr;
static C3D_RenderTarget *bottom = nullptr;

static C2D_Font font;
static C2D_TextBuf textBuf;

static C2D_Text currentLevelLabel[LEVEL_TEXT_UNITS];
static C2D_Text slashLabel;
static C2D_Text lastLevelLabel[LEVEL_TEXT_UNITS];
static C2D_Text nextLevelLabel[LEVEL_TEXT_UNITS];

static float labelX[LEVEL_TEXT_UNITS];
static float labelY[LEVEL_TEXT_UNITS];

static float slashLabelX;
static float slashLabelY;

static float lastLabelX[LEVEL_TEXT_UNITS];
static float lastLabelY[LEVEL_TEXT_UNITS];

static float nextLevelX[LEVEL_TEXT_UNITS];
static float nextLevelY[LEVEL_TEXT_UNITS];

float target_time;
float current_y_digit;
char* replaced_digits[LEVEL_TEXT_UNITS];
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
  else
    padding = 0;

  for (size_t i = 0; i < padding; i++)
  {
    result.append("0");
  }

  result.append(to_string(number));

  return result;
}

void setNextLevelDigits()
{
  for (size_t i = 0; i < LEVEL_TEXT_UNITS; i++)
  {
    if (replaced_digits[i] != NULL)
    {
      nextLevelX[i] = labelX[i];
      nextLevelY[i] = labelY[i] - 100.0f;
      C2D_TextFontParse(&nextLevelLabel[i], font, textBuf, &replaced_digits[i][0]);
    }
  }
}

void setReplacedDigits(string current_level, string next_level)
{
  for (size_t i = 0; i < LEVEL_TEXT_UNITS; i++)
  {
    if (current_level[i] != next_level[i])
    {
      replaced_digits[i] = &next_level[i];
    }
    else replaced_digits[i] = nullptr;
  }
}

bool transitionInit(C3D_RenderTarget *targetTop, C3D_RenderTarget *targetBottom,
                    uint8_t currentLevel, uint8_t nextLevel)
{
  top = targetTop;
  bottom = targetBottom;
  string currentLevelString;
  string nextLevelString;
  string lastLevelString;
  float text_width;
  float text_height;

  textBuf = C2D_TextBufNew(256);

  if (!textBuf)
  {
    return false;
  }

  font = C2D_FontLoad("romfs:/fonts/byte_bounce/bytebounce.medium.bcfnt");

  currentLevelString = numberToString(currentLevel);
  nextLevelString = numberToString(nextLevel);

  log_message(currentLevelString.c_str());

  lastLevelString = numberToString(LAST_LEVEL);

  log_message(lastLevelString.c_str());

  for (size_t i = 0; i < LEVEL_TEXT_UNITS; i++)
  {
    char digit[2] = {currentLevelString[i], '\0'};
    if (!C2D_TextFontParse(&currentLevelLabel[i], font, textBuf,
                           digit))
    {
      return false;
    }
    C2D_TextGetDimensions(&currentLevelLabel[i], 1.0f, 1.0f, &text_width, &text_height);
    labelX[i] = (SCREEN_WIDTH / 2 - 80.0f) + 20 * i - text_width / 2;
    labelY[i] = SCREEN_HEIGHT / 2 - text_height / 2;
    C2D_TextOptimize(&currentLevelLabel[i]);
  }

  if (!C2D_TextFontParse(&slashLabel, font, textBuf, "/"))
    return false;
  C2D_TextOptimize(&slashLabel);
  C2D_TextGetDimensions(&slashLabel, 1.0f, 1.0f, &text_width, &text_height);
  slashLabelX = SCREEN_WIDTH / 2 - text_width / 2;
  slashLabelY = SCREEN_HEIGHT / 2 - text_height / 2;

  for (size_t i = 0; i < LEVEL_TEXT_UNITS; i++)
  {
    char data[2] = {lastLevelString[i], '\0'};
    if (!C2D_TextFontParse(&lastLevelLabel[i], font, textBuf,
                           data))
    {
      return false;
    }
    C2D_TextGetDimensions(&lastLevelLabel[i], 1.0f, 1.0f, &text_width, &text_height);
    lastLabelX[i] = (SCREEN_WIDTH / 2 + 40.0f) + 20 * i - text_width / 2;
    lastLabelY[i] = SCREEN_HEIGHT / 2 - text_height / 2;
    C2D_TextOptimize(&lastLevelLabel[i]);
  }

  setReplacedDigits(currentLevelString, nextLevelString);
  setNextLevelDigits();
  time_start = osGetTime();
  return true;
}

void animateDigits(uint64_t current_time, float *current_y, float target_y)
{
  float normalized_y = (3 * current_time * current_time) - (2 * current_time * current_time * current_time);

  *current_y += normalized_y * target_y;
}



Scene transitionUpdate()
{
  time_stop = osGetTime();
  time_elapsed = time_stop - time_start;

  if (time_elapsed >= SCENE_TIMER)
  {
    return SCENE_MENU;
  }
  animateDigits(time_elapsed / TRANSITION_TIMER, &current_y_digit, labelY[0] - 100.0f);
  return SCENE_NONE;
}

void transitionDraw()
{
  float text_width;
  float text_height;
  size_t i;

  C2D_TargetClear(top, C2D_Color32(40, 40, 40, 255));
  C2D_SceneBegin(top);

  C2D_TextGetDimensions(&currentLevelLabel[0], 1.0f, 1.0f, &text_width, &text_height);

  for (i = 0; i < LEVEL_TEXT_UNITS; i++)
  {
    C2D_DrawText(&currentLevelLabel[i], C2D_WithColor, labelX[i], labelY[i],
                 0.0f, 1.0f, 1.0f, C2D_Color32(255, 255, 255, 255));
  }

  C2D_TextGetDimensions(&slashLabel, 1.0f, 1.0f, &text_width, &text_height);

  C2D_DrawText(&slashLabel, C2D_WithColor, slashLabelX, slashLabelY, 0.0f, 1.0f,
               1.0f, C2D_Color32(255, 255, 255, 255));

  C2D_TextGetDimensions(&lastLevelLabel[0], 1.0f, 1.0f, &text_width, &text_height);

  for (i = 0; i < LEVEL_TEXT_UNITS; i++)
  {
    C2D_DrawText(&lastLevelLabel[i], C2D_WithColor, lastLabelX[i],
                 lastLabelY[i], 0.0f, 1.0f, 1.0f,
                 C2D_Color32(255, 255, 255, 255));
  }

  for (i = 0; i < LEVEL_TEXT_UNITS; i++)
  {
    if (replaced_digits[i] != nullptr)
    {
      C2D_DrawText(&nextLevelLabel[i], C2D_WithColor, nextLevelX[i],
        current_y_digit, 0.0f, 1.0f, 1.0f, C2D_Color32(255, 255, 255, 255));
    }
  }
  C2D_Flush();
}

void transitionCleanup()
{
  C2D_TextBufDelete(textBuf);
  top = NULL;
  bottom = NULL;
}
