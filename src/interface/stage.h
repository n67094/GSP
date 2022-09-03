#ifndef INTERFACE_STAGE_H
#define INTERFACE_STAGE_H

#include "types.h"

#define STAGES_SIZE 5

extern u32 stage_current;

static const Stage *stages = (Stage[]){
	{
		.items = (StageItem[]) {
			{
				.tile_id = 0,
				.consumable_amount = -1
			}
		},
		.height = 0,
		.in_queue = true
	},
};

void StageInit();

void StageUpdate();

#endif
