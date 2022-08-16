#include <seven/video/object.h>

#include "../../data/sprites/metrics-1.tiles.h"
#include "../../data/sprites/metrics-2.tiles.h"

#include "../global.h"
#include "../types.h"
#include "../core/memory.h"
#include "../core/object.h"
#include "../core/tile.h"
#include "interface-data.h"
#include "metrics.h"

void MetricsInit()
{
  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_METRICS_1], metrics_1_tiles, metrics_1_tiles_size);
  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_METRICS_2], metrics_2_tiles, metrics_2_tiles_size);

  Object *metrics_1 = &object_buffer[OAM_METRICS_1];
  Object *metrics_2 = &object_buffer[OAM_METRICS_2];
  Object *throttle_caret = &object_buffer[OAM_THROTTLE_CARET];

  ObjectSetAttr(metrics_1,
      OBJ_HORIZONTAL,
      OBJ_SIZE_64x32,
      OBJ_PALETTE_ID(OBJ_PALETTE_0) | OBJ_TILE_ID(TILE_METRICS_1)
  );
  ObjectSetPos(metrics_1, POS_AXES_X, POS_AXES_Y);

  ObjectSetAttr(metrics_2, OBJ_SQUARE, OBJ_SIZE_64x32, OBJ_PALETTE_ID(OBJ_PALETTE_0) | OBJ_TILE_ID(TILE_METRICS_2));
  ObjectSetPos(metrics_2, POS_ROLL_CARET_X, POS_ROLL_CARET_Y);

  ObjectSetAttr(
      throttle_caret, OBJ_SQUARE, OBJ_SIZE_8x8, OBJ_PALETTE_ID(OBJ_PALETTE_0) | OBJ_TILE_ID(TILE_THROTTLE_CARET));
  ObjectSetPos(throttle_caret, POS_THROTTLE_CARET_X, POS_THROTTLE_CARET_Y);
}

void MetricsUpdate(s32 roll, s32 pitch, s32 yaw) {}
