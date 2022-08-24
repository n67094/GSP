#include <seven/hw/video.h>

#include "../core/memory.h"
#include "../entity/earth.h"
#include "../renderer/sphere.h"
#include "../types.h"

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

void SpaceshipDraw(s32 pitch2, s32 spin){
	u32 radius = 32;
	u32 xPos = 64;
	u32 yPos = 64;
	static s32 pitch = 0;
	static u32 pitchneg = 0;
	if(pitchneg){
		pitch--;
		if (pitch < -255){
			pitchneg = 0;
			pitch = -255;
		}
	}
	else{
		pitch++;
		if (pitch > 255){
			pitchneg = 1;
			pitch = 255;
		}
	}

	
	SetupPosTableCylinder(pos_table_1, TrigGetSin(pitch), radius, xPos, yPos);
	
	for (u32 i = 0; i < 128; i++){
		spaceship_buffer[(i * 128) + (pos_table_1[i] & 0xff)] = 0xa0;
	}
}