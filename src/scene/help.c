#include <seven/hw/input.h>
#include <seven/base/bits.h>
#include <seven/video/bg_bitmap.h>
#include <seven/video/prelude.h>
#include <seven/hw/video.h>

#include "../../data/bitmaps/help-0.h"
#include "../../data/bitmaps/help-1.h"
#include "../../data/bitmaps/help-2.h"
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

#define HELP_IMAGE_SIZE 3
int help_image = 0;

static void HelpOpen() {
  REG_DISPCNT = VIDEO_MODE_BITMAP_INDEXED | VIDEO_BG2_ENABLE;

	MemCpy32(BG_PALETTE, help_0_palette, help_0_palette_size);
}

static void HelpUpdate()
{
	if(inputKeysPressed(KEY_LEFT)) {
		if(help_image - 1 < 0) {
			help_image = HELP_IMAGE_SIZE - 1;
		} else {
		--help_image;
		}
	} else if (inputKeysPressed(KEY_RIGHT)){
		if(help_image + 1 == HELP_IMAGE_SIZE) {
			help_image = 0;
		} else {
		++help_image;
		}
	}

  if(inputKeysPressed(KEY_A)) {
    SceneGoTo(mission_scene);
  }
}

static void HelpDraw()
{
	switch(help_image) {
		case 0:
			MemCpy32(MODE4_FRAME_0, help_0_tiles, help_1_tiles_size);
		break;
		case 1:
			MemCpy32(MODE4_FRAME_0, help_1_tiles, help_1_tiles_size);
		break;
    case 2:
			MemCpy32(MODE4_FRAME_0, help_2_tiles, help_1_tiles_size);
		break;
	}
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
