#ifndef INTERFACE_TYPES_H
#define INTERFACE_TYPES_H

#include "../core/types.h"

typedef struct StageItem{
	u16 tile_id;
	s16 consumable_amount;
} StageItem;

typedef struct Stage {
	StageItem *items;
	u16 height;
	bool in_queue;
} Stage;

#endif
