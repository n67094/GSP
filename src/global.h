#ifndef GLOBAL_H
#define GLOBAL_H

#include "types.h"

// PALETTE ID
#define OBJ_PALETTE_UI_ID 0

// OAM ID
// ======
#define OAM_AXES_LABEL 0
#define OAM_AXES_ROLL_CURSOR 1
#define OAM_AXES_PITCH_CURSOR 2
#define OAM_AXES_YAW_CURSOR 3

// TILE ID
// =======
#define TILE_AXES_LABEL 0
#define TILE_AXES_ROLL_CURSOR 0
#define TILE_AXES_PITCH_CURSOR 0
#define TILE_AXES_YAW_CURSOR 0

extern Object object_buffer[128];

extern ShipData ship_data;

#endif
