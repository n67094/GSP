#include <seven/video/bg_bitmap.h>
#include <seven/video/prelude.h>
#include <seven/hw/input.h>
#include <seven/hw/memory.h>
#include <seven/hw/video.h>
#include <seven/hw/waitstate.h>

#include "../debug/log.h"

#include "../core/label.h"

#include "../entity/earth.h"
#include "../entity/spaceship.h"
#include "../entity/spaceship-data.h"

#include "../interface/interface.h"
#include "../interface/stage.h"

#include "../renderer/sphere.h"

#include "../types.h"
#include "../global.h"

#include "scene.h"

#define ANGLE_ACC 0x10
#define CAMERA_SPEED 10

SphereData earth;

RotationMatrix spaceship_rotation;
RotationMatrix camera_rotation;

ShipData spaceship = {
	.num_columns = 4, .columns_ptr = ship_columns, .length = 200, .rotation_matrix = &spaceship_rotation,
};

CameraData camera = {
	.rotation_matrix = &camera_rotation, .spin = 0, .pitch = 0
};

struct BgAffineDstData Bg3AffineTemp;
struct BgAffineDstData *Bg3AffineReg = (struct BgAffineDstData *)0x4000030;

static void MissionOpen()
{
  REG_DISPCNT = VIDEO_MODE_AFFINE | VIDEO_BG2_ENABLE | VIDEO_BG3_ENABLE | VIDEO_OBJ_ENABLE | VIDEO_OBJ_MAPPING_1D;
  REG_BG2CNT = BG_TILE_8BPP | BG_PRIORITY(1);
  REG_BG3CNT = BG_TILE_8BPP | BG_PRIORITY(0) | BG_GFX_BASE(2) | BG_MAP_BASE(8);

  EarthInit();
  SpaceshipInit();

  InterfaceInit();
  
  REG_WAITCNT = WAIT_ROM_N_2 | WAIT_ROM_S_1 | WAIT_PREFETCH_ENABLE;

}

static void MissionUpdate()
{
	//unfortunately, I did not have time to implement staging...
  /*if(inputKeysReleased(KEY_LEFT)) { 
    int i;
    Stage *stage = &stages[stage_current];
    for(i = 0; i < stage->size && stage->items[i].consumable_amount > 0; ++i) {
      stage->items[i].consumable_amount -= 1;
    }
  }

  if(inputKeysReleased(KEY_A)) {
    StageNext();
  }*/
  
  if(~(REG_KEYINPUT)&KEY_B){ //while B is held, control the camera
    if (~(REG_KEYINPUT)&KEY_LEFT){
	  camera.spin -= CAMERA_SPEED;
    }
  
    if(~(REG_KEYINPUT)&KEY_RIGHT){
	  camera.spin += CAMERA_SPEED;
    }
  
    if (~(REG_KEYINPUT)&KEY_UP){
	  camera.pitch += CAMERA_SPEED;
	  if(camera.pitch > 0xff){
		camera.pitch = 0xff;
	  }
    }
  
    if(~(REG_KEYINPUT)&KEY_DOWN){
	  camera.pitch -= CAMERA_SPEED;
	  if(camera.pitch < -0xff){
		camera.pitch = -0xff;
	  }
    }
  }
  else{ //if B is not held, control the spaceship
    if (~(REG_KEYINPUT)&KEY_LEFT) {
      spaceship.yaw_vel += ANGLE_ACC;
    }

    if (~(REG_KEYINPUT)&KEY_RIGHT) {
      spaceship.yaw_vel -= ANGLE_ACC;
    }
    
    if (~(REG_KEYINPUT)&KEY_DOWN) {
      spaceship.pitch_vel += ANGLE_ACC;
    }
  
    if (~(REG_KEYINPUT)&KEY_UP) {
      spaceship.pitch_vel -= ANGLE_ACC;
   }
  
    if (~(REG_KEYINPUT)&KEY_R) {
      spaceship.roll_vel += ANGLE_ACC;
    }
	
    if (~(REG_KEYINPUT)&KEY_L) {
      spaceship.roll_vel -= ANGLE_ACC;
    }
  }

   InterfaceUpdate(7, 8, 3, 1234, 2345, 5);
  
  InterfaceDraw();
  ClearBuffer(spaceship_buffer);
  SpaceshipDraw(&spaceship, &Bg3AffineTemp, &camera);
  EarthDraw(&earth);
}

static void MissionDraw() {
  TransferBuffer(spaceship_buffer, GFX_BASE_ADDR(2));
  Bg3AffineReg->h_diff_x = Bg3AffineTemp.h_diff_x;
  Bg3AffineReg->v_diff_x = Bg3AffineTemp.v_diff_x;
  Bg3AffineReg->h_diff_y = Bg3AffineTemp.h_diff_y;
  Bg3AffineReg->v_diff_y = Bg3AffineTemp.v_diff_y;
  Bg3AffineReg->start_x = Bg3AffineTemp.start_x;
  Bg3AffineReg->start_y = Bg3AffineTemp.start_y;
}

static void MissionVBlank() {}

static void MissionClose() {}

Scene mission_scene = {
    .Open = MissionOpen, .Update = MissionUpdate, .Draw = MissionDraw, .VBlank = MissionVBlank, .Close = MissionClose};
