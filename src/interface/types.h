#ifndef INTERFACE_TYPES_H
#define INTERFACE_TYPES_H

#include "../core/types.h"

typedef struct StageItem{
	Object *icon;
	Object *caret;
	Object *gauge;
	u16 tile_id;
	s16 consumable_amount;
} StageItem;

typedef struct Stage {
	Object *number;
	Object *separator;
	StageItem *items;
	u16 height; // pre computed value to save cycles, in px
	u16 size;
} Stage;

#endif
