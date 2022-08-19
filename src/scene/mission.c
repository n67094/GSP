#include <seven/video/bg_bitmap.h>
#include <seven/video/prelude.h>
#include <seven/hw/input.h>
#include <seven/hw/memory.h>
#include <seven/hw/video.h>

#include "../entity/earth.h"
#include "../interface/interface.h"
#include "../renderer/sphere.h"
#include "../renderer/spaceship.h"
#include "../types.h"

#include "scene-manager.h"
#include "../global.h"

SphereData earth;

static void MissionOpen()
{
  REG_DISPCNT = VIDEO_MODE_AFFINE | VIDEO_BG2_ENABLE | VIDEO_OBJ_MAPPING_1D;
  REG_BG2CNT = BG_TILE_8BPP;

  EarthInit();

  InterfaceInit();
}

static void MissionUpdate()
{
  if (~(REG_KEYINPUT)&KEY_UP) {
    earth.pitch++;
  }

  if (~(REG_KEYINPUT)&KEY_DOWN) {
    earth.pitch--;
  }

  if (~(REG_KEYINPUT)&KEY_LEFT) {
    earth.spin--;
  }

  if (~(REG_KEYINPUT)&KEY_RIGHT) {
    earth.spin++;
  }

  // Due to the time it take to compute it cannot be move in draw
  //EarthDraw(&earth);
}

static void MissionDraw() {
	TransferBuffer(spaceship_buffer, (u16 *)0x6008000); //That VRAM address was just made up for testing purposes.
	ClearBuffer(spaceship_buffer);
}

static void MissionVBlank() {}

static void MissionClose() {}

Scene mission_scene = {
    .Open = MissionOpen, .Update = MissionUpdate, .Draw = MissionDraw, .VBlank = MissionVBlank, .Close = MissionClose};
