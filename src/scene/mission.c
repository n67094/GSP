#include <seven/video/bg_bitmap.h>
#include <seven/video/prelude.h>

#include "scene-manager.h"

#include "../types.h"
#include "../renderer/sphere.h"
#include "../core/utils.h"

#include "../../data/bitmaps/earth.h"

#include "seven/hw/input.h"
#include "seven/hw/memory.h"
#include "seven/hw/video.h"

static s32 pitch = 0;
static s32 spin = 0;

static void MissionOpen() {
  REG_DISPCNT = VIDEO_MODE_AFFINE | VIDEO_BG2_ENABLE | VIDEO_OBJ_MAPPING_1D;
  REG_BG2CNT = BG_TILE_8BPP;

  TransferPalette(earth_palette);
  TransferTilemap();
}

static void MissionUpdate() {
  if (~(REG_KEYINPUT)&KEY_UP) {
    pitch++;
  }

  if (~(REG_KEYINPUT)&KEY_DOWN) {
    pitch--;
  }

  if (~(REG_KEYINPUT)&KEY_LEFT) {
    spin--;
  }

  if (~(REG_KEYINPUT)&KEY_RIGHT) {
    spin++;
  }

  // Due to the time it take to compute it cannot be move in draw
  SphereRender(earth_tiles, (u8 *)MEM_VRAM, pitch, spin, 7, 9);
}

static void MissionDraw() {
}

static void MissionVBlank() {

}

static void MissionClose() {

}

Scene mission_scene = {
  .Open = MissionOpen,
  .Update = MissionUpdate,
  .Draw = MissionDraw,
  .VBlank = MissionVBlank,
  .Close = MissionClose
};

