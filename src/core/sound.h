#ifndef CORE_SOUND_H
#define CORE_SOUND_H

#include "../types.h"

void SoundInit();

//returns the duration played if sound has reached the end. 0 otherwise.
u32 CheckSoundProgress(cs8 *sound, u32 channel, u32 size);

void SoundPlay(cs8 *sound, u32 channel);

void SoundRepeat(cs8 *sound, u32 channel, u32 duration);

void SoundStop(u32 channel);

#endif
