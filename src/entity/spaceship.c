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
	#define texture_width 6 //these will be defined in the part data, but for testing purposes, I have them here
	#define texture_height 7 
	
	static u32 univ_scale = 0x100; //this scale is calculated based on the dimensions of the ship,
	//so that the entire ship will always fit inside the bg, no matter the orientation.
	static u32 part_radius = 32; //this will be set in the part data, but it is a variable here for testing purposes
	static u32 part_height = 64; //this will be set in the part data, but it is a variable here for testing purposes
	
	u32 horiz_len = (TrigGetCos(pitch) * univ_scale * part_height) >> 16; //this is the number of pixels the side texture will be.
	//the assembly function will combine this with the texture width to figure out the scale.
	s32 xPos = 64 - ((TrigGetCos(pitch) * horiz_len) / 512); 
	u32 yPos = 64;

	u32 radius = (part_radius * univ_scale) >> 8;
	
	spin = spin & 0x3ff; //get the spin into a range of 0-2pi
	spin += 0x100;
	
	//normally, the radius will only be set by the part data, this key controlls is for testing only
	if (~(REG_KEYINPUT)&KEY_L) {
		part_radius++;
		if (part_radius > 63){
			part_radius = 63;
		}
	}
	if (~(REG_KEYINPUT)&KEY_R) {
		part_radius--;
		if (part_radius < 1){
			part_radius = 1;
		}
	}
	if (~(REG_KEYINPUT)&KEY_A) {
		part_height++;
		if (part_height > 63){
			part_height = 63;
		}
	}
	if (~(REG_KEYINPUT)&KEY_B) {
		part_height--;
		if (part_height < 1){
			part_height = 1;
		}
	}

	SetupPosTableCylinder(TrigGetSin(pitch), radius, xPos);
	
	DrawCylinderWall(TestWallTextureBitmap + ((spin * (1 << texture_height) / 1024) * (1 << texture_width)), texture_width, texture_height,
					spaceship_buffer + (yPos * 128) - (radius * 128), horiz_len, radius);
}