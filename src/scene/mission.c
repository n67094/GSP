#include <seven/video/bg_bitmap.h>
#include <seven/video/prelude.h>
#include <seven/hw/input.h>
#include <seven/hw/memory.h>
#include <seven/hw/video.h>

#include "../core/label.h"
#include "../entity/earth.h"
#include "../interface/interface.h"
#include "../interface/stage.h"
#include "../renderer/sphere.h"
#include "../types.h"
#include "../global.h"

#include "scene.h"

SphereData earth;

static void MissionOpen()
{
  REG_DISPCNT = VIDEO_MODE_AFFINE | VIDEO_BG2_ENABLE | VIDEO_OBJ_ENABLE | VIDEO_OBJ_MAPPING_1D;
  REG_BG2CNT = BG_TILE_8BPP;

  EarthInit();

  InterfaceInit();
}

static void MissionUpdate()
{
  if (~(REG_KEYINPUT)&KEY_UP) {
    earth.pitch++;
  }

  if(inputKeysReleased(KEY_A)) {
    StageNext();
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
  EarthDraw(&earth);

  InterfaceUpdate(7, 8, 3, 1234, 2345, 5);
}

static void MissionDraw() {
  InterfaceDraw();

}

static void MissionVBlank() {}

static void MissionClose() {}

Scene mission_scene = {
    .Open = MissionOpen, .Update = MissionUpdate, .Draw = MissionDraw, .VBlank = MissionVBlank, .Close = MissionClose};
