#include <seven/video/object.h>

#include "../../data/sprites/axes.tiles.h"

#include "../global.h"
#include "../types.h"
#include "../core/memory.h"
#include "../core/object.h"
#include "../core/tile.h"
#include "interface-data.h"
#include "axes.h"

static void AxesMoveCaret(Object *caret, u16 origin_x, u16 origin_y, s16 offset_x) {
  if(offset_x > -7 && offset_x < 7) {
    ObjectSetPos(caret, origin_x + offset_x, origin_y);
  }
}

void AxesInit() {
  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_AXES], axes_tiles, axes_tiles_size);

  Object *label = &object_buffer[OAM_AXES];
  Object *roll_caret = &object_buffer[OAM_ROLL_CARET];
  Object *pitch_caret = &object_buffer[OAM_PITCH_CARET];
  Object *yaw_caret = &object_buffer[OAM_YAW_CARET];

  ObjectSetAttr(label,
    OBJ_SHAPE_WIDE,
    OBJ_SIZE_64X32,
    OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_AXES)
  );
  ObjectSetPos(label, POS_AXES_X, POS_AXES_Y);

  ObjectSetAttr(roll_caret,
    OBJ_SHAPE_SQUARE,
    OBJ_SIZE_8X8,
    OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_CARET)
  );
  ObjectSetPos(roll_caret, POS_ROLL_CARET_X, POS_ROLL_CARET_Y);

  ObjectSetAttr(pitch_caret,
    OBJ_SHAPE_SQUARE,
    OBJ_SIZE_8X8,
    OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_CARET)
  );
  ObjectSetPos(pitch_caret, POS_PITCH_CARET_X, POS_PITCH_CARET_Y);

  ObjectSetAttr(
      yaw_caret, 
      OBJ_SHAPE_SQUARE, 
      OBJ_SIZE_8X8, 
      OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_CARET));
  ObjectSetPos(yaw_caret, POS_YAW_CARET_X, POS_YAW_CARET_Y);
}

void AxesUpdate(s32 roll, s32 pitch, s32 yaw) {
  Object *roll_caret = &object_buffer[OAM_ROLL_CARET];
  Object *pitch_caret = &object_buffer[OAM_PITCH_CARET];
  Object *yaw_caret = &object_buffer[OAM_YAW_CARET];

  AxesMoveCaret(roll_caret, POS_ROLL_CARET_X, POS_ROLL_CARET_Y, roll);
  AxesMoveCaret(pitch_caret, POS_PITCH_CARET_X, POS_PITCH_CARET_Y, pitch);
  AxesMoveCaret(yaw_caret, POS_YAW_CARET_X, POS_YAW_CARET_Y, yaw);
}
