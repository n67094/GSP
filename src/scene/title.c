#include <seven/hw/input.h>
#include <seven/video/bg_bitmap.h>
#include <seven/video/prelude.h>
#include <seven/hw/video.h>

#include "../../data/bitmaps/title.h"
#include "../../data/bitmaps/title-obj.palette.h"
#include "../../data/bitmaps/start-1.tiles.h"
#include "../../data/bitmaps/start-2.tiles.h"
#include "../../data/sounds/title-sound.h"

#include "../global.h"
#include "../types.h"

#include "../debug/log.h"

#include "../core/input.h"
#include "../core/memory.h"
#include "../core/object.h"
#include "../core/tile.h"
#include "../core/sound.h"

#include "scene.h"


#define TITLE_INTERFACE_PALETTE_0 0

#define OAM_TITLE_START_1 0
#define TILE_TITLE_START_1 512
#define POS_TITLE_START_1_X 64
#define POS_TITLE_START_1_Y 128

#define OAM_TITLE_START_2 1
#define TILE_TITLE_START_2 544
#define POS_TITLE_START_2_X 128
#define POS_TITLE_START_2_Y 128

#define DELAY_TITLE_START 30 //Frame so 0.5 sec

static u32 title_frame_counter = 0;
static bool title_show_start = true;

static void TitleOpen() {
  OamInit(object_buffer, 128);

  REG_DISPCNT = VIDEO_MODE_BITMAP_INDEXED | VIDEO_BG2_ENABLE | VIDEO_OBJ_ENABLE | VIDEO_OBJ_MAPPING_1D;

  MemCpy32(BG_PALETTE, title_palette, title_palette_size);

  MemCpy32(MODE4_FRAME_0, title_tiles, title_tiles_size);

  MemCpy32(OBJ_PALETTE, title_obj_palette, title_obj_palette_size);
  MemCpy32(&OBJ4_CHARBLOCKS[1][TILE_TITLE_START_1 - 512], start_1_tiles, start_1_tiles_size);
  MemCpy32(&OBJ4_CHARBLOCKS[1][TILE_TITLE_START_2 - 512], start_2_tiles, start_2_tiles_size);

  Object *start1 = &object_buffer[OAM_TITLE_START_1];
  Object *start2 = &object_buffer[OAM_TITLE_START_2];

  ObjectSetAttr(start1,
    OBJ_SHAPE_WIDE,
    OBJ_SIZE_64X32,
    OBJ_PALETTE_NUMBER(TITLE_INTERFACE_PALETTE_0) | OBJ_TILE_NUMBER(TILE_TITLE_START_1)
  );
  ObjectSetPos(start1, POS_TITLE_START_1_X, POS_TITLE_START_1_Y);

  ObjectSetAttr(start2,
    OBJ_SHAPE_WIDE,
    OBJ_SIZE_64X32,
    OBJ_PALETTE_NUMBER(TITLE_INTERFACE_PALETTE_0) | OBJ_TILE_NUMBER(TILE_TITLE_START_2)
  );
  ObjectSetPos(start2, POS_TITLE_START_2_X, POS_TITLE_START_2_Y);
  
  SoundInit();
  SoundPlay(title_song, TITLE_SOUND_CHANNEL);
}

static void TitleUpdate()
{
  if (inputKeysPressed(KEY_START)) {
    SceneGoTo(help_scene);
  }
}

static void TitleDraw()
{
  oamWriteObjects(0, object_buffer, 128);
}

static void TitleVBlank() {
  if (title_frame_counter >= 30) {
    Object *start_1 = &object_buffer[OAM_TITLE_START_1];
    Object *start_2 = &object_buffer[OAM_TITLE_START_2];

    title_show_start = !title_show_start;

    ObjectVisibility(start_1, title_show_start, OBJ_MODE_REGULAR);
    ObjectVisibility(start_2, title_show_start, OBJ_MODE_REGULAR);

    title_frame_counter = 0;
  } else {
    ++title_frame_counter;
  }

  u32 sound_duration = CheckSoundProgress(title_song, TITLE_SOUND_CHANNEL, TITLE_SOUND_SIZE);
  if(sound_duration){
  SoundRepeat(title_song, TITLE_SOUND_CHANNEL, sound_duration);
  }
}

static void TitleClose() {
  // SoundStop(TITLE_SOUND_CHANNEL);
}

Scene title_scene = {
    .Open = TitleOpen, .Update = TitleUpdate, .Draw = TitleDraw, .VBlank = TitleVBlank, .Close = TitleClose};
