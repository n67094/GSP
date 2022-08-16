#include <seven/video/object.h>

#include "../../data/sprites/axes.tiles.h"

#include "../global.h"
#include "../types.h"
#include "../core/memory.h"
#include "../core/object.h"
#include "../core/tile.h"
#include "interface-data.h"
#include "axes.h"

void AxesInit() {
  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_AXES], axes_tiles, axes_tiles_size);

  Object *label = &object_buffer[OAM_AXES];
  Object *roll_caret = &object_buffer[OAM_ROLL_CARET];
  Object *pitch_caret = &object_buffer[OAM_PITCH_CARET];
  Object *yaw_caret = &object_buffer[OAM_YAW_CARET];

  ObjectSetAttr(label,
      OBJ_HORIZONTAL,
      OBJ_SIZE_64x32,
      OBJ_PALETTE_ID(OBJ_PALETTE_0) | OBJ_TILE_ID(TILE_AXES)
  );
  ObjectSetPos(label, POS_AXES_X, POS_AXES_Y);

  ObjectSetAttr(roll_caret,
      OBJ_SQUARE,
      OBJ_SIZE_8x8,
      OBJ_PALETTE_ID(OBJ_PALETTE_0) | OBJ_TILE_ID(TILE_CARET)
  );
  ObjectSetPos(roll_caret, POS_ROLL_CARET_X, POS_ROLL_CARET_Y);

  ObjectSetAttr(pitch_caret,
      OBJ_SQUARE,
      OBJ_SIZE_8x8,
      OBJ_PALETTE_ID(OBJ_PALETTE_0) | OBJ_TILE_ID(TILE_CARET)
  );
  ObjectSetPos(pitch_caret, POS_PITCH_CARET_X, POS_PITCH_CARET_Y);

  ObjectSetAttr(yaw_caret, OBJ_SQUARE, OBJ_SIZE_8x8, OBJ_PALETTE_ID(OBJ_PALETTE_0) | OBJ_TILE_ID(TILE_CARET));
  ObjectSetPos(yaw_caret, POS_YAW_CARET_X, POS_YAW_CARET_Y);
}

void AxesUpdate(s32 roll, s32 pitch, s32 yaw) {

}
