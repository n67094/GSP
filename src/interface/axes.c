#include "../../data/sprites/axes.tiles.h"

#include "../global.h"
#include "../types.h"
#include "../core/memory.h"
#include "../core/object.h"
#include "../core/tile.h"
#include "interface-data.h"
#include "axes.h"
#include "seven/video/object.h"

void AxesInit() {
  MemCpy32(&TILE4_CHARBLOCKS[4][0], axes_tiles, axes_tiles_size);
  //MemCpy32(&TILE4_MEM[4][0], axes_tiles, axes_tiles_size);

  Object *label = &object_buffer[OAM_AXES_LABEL];
  // Object *roll_cursor = &object_buffer[OAM_AXES_ROLL_CURSOR];
  // Object *pitch_cursor = &object_buffer[OAM_AXES_PITCH_CURSOR];
  // Object *yaw_curosr = &object_buffer[OAM_AXES_YAW_CURSOR];

  // LABEL
  ObjectSetAttr(label,
      ATTR0_HORIZONTAL,
      ATTR1_SIZE_64x32,
      ATTR2_PALETTE_BANK(0) | ATTR2_TILE_ID(TILE_AXES_LABEL)
  );
  ObjectSetPos(label, POS_AXES_LABEL_X, POS_AXES_LABEL_Y);
}

void AxesUpdate(s32 roll, s32 pitch, s32 yaw) {

}
