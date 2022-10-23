#include <seven/video/bg_bitmap.h>
#include <seven/video/prelude.h>
#include <seven/hw/input.h>
#include <seven/hw/memory.h>
#include <seven/hw/video.h>

#include "../debug/log.h"

#include "../core/label.h"
#include "../core/font.h"
#include "../core/tile.h"

#include "../entity/earth.h"
#include "../entity/spaceship.h"

#include "../interface/interface.h"
#include "../interface/stage.h"

#include "../renderer/sphere.h"

#include "../types.h"
#include "../global.h"

#include "scene.h"

SphereData earth;
ShipData spaceship;

static void MissionOpen()
{


  ClearBuffer((vu8 *)MEM_VRAM_OBJ);

  REG_DISPCNT = VIDEO_MODE_AFFINE |/* VIDEO_BG2_ENABLE |*/ VIDEO_BG3_ENABLE | VIDEO_OBJ_ENABLE | VIDEO_OBJ_MAPPING_1D;
  REG_BG2CNT = BG_TILE_8BPP | BG_PRIORITY(1);
  REG_BG3CNT = BG_TILE_8BPP | BG_PRIORITY(0) | BG_GFX_BASE(2) | BG_MAP_BASE(8);

  FontInit();
  LabelInit(&default_font);

  //EarthInit();
  // SpaceshipInit();
  SpaceshipInit();

  InterfaceInit();
  spaceship.pitch = 0; 
  spaceship.spin = 0;

}

static void MissionUpdate()
{
  if (~(REG_KEYINPUT)&KEY_UP) {
    earth.pitch++;
  }

  if(inputKeysReleased(KEY_LEFT)) {
    int i;
    Stage *stage = &stages[stage_current];
    for(i = 0; i < stage->size && stage->items[i].consumable_amount > 0; ++i) {
      stage->items[i].consumable_amount -= 1;
    }
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
  
    if (~(REG_KEYINPUT)&KEY_LEFT) {
    spaceship.pitch--;
	if (spaceship.pitch < -255){
		spaceship.pitch = -255;
	}
  }

  if (~(REG_KEYINPUT)&KEY_RIGHT) {
    spaceship.pitch++;
	if (spaceship.pitch > 255){
		spaceship.pitch = 255;
	}
  }
      if (~(REG_KEYINPUT)&KEY_DOWN) {
    spaceship.spin-=4;
  }

  if (~(REG_KEYINPUT)&KEY_UP) {
    spaceship.spin+=4;
  }

  // Due to the time it take to compute it cannot be move in draw
  // EarthDraw(&earth);
  TransferBuffer(spaceship_buffer, GFX_BASE_ADDR(2));
  ClearBuffer(spaceship_buffer);
  SpaceshipDraw(spaceship.pitch, spaceship.spin);

  InterfaceUpdate(7, 8, 3, 1234, 2345, 5);
}

static void MissionDraw() {
  InterfaceDraw();
}

static void MissionVBlank() {}

static void MissionClose() {}

Scene mission_scene = {
    .Open = MissionOpen, .Update = MissionUpdate, .Draw = MissionDraw, .VBlank = MissionVBlank, .Close = MissionClose};
