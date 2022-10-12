#include <seven/hw/input.h>
#include <seven/base/bits.h>
#include <seven/video/bg_bitmap.h>
#include <seven/video/prelude.h>
#include <seven/hw/video.h>

#include "../../data/bitmaps/title.h"
#include "../../data/bitmaps/title.h"
#include "../../data/sounds/title-sound.h"

#include "../types.h"

#include "../global.h"
#include "../debug/log.h"

#include "../core/input.h"
#include "../core/memory.h"
#include "../core/sound.h"

#include "scene.h"
#include "seven/base/bits.h"

#define HELP_INTERFACE_PALETTE_0 0

static void HelpOpen() {
  REG_DISPCNT = VIDEO_MODE_BITMAP_INDEXED | VIDEO_BG2_ENABLE;

  MemCpy32(BG_PALETTE, title_palette, title_palette_size);

  MemCpy32(MODE4_FRAME_0, title_tiles, title_tiles_size);
  MemCpy32(MODE4_FRAME_1, title_tiles, title_tiles_size);
}

static void HelpUpdate()
{
  if(inputKeysPressed(KEY_LEFT) || inputKeysPressed(KEY_RIGHT)) {
    bmpSwapBuffers();
  }

  if(inputKeysPressed(KEY_A)) {
    SceneGoTo(mission_scene);
  }
}

static void HelpDraw()
{
}

static void HelpVBlank() {
  u32 sound_duration = CheckSoundProgress(title_song, TITLE_SOUND_CHANNEL, TITLE_SOUND_SIZE);
  if(sound_duration){
  SoundRepeat(title_song, TITLE_SOUND_CHANNEL, sound_duration);
  }
}

static void HelpClose() {
  SoundStop(TITLE_SOUND_CHANNEL);
}

Scene help_scene = {
    .Open = HelpOpen, .Update = HelpUpdate, .Draw = HelpDraw, .VBlank = HelpVBlank, .Close = HelpClose};
