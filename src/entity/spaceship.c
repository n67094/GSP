#include <seven/hw/video.h>
#include <seven/hw/input.h>

#include "../core/memory.h"
#include "../core/trig.h"

#include "../types.h"
#include "../global.h"

#include "parts-data.h"
#include "spaceship.h"
#include "spaceship-data.h"

static void SpaceshipInitTileMap() {
  vu16 *tilemap_ptr = (u16 *)MAP_BASE_ADDR(8);

  for (u32 i = 0; i < 128; ++i) {
    tilemap_ptr[i] = (((i << 1) + 1) << 8) + (i << 1);
  }
}

void SpaceshipInit(){
	SpaceshipInitTileMap();
	
	//These palette entries are temporary. The unified sphere/spaceship palette will be copied instead once it is ready
	extern u8 capsule_palette[512];
	for(u32 i = 0; i < 16; i++) {
		((u16 *)0x05000000)[i] = ((u16 *)capsule_palette)[i];
	}
	
	*(u16 *)0x05000140 = 0x3ff; //Setting the color Yellow to palette entry 0xa0
	*(u16 *)0x05000142 = 0x1f; //Setting the color Red to palette entry 0xa1
}

void SpaceshipDraw(ShipData *Spaceship){
	ColumnData *current_column;
	const PartData *current_part;
	const SegmentData *current_segment;
	ColumnData *sorted_columns[Spaceship->num_columns];
	vu8 *bufferPtr;
	s32 spin = Spaceship->spin;
	s32 pitch = Spaceship->pitch;
	u32 pos_x;
	u32 pos_y = 64;
	
	static s32 univ_scale = 0x9e; //this scale is calculated based on the dimensions of the ship,
	//so that the entire ship will always fit inside the bg, no matter the orientation.
	
	spin = spin & 0x3ff; 
	spin += 0x200; //get the spin into a range of pi-3pi
	
	SortColumns(sorted_columns, Spaceship); //sort the columns from back to front.
	
	current_column = &side_column_1;
	for(s32 k = 0; k < Spaceship->num_columns; k++){
		current_column = sorted_columns[k];
		u32 initialx_pos = 64 + ((-TrigGetCos(current_column->angle + spin) * current_column->radius * TrigGetSin(pitch) * univ_scale) >> 24);
		pos_y = 64 + ((TrigGetSin(current_column->angle + spin) * current_column->radius * univ_scale) >> 16);
		if(pitch >= 0){ //if the top of the spaceship is pointing towards the camera
			s32 total_height = current_column->length - current_column->y_pos;
			for(s32 i = current_column->num_parts - 1; i >= 0; i--){
				current_part = current_column->parts_ptr[i];
				for(s32 j = current_part->num_segments - 1; j >= 0; j--){
					current_segment = &current_part->segments_ptr[j];
					
					u32 type = current_segment->type;
					u32 radius_1 = (current_segment->radius_1 * univ_scale) >> 8;
					u32 radius_2 = (current_segment->radius_2 * univ_scale) >> 8;
					s32 height = (current_segment->height * univ_scale) >> 8;
					u32 gfx_width = current_segment->gfx_width;
					u32 gfx_height = current_segment->gfx_height;
					cu8 *gfxPtr = current_segment->gfx_data + (spin >> (10 - gfx_height) << gfx_width);
					
					if (type & 0x1){ //if this segment is a base
					//skip for now
					}
					
					else if (type & 0x2){ //if this segment is a cone wall
						if(current_segment->height <= 0){ //if the smaller radius of the cone is towards the top
							pos_x = initialx_pos + ((total_height * univ_scale * TrigGetCos(pitch)) >> 16);
							total_height += current_segment->height;
						}
						else{ //if the smaller radius of the cone is towards the bottom
							pos_x = initialx_pos + (((total_height - current_segment->height) * univ_scale * TrigGetCos(pitch)) >> 16);
							total_height -= current_segment->height;
						}
						SetupPosTableCone(pitch, radius_1, radius_2, pos_x, height);
						bufferPtr = spaceship_buffer + (pos_y << 7);
						if((pitch ^ height) <= 0){//if pitch and height have the same signs, also render the backside
							DrawConeWallBack(gfxPtr, bufferPtr, gfx_width, gfx_height);
						}
						DrawConeWall(gfxPtr, bufferPtr, gfx_width, gfx_height);
					}
					
					else{ //if this segment is a cylinder wall
						pos_x = initialx_pos + (((total_height - current_segment->height) * univ_scale * TrigGetCos(pitch)) >> 16);
						SetupPosTableCylinder(TrigGetSin(pitch), radius_1, pos_x);
						u32 horiz_len = (TrigGetCos(pitch) * height) >> 8;
						bufferPtr = spaceship_buffer + (pos_y << 7) - (radius_1 * 128);
						DrawCylinderWall(gfxPtr, gfx_width, gfx_height, bufferPtr, horiz_len, radius_1);
						total_height -= current_segment->height;
					}
				}
			}
		}
		else{//if the bottom of the spaceship is pointing towards the camera
			s32 total_height = current_column->y_pos;
			for(s32 i = 0; i < current_column->num_parts; i++){
				current_part = current_column->parts_ptr[i];
				for(s32 j = 0; j < current_part->num_segments; j++){
					current_segment = &current_part->segments_ptr[j];
					
					u32 type = current_segment->type;
					u32 radius_1 = (current_segment->radius_1 * univ_scale) >> 8;
					u32 radius_2 = (current_segment->radius_2 * univ_scale) >> 8;
					s32 height = (current_segment->height * univ_scale) >> 8;
					u32 gfx_width = current_segment->gfx_width;
					u32 gfx_height = current_segment->gfx_height;
					cu8 *gfxPtr = current_segment->gfx_data + (spin >> (10 - gfx_height) << gfx_width);
					
					if (type & 0x1){ //if this segment is a base
					//skip for now
					}
					
					else if (type & 0x2){ //if this segment is a cone wall
						if(current_segment->height <= 0){ //if the smaller radius of the cone is towards the top
							pos_x = initialx_pos - (((total_height + current_segment->height) * univ_scale * TrigGetCos(pitch)) >> 16);
							total_height += current_segment->height;
						}
						else{ //if the smaller radius of the cone is towards the bottom
							pos_x = initialx_pos - ((total_height * univ_scale * TrigGetCos(pitch)) >> 16);
							total_height -= current_segment->height;
						}
						SetupPosTableCone(pitch, radius_1, radius_2, pos_x, height);
						bufferPtr = spaceship_buffer + (pos_y << 7);
						if((pitch ^ height) <= 0){//if pitch and height have the same signs, also render the backside
							DrawConeWallBack(gfxPtr, bufferPtr, gfx_width, gfx_height);
						}
						DrawConeWall(gfxPtr, bufferPtr, gfx_width, gfx_height);
					}
					
					else{ //if this segment is a cylinder wall
						pos_x = initialx_pos - (((total_height) * univ_scale * TrigGetCos(pitch)) >> 16);
						SetupPosTableCylinder(TrigGetSin(pitch), radius_1, pos_x);
						u32 horiz_len = (TrigGetCos(pitch) * height) >> 8;
						bufferPtr = spaceship_buffer + (pos_y << 7) - (radius_1 * 128);
						DrawCylinderWall(gfxPtr, gfx_width, gfx_height, bufferPtr, horiz_len, radius_1);
						total_height -= current_segment->height;
					}
				}
			}
		}
	}
}

void SortColumns(ColumnData *sorted_columns[], ShipData *Spaceship){
	s32 distance_table[Spaceship->num_columns];
	s32 temp;
	for(u32 i = 0; i < Spaceship->num_columns; i++){ //calculate the distance of each column
		sorted_columns[i] = Spaceship->columns_ptr[i];
		distance_table[i] = TrigGetCos(Spaceship->spin + Spaceship->columns_ptr[i]->angle) * Spaceship->columns_ptr[i]->radius;
	}
	for(u32 i = 0; i < Spaceship->num_columns; i++){//selection sort because I am lazy
		s32 lowest = i;
		for(u32 j = i+1; j < Spaceship->num_columns; j++){
			if (distance_table[j] < distance_table[lowest]){
				lowest = j;
			}
		}
		temp = distance_table[i]; //swap the lowest value with the current value
		distance_table[i] = distance_table[lowest];
		distance_table[lowest] = temp;
		temp = (s32)sorted_columns[i];
		sorted_columns[i] = sorted_columns[lowest];
		sorted_columns[lowest] = (ColumnData *)temp;
	}
}