#include <seven/hw/video.h>
#include <seven/hw/input.h>
#include <seven/svc/affine.h>

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
	extern u8 capsulePal[32];
	for(u32 i = 0; i < 16; i++) {
	 ((u16 *)0x05000020)[i] = ((u16 *)capsulePal)[i + 16];
	}
	
	// MemCpy16(BG_PALETTE_BANK[1], capsulePal, 32);
	
	*(u16 *)0x05000140 = 0x3ff; //Setting the color Yellow to palette entry 0xa0
	*(u16 *)0x05000142 = 0x1f; //Setting the color Red to palette entry 0xa1
}

void SpaceshipDraw(ShipData *Spaceship, struct BgAffineDstData *Bg3AffineTemp, CameraData *Camera, s32 throttle){
	ColumnData *current_column;
	static u32 frameCount = 0;
	const PartData *current_part;
	const SegmentData *current_segment;
	ColumnData *sorted_columns[Spaceship->num_columns];
	vu8 *bufferPtr;
	RotationMatrix result_matrix;
	s32 spin; //spin of the rocket
	s32 pitch; //pitch of the rocket
	s32 rotate; //the rotation of the background
	u32 pos_x;
	u32 pos_y = 64;
	u32 update_matrix;
	s32 delta_roll;
	s32 delta_pitch;
	s32 delta_yaw;
	
	
	static s32 univ_scale = 0x9e; //this scale is calculated based on the dimensions of the ship,
	//so that the entire ship will always fit inside the bg, no matter the orientation.
	
	frameCount++;
	
	update_matrix = 1;
	
	Spaceship->roll_pos += Spaceship->roll_vel;
	Spaceship->pitch_pos += Spaceship->pitch_vel;
	Spaceship->yaw_pos += Spaceship->yaw_vel;
	
	/*if(Spaceship->roll_pos >> 8){
		delta_roll = Spaceship->roll_pos >> 8;
		Spaceship->roll_pos -= (delta_roll << 8);
		update_matrix = 1;
	}
	if(Spaceship->pitch_pos >> 8){
		delta_pitch = Spaceship->pitch_pos >> 8;
		Spaceship->pitch_pos -= (delta_pitch << 8);
	}
	if(Spaceship->yaw_pos >> 8){
		delta_yaw = Spaceship->yaw_pos >> 8;
		Spaceship->yaw_pos -= (delta_yaw << 8);
	}
	if(REG_KEYINPUT&KEY_B){
		if (~(REG_KEYINPUT)&KEY_LEFT) {
			Spaceship->alpha += 5;
		}
		
		if (~(REG_KEYINPUT)&KEY_RIGHT) {
			Spaceship->alpha -= 5;
		}
		
		if (~(REG_KEYINPUT)&KEY_DOWN) {
			Spaceship->beta += 3;		
		}
		
		if (~(REG_KEYINPUT)&KEY_UP) {
			Spaceship->beta -= 3;
		}
		
		if (~(REG_KEYINPUT)&KEY_R) {
			Spaceship->gamma += 5;
		}
		
		if (~(REG_KEYINPUT)&KEY_L) {
			Spaceship->gamma -= 5;
		}
	}*/
	Spaceship->beta &= 0x3ff; //clamp into range of 0 to 2pi
	Spaceship->alpha &= 0x3ff; //clamp into range of 0 to 2pi
	Spaceship->gamma &=0x3ff; //clamp into range of 0 to 2pi
	
	CreateMatrix(Spaceship->rotation_matrix, Spaceship->alpha, Spaceship->beta, Spaceship->gamma); //create the rotation matrix for the spaceship
	
	MultiplyCameraMatrix(&result_matrix, Camera->spin, Camera->pitch, Spaceship->rotation_matrix); 
	
	rotate = GetRotate(&result_matrix);
	spin = GetSpin(&result_matrix, rotate);
	pitch = GetPitch(&result_matrix);
	
	PrepareAffine(rotate, Bg3AffineTemp); //calculate the contents of the affine registers
	
	spin = spin & 0x3ff; 
	spin += 0x200; //get the spin into a range of pi-3pi
	
	SortColumns(sorted_columns, Spaceship, spin); //sort the columns from back to front.
	
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
					u32 gfx_width = current_segment->gfx_width;
					u32 gfx_height = current_segment->gfx_height;
					s32 height;
					cu8 *gfxPtr;
					if(type & 0x4){ //if this is an engine exhaust segment
						if(throttle == 0){
							if(current_segment->height <= 0){ //if the smaller radius of the cone is towards the top
								pos_x = initialx_pos + ((total_height * univ_scale * TrigGetCos(pitch)) >> 16);
								total_height += current_segment->height;
							}
							else{ //if the smaller radius of the cone is towards the bottom
								pos_x = initialx_pos + (((total_height - current_segment->height) * univ_scale * TrigGetCos(pitch)) >> 16);
								total_height -= current_segment->height;
							}
							continue;
						}
						height = (current_segment->height * univ_scale * throttle >> 16);
						u32 modified_spin = ((spin + ((frameCount & 0x7) << 7)) & 0x3ff) + 0x200;
						gfxPtr = current_segment->gfx_data + (modified_spin >> (10 - gfx_height) << gfx_width);
					}
					else{
						height = (current_segment->height * univ_scale) >> 8;
						gfxPtr = current_segment->gfx_data + (spin >> (10 - gfx_height) << gfx_width);
					}
					
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
					u32 gfx_width = current_segment->gfx_width;
					u32 gfx_height = current_segment->gfx_height;
					s32 height;
					cu8 *gfxPtr;
					if(type & 0x4){ //if this is an engine exhaust segment
						if(throttle == 0){
							if(current_segment->height <= 0){ //if the smaller radius of the cone is towards the top
								pos_x = initialx_pos + ((total_height * univ_scale * TrigGetCos(pitch)) >> 16);
								total_height += current_segment->height;
							}
							else{ //if the smaller radius of the cone is towards the bottom
								pos_x = initialx_pos + (((total_height - current_segment->height) * univ_scale * TrigGetCos(pitch)) >> 16);
								total_height -= current_segment->height;
							}
							continue;
						}
						height = (current_segment->height * univ_scale * throttle >> 16);
						u32 modified_spin = ((spin + ((frameCount & 0x7) << 7)) & 0x3ff) + 0x200;
						gfxPtr = current_segment->gfx_data + (modified_spin >> (10 - gfx_height) << gfx_width);
					}
					else{
						height = (current_segment->height * univ_scale) >> 8;
						gfxPtr = current_segment->gfx_data + (spin >> (10 - gfx_height) << gfx_width);
					}
					
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
						if(((pitch ^ height) <= 0) || (type & 8)){//if pitch and height have the same signs, also render the backside
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

void SortColumns(ColumnData *sorted_columns[], ShipData *Spaceship, s32 spin){
	s32 distance_table[Spaceship->num_columns];
	s32 temp;
	for(u32 i = 0; i < Spaceship->num_columns; i++){ //calculate the distance of each column
		sorted_columns[i] = Spaceship->columns_ptr[i];
		distance_table[i] = TrigGetCos(spin + Spaceship->columns_ptr[i]->angle) * Spaceship->columns_ptr[i]->radius;
	}
	for(u32 i = 0; i < Spaceship->num_columns; i++){//selection sort because I am lazy
		s32 lowest = i;
		for(u32 j = i+1; j < Spaceship->num_columns; j++){
			if (distance_table[j] > distance_table[lowest]){
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

void CreateMatrix(RotationMatrix *RM, s32 alpha, s32 beta, s32 gamma){

	s32 cr = TrigGetCos(alpha);
	s32 sr = TrigGetSin(alpha);
	s32 cp = TrigGetCos(beta);
	s32 sp = TrigGetSin(beta);
	s32 cy = TrigGetCos(gamma);
	s32 sy = TrigGetSin(gamma);
	

	//combine the rotations into a new rotation matrix
	RM->X1 = cp*cr >> 8;
	RM->X2 = cp*sr >> 8;
	RM->X3 = -sp;
	RM->Y1 = (sy*sp*cr >> 8) - cy*sr >> 8;
	RM->Y2 = (sy*sp*sr >> 8) + cy*cr >> 8;
	RM->Y3 = sy*cp >> 8;
	RM->Z1 = (cy*sp*cr >> 8) + sy*sr >> 8; 
	RM->Z2 = (cy*sp*sr >> 8) - sy*cr >> 8;
	RM->Z3 = cy*cp >> 8;
}

void MultiplyCameraMatrix(RotationMatrix *Mdest, s32 spin, s32 pitch, RotationMatrix *M2){
	RotationMatrix temp_rot;
	RotationMatrix *temp = &temp_rot;
	
	s32 cs = TrigGetCos(spin);
	s32 cp = TrigGetCos(pitch);
	s32 ss = TrigGetSin(spin);
	s32 sp = TrigGetSin(pitch);
	
	temp->X1 = cs;
	temp->X2 = sp * ss >> 8;
	temp->X3 = cp * ss >> 8;
	temp->Y1 = 0;
	temp->Y2 = cp;
	temp->Y3 = -sp;
	temp->Z1 = -ss;
	temp->Z2 = sp * cs >> 8;
	temp->Z3 = cs * cp >> 8;
	
	Mdest->X1 = temp->X1 * M2->X1 + temp->Y1 * M2->X2 + temp->Z1 * M2->X3 >> 8;
	Mdest->X2 = temp->X2 * M2->X1 + temp->Y2 * M2->X2 + temp->Z2 * M2->X3 >> 8;
	Mdest->X3 = temp->X3 * M2->X1 + temp->Y3 * M2->X2 + temp->Z3 * M2->X3 >> 8;
	Mdest->Y1 = temp->X1 * M2->Y1 + temp->Y1 * M2->Y2 + temp->Z1 * M2->Y3 >> 8;
	Mdest->Y2 = temp->X2 * M2->Y1 + temp->Y2 * M2->Y2 + temp->Z2 * M2->Y3 >> 8;
	Mdest->Y3 = temp->X3 * M2->Y1 + temp->Y3 * M2->Y2 + temp->Z3 * M2->Y3 >> 8;
	Mdest->Z1 = temp->X1 * M2->Z1 + temp->Y1 * M2->Z2 + temp->Z1 * M2->Z3 >> 8;
	Mdest->Z2 = temp->X2 * M2->Z1 + temp->Y2 * M2->Z2 + temp->Z2 * M2->Z3 >> 8;
	Mdest->Z3 = temp->X3 * M2->Z1 + temp->Y3 * M2->Z2 + temp->Z3 * M2->Z3 >> 8;
}

void PrepareAffine(s32 rotate, struct BgAffineDstData *Bg3AffineTemp){
	struct BgAffineSrcData Bg3Affine = {0x4000, 0x4000, 120, 80, 0x100, 0x100, (rotate & 0x3ff) << 6};

	svcBgAffineSet(&Bg3Affine, Bg3AffineTemp, 1);
}

s32 GetRotate(RotationMatrix *RM){
	s32 rotate;
	if((RM->X1 <= 0x20) && (RM->X1 >= -0x20)){
		if((RM->X1 * RM->X1 + RM->X2 * RM->X2) <= 0x400){ //if the spaceship is pointing towards or away from the camera
			if(RM->X1 * RM->X1 < RM->X2 * RM->X2){
				rotate = TrigGetArcCos(TrigGetInverse(TrigGetPythHyp(RM->X1 * RM->X1 + RM->X2 * RM->X2)) * RM->X1 >> 8);
				if(RM->X2 > 0){
					rotate = 0x200 - rotate;
				}
				else{
					rotate -= 0x200;
				}
			}
			else{
				rotate = TrigGetArcSin(TrigGetInverse(TrigGetPythHyp(RM->X1 * RM->X1 + RM->X2 * RM->X2)) * RM->X2 >> 8);
				if(RM->X1 > 0){
					rotate = 0x200 - rotate;
				}
				rotate -= 0x200;
			}
		}
		else{
			rotate = TrigGetArcCos(RM->X1 * TrigGetInvPythSqrt(RM->X3) >> 8);
			if(RM->X2 < 0){
			rotate = -rotate;
			}
		}
	}
	else{
		rotate = TrigGetArcSin(RM->X2 * TrigGetInvPythSqrt(RM->X3) >> 8);
		if(RM->X1 < 0){
			rotate = 0x200 - rotate;
		}
	}
	return rotate & 0x3ff;
}

s32 GetPitch(RotationMatrix *RM){
	if((RM->X1 * RM->X1 + RM->X2 * RM->X2) <= 0x400){ //if the ship is really close to pointing directly at or away from the screen
		s32 temp = TrigGetPythHyp(RM->X1 * RM->X1 + RM->X2 * RM->X2); //use this more precise formula
		if(RM->X3 > 0){
			return -TrigGetArcCos(temp);
		}	
		return TrigGetArcCos(temp);
	}
	return TrigGetArcSin(-RM->X3); //otherwise, use this general case formula
}

s32 GetSpin(RotationMatrix *RM, s32 rotate){
	s32 spin;
	s32 phi;
	if((RM->X1 * RM->X1 + RM->X2 * RM->X2) <= 0x800){//if the ship is really close to pointing directly at or away from the screen
		if(RM->Y2 * RM->Y2 < RM->Y1 * RM->Y1){
			phi = TrigGetArcCos(RM->Y2);
		}
		else{
			phi = TrigGetArcSin(RM->Y1);
			if(RM->Y1 > 0){
				phi = -phi;
			}
			if(RM->Z1 > 0){
				phi = -phi;
			}
			else{
				phi += 0x200;
			}
			if(RM->X3 > 0){
				phi = 0x200 - phi;
			}
		}
		if(RM->X3 < 0){
			if (RM->Y1 > 0){
				phi = -phi;
			}
			spin = phi - rotate + 0x200;
		}
		else{
			if(RM->Y1 < 0){
				phi = -phi;
			}
			spin = phi + rotate + 0x200;
		}
	}
	else if((RM->Z3 >= -0x20) && (RM->Z3 <= 0x20)){
		spin = TrigGetArcCos(RM->Z3 * TrigGetInvPythSqrt(RM->X3) >> 8);
		if(RM->Y3 > 0){
			spin = -spin;
		}
		spin -= 0x200;
	}
	else{
		spin = TrigGetArcSin(RM->Y3 * TrigGetInvPythSqrt(RM->X3) >> 8);
		if(RM->Z3 > 0){
			spin = 0x200 - spin;
		}
	}

	return spin;
}
