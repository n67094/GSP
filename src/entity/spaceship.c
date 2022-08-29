#include <seven/hw/video.h>

#include "../core/memory.h"
#include "../entity/earth.h"
#include "../renderer/sphere.h"
#include "../types.h"
#include <seven/hw/input.h>

//#include "../../data/bitmaps/earth.palette.h"
#include "../../data/bitmaps/earth.tiles.h"

#include "spaceship.h"
#include "../global.h"
#include "../core/trig.h"

static void SpaceshipInitTileMap() {
  vu16 *tilemap_ptr = (u16 *)MAP_BASE_ADDR(8);

  for (u32 i = 0; i < 128; ++i) {
    tilemap_ptr[i] = (((i << 1) + 1) << 8) + (i << 1);
  }
}

void SpaceshipInit(){
	SpaceshipInitTileMap();
	
	//These palette entries are temporary. The unified sphere/spaceship palette will be copied instead once it is ready
	*(u16 *)0x05000140 = 0x3ff; //Setting the color Yellow to palette entry 0xa0
	*(u16 *)0x05000142 = 0x1f; //Setting the color Red to palette entry 0xa1
}

void SpaceshipDraw(s32 pitch, s32 spin){
	static u32 radius = 32;
	static u32 height = 64;
	u32 xPos = 64 - ((TrigGetCos(pitch)) / (512/height)); //This is a very inefficient calculation, but it is for demo purposes only
	u32 yPos = 64;
	u32 scale = TrigGetSec(pitch);
	
	spin = spin & 0x3ff; //get the spin into a range of 0-2pi
	spin += 0x100;
	
	
	if (scale >= 0) {
		scale = -scale;
	}
	//normally, the radius will only be set by the part data, this key controlls is for testing only
	if (~(REG_KEYINPUT)&KEY_L) {
		radius++;
		if (radius > 63){
			radius = 63;
		}
	}

	if (~(REG_KEYINPUT)&KEY_R) {
		radius--;
		if (radius < 1){
			radius = 1;
		}
	}

	SetupPosTableCylinder(TrigGetSin(pitch), radius, xPos);
	
	DrawCylinderWall(TestWallTextureBitmap + ((spin/8) * 64), 6, scale, radius, spaceship_buffer + 8192, 7);
}