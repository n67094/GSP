#include <seven/video/object.h>

#include "../../data/sprites/metrics-1.tiles.h"
#include "../../data/sprites/metrics-2.tiles.h"

#include "../core/memory.h"
#include "../core/object.h"
#include "../core/tile.h"
#include "../core/label.h"
#include "../core/utils.h"

#include "../debug/log.h"

#include "../global.h"
#include "../types.h"

#include "interface-data.h"
#include "metrics.h"

static void MetricsMoveThrottleCaret(u16 offset_x) {
  Object *caret = &object_buffer[OAM_THROTTLE_CARET];

  if(offset_x < 48) {
    ObjectSetPos(caret, POS_THROTTLE_CARET_X + offset_x, POS_THROTTLE_CARET_Y);
  }
}

void MetricsInit()
{
  MemCpy32(&OBJ4_CHARBLOCKS[0][TILE_METRICS_1], metrics_1_tiles, metrics_1_tiles_size);
  MemCpy32(&OBJ4_CHARBLOCKS[0][TILE_METRICS_2], metrics_2_tiles, metrics_2_tiles_size);

  Object *metrics_1 = &object_buffer[OAM_METRICS_1];
  Object *metrics_2 = &object_buffer[OAM_METRICS_2];
  Object *throttle_caret = &object_buffer[OAM_THROTTLE_CARET];

  ObjectSetAttr(metrics_1,
    OBJ_SHAPE_WIDE,
    OBJ_SIZE_64X32,
    OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_METRICS_1)
  );
  ObjectSetPos(metrics_1, POS_METRICS_1_X, POS_METRICS_2_Y);

  ObjectSetAttr(metrics_2,
    OBJ_SHAPE_WIDE,
    OBJ_SIZE_64X32,
    OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_METRICS_2)
  );
  ObjectSetPos(metrics_2, POS_METRICS_2_X, POS_METRICS_2_Y);

  ObjectSetAttr(throttle_caret,
    OBJ_SHAPE_SQUARE,
    OBJ_SIZE_8X8,
    OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_THROTTLE_CARET));
  ObjectSetPos(throttle_caret, POS_THROTTLE_CARET_X, POS_THROTTLE_CARET_Y);
}

void MetricsUpdate(u16 altitue, u16 speed, u16 throttle) {
  MetricsMoveThrottleCaret(throttle);

  char altitue_str[7];
  int altitue_bcd = UtilsBCD(altitue);
  UtilsBCDtoA(altitue_bcd, altitue_str);

  LabelPrint(altitue_str, POS_ALTITUE_X, POS_ALTITUE_Y, OAM_ALTITUE, object_buffer, 1);

  char speed_str[7];
  int speed_bcd = UtilsBCD(speed);
  UtilsBCDtoA(speed_bcd, speed_str);

  LabelPrint(speed_str, POS_SPEED_X, POS_SPEED_Y, OAM_SPEED, object_buffer, 1);
}
