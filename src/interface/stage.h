#ifndef INTERFACE_STAGE_H
#define INTERFACE_STAGE_H

#include "types.h"

#include "interface-data.h"

#define STAGES_SIZE 4

extern u32 stage_previous;
extern u32 stage_current;

// on screen stages index
extern u32 stage_visible_start;
extern u32 stage_visible_end;
extern bool stage_visible_consumable;

static Stage *stages = (Stage[]){
	// PARACHUTE
	{
		.items = (StageItem[]) {
			{
				.tile_id = TILE_PARACHUTE,
				.consumable_amount = -1
			}
		},
		.size = 1,
		.height = 0,
	},
	// DECOUPLER
	{
		.items = (StageItem[]) {
			{
				.tile_id = TILE_DECOUPLER_V,
				.consumable_amount = -1
			}
		},
		.size = 1,
		.height = 0,
	},
	// ENGINE + DECOUPLER
	{
		.items = (StageItem[]) {
			{
				.tile_id = TILE_ENGINE,
				.consumable_amount = 100
			},
			{
				.tile_id = TILE_DECOUPLER_H,
				.consumable_amount = -1
			}
		},
		.size = 2,
		.height = 0,
	},
	// ENGINE
	{
		.items = (StageItem[]) {
			{
				.tile_id = TILE_ENGINE,
				.consumable_amount = 100
			},
			{
				.tile_id = TILE_ENGINE,
				.consumable_amount = 100
			},
			{
				.tile_id = TILE_ENGINE,
				.consumable_amount = 100
			}
		},
		.size = 3,
		.height = 0,
	}
};

/*
 * return the number of object taken on the oam_buffer
 */
int StageInit(Object *oam_buffer, u32 oam_start);

void StageUpdate();

void StageNext();

#endif
