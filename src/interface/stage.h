#ifndef INTERFACE_STAGE_H
#define INTERFACE_STAGE_H

#include "types.h"

#define STAGES_SIZE 5

extern u32 stage_previous;
extern u32 stage_current;

// on screen stages index
extern u32 stage_visible_start;
extern u32 stage_visible_end;
extern bool stage_visible_consumable;

static const Stage *stages = (Stage[]){
	{
		.items = (StageItem[]) {
			{
				.tile_id = 0,
				.consumable_amount = -1
			}
		},
		.height = 0,
	},
};

void StageInit();

void StageUpdate();

void StageNext();

#endif
