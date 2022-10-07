#ifndef CORE_SOUND_H
#define CORE_SOUND_H

#include "../types.h"

void SoundInit();
u32 CheckSoundProgress(SoundData *); //returns the duration played if sound has reached the end. 0 otherwise.
void SoundPlay(SoundData *);
void SoundRepeat(SoundData *, u32);
void SoundStop(SoundData *);

#endif
