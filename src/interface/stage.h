#ifndef INTERFACE_STAGE_H
#define INTERFACE_STAGE_H

#include "types.h"

#include "interface-data.h"

#define STAGES_SIZE 4

#define STAGE_MAX_HEIGHT 100
#define STAGE_SEPARATOR_HEIGHT 8
#define STAGE_ICON_HEIGHT 16

extern int stage_previous;
extern int stage_current;

extern int stage_star;
extern int stage_end;

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
		.height = 24,
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
		.height = 24,
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
		.height = 40,
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
		.height = 56
	}
};

/*
 * return the number of object taken on the oam_buffer
 */
int StageInit(Object *oam_buffer, u32 oam_start);

void StageUpdate();

void StageNext();

#endif
