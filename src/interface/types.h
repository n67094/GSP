#ifndef INTERFACE_TYPES_H
#define INTERFACE_TYPES_H

#include "../core/types.h"

typedef struct StageItem{
	Object *icon;
	Object *caret;
	Object *gauge;
	u16 tile_id;
	int consumable_amount;
} StageItem;

typedef struct Stage {
	Object *number;
	Object *separator;
	StageItem *items;
	u16 height; // pre computed value to save cycles, in px
	u16 size;
} Stage;

typedef enum FacecamState {
	FACECAM_STATE_NORMAL,
	FACECAM_STATE_HAPPY,
	FACECAM_STATE_SICK,
} FacecamState;

typedef struct Facecam {
	Object *top;
	Object *bottom;
	u16 top_tile;
	u16 bottom_tile;
} Facecam;

#endif
