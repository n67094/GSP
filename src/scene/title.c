#include <seven/base/types.h>
#include <seven/video/prelude.h>
#include <seven/video/bg_bitmap.h>
#include <seven/hw/input.h>

#include "scene-manager.h"

#include "../core/input.h"

static void TitleOpen() {
  REG_DISPCNT = VIDEO_MODE_BITMAP | VIDEO_BG2_ENABLE;
}

static void TitleUpdate() {
  if (~(REG_KEYINPUT)&KEY_A) {
    SceneManagerGoTo(mission_scene);
  }
}

static void TitleDraw() {
  MODE3_FRAME[80][120] = COLOR_RED;
  MODE3_FRAME[80][136] = COLOR_RED;
  MODE3_FRAME[96][120] = COLOR_RED;
  MODE3_FRAME[96][136] = COLOR_RED;
}

static void TitleVBlank() {

}

static void TitleClose() {

}

Scene title_scene = {
  .Open = TitleOpen,
  .Update = TitleUpdate,
  .Draw = TitleDraw,
  .VBlank = TitleVBlank,
  .Close = TitleClose
};
