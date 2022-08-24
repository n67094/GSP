#include <seven/video/bg_bitmap.h>
#include <seven/video/prelude.h>
#include <seven/hw/input.h>
#include <seven/hw/memory.h>
#include <seven/hw/video.h>

#include "../entity/earth.h"
#include "../interface/interface.h"
#include "../renderer/sphere.h"
#include "../entity/spaceship.h"
#include "../types.h"

#include "scene-manager.h"
#include "../global.h"

SphereData earth;
ShipData spaceship;

static void MissionOpen()
{
  REG_DISPCNT = VIDEO_MODE_AFFINE | VIDEO_BG2_ENABLE | VIDEO_BG3_ENABLE | VIDEO_OBJ_MAPPING_1D;
  REG_BG2CNT = BG_TILE_8BPP | BG_PRIORITY(1);
  REG_BG3CNT = BG_TILE_8BPP | BG_PRIORITY(0) | BG_GFX_BASE(2) | BG_MAP_BASE(8);

  EarthInit();
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
	ClearBuffer(spaceship_buffer);
	SpaceshipDraw(spaceship.pitch, spaceship.spin);
  	TransferBuffer(spaceship_buffer, GFX_BASE_ADDR(2)); 
	
	
}

static void MissionDraw() {

}

static void MissionVBlank() {}

static void MissionClose() {}

Scene mission_scene = {
    .Open = MissionOpen, .Update = MissionUpdate, .Draw = MissionDraw, .VBlank = MissionVBlank, .Close = MissionClose};
