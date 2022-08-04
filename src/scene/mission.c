#include <seven/video/prelude.h>
#include <seven/video/bg_bitmap.h>

#include "scene-manager.h"

static void MissionOpen() {
  REG_DISPCNT = VIDEO_MODE_BITMAP | VIDEO_BG2_ENABLE;
}

static void MissionUpdate() {

}

static void MissionDraw() {
  MODE3_FRAME[80][120] = COLOR_BLUE;
  MODE3_FRAME[80][136] = COLOR_BLUE;
  MODE3_FRAME[96][120] = COLOR_BLUE;
  MODE3_FRAME[96][136] = COLOR_BLUE;
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

