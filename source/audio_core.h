#ifndef AUDIO_CORE_H
#define AUDIO_CORE_H

#include <3ds.h>
#include <stdbool.h>

bool audioInit(void);
bool audioPlayMusic(const char *path);
void audioStopMusic(void);
void audioExit(void);
bool audioChangeMusic(const char *path);

#endif