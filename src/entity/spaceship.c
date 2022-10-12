#include <seven/hw/video.h>
#include <seven/hw/input.h>

#include "../../data/bitmaps/capsule.tiles.h"
#include "../../data/bitmaps/wall.tiles.h"

#include "../core/memory.h"
#include "../core/trig.h"

#include "../entity/earth.h"

#include "../renderer/sphere.h"

#include "../types.h"
#include "../global.h"

#include "spaceship-data.h"
#include "spaceship.h"

static void SpaceshipInitTileMap() {
  vu16 *tilemap_ptr = (u16 *)MAP_BASE_ADDR(8);

  for (u32 i = 0; i < 128; ++i) {
    tilemap_ptr[i] = (((i << 1) + 1) << 8) + (i << 1);
  }
}

void SpaceshipInit(){
	SpaceshipInitTileMap();
	
	//These palette entries are temporary. The unified sphere/spaceship palette will be copied instead once it is ready
	for(u32 i = 0; i < 16; i++) {
		((u16 *)0x05000000)[i] = ((u16 *)capsule_palette)[i];
	}
	
	*(u16 *)0x05000140 = 0x3ff; //Setting the color Yellow to palette entry 0xa0
	*(u16 *)0x05000142 = 0x1f; //Setting the color Red to palette entry 0xa1
}

void SpaceshipDraw(s32 pitch, s32 spin){
	
	static u32 univ_scale = 0x100; //this scale is calculated based on the dimensions of the ship,
	//so that the entire ship will always fit inside the bg, no matter the orientation.
	
	u32 pos_x = 64;
	u32 pos_y = 64;

	if (~(REG_KEYINPUT)&KEY_L) {
		univ_scale--;
		if (univ_scale < 1){
			univ_scale = 1;
		}
	}

	if (~(REG_KEYINPUT)&KEY_R) {
		univ_scale++;
		if (univ_scale > 0x170){
			univ_scale = 0x170;
		}
	}
	vu8 debug = 1;
	
	spin = spin & 0x3ff; //get the spin into a range of 0-2pi
	spin += 0x200;
	
	// why not on the header ?
	// extern SegmentData capsule_wall; 				//This will eventually be referenced indirectly by the data for the ship.
	const SegmentData *current_segment = &capsule_wall;	//But for testing, I'm just putting it here.
	
	u32 radius_1 = (current_segment->radius_1 * univ_scale) >> 8;
	u32 radius_2 = (current_segment->radius_2 * univ_scale) >> 8;
	//while(debug);
	s32 height = (current_segment->height * univ_scale) >> 8;
	u32 gfx_width = current_segment->gfx_width;
	u32 gfx_height = current_segment->gfx_height;
	cu8 *gfxPtr = current_segment->gfx_data + (spin >> (10 - gfx_height) << gfx_width);

	/*SetupPosTableCylinder(TrigGetSin(pitch), radius_1, xPos);
	u32 horiz_len = (TrigGetCos(pitch) * height) >> 16;
	DrawCylinderWall(gfxPtr, texture_width, texture_height, spaceship_buffer + (yPos * 128) - (radius_1 * 128), horiz_len, radius_1);
	*/
	SetupPosTableCone(pitch, radius_1, radius_2, pos_x, height);
	DrawConeWallBack(gfxPtr, spaceship_buffer + (pos_y << 7), gfx_width, gfx_height);
	DrawConeWall(gfxPtr, spaceship_buffer + (pos_y << 7), gfx_width, gfx_height);
}