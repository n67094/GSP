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

  LOG_DEBUG("OKI");

  char speed_str[7];
  int speed_bcd = UtilsBCD(245);

  LOG_DEBUG("OKI 1");
  UtilsBCDtoA(speed_bcd, speed_str);
  LOG_DEBUG("OKI 2");

  LOG_DEBUG("speed: %s", speed_str);

  LabelPrint(speed_str, 10, 10, 60, object_buffer, 1);

  // this is here cause it just update values in OAM is not a drawing which shoul happend in Draw function
  // LabelPrint("This is a test", 10, 20, 71, object_buffer, 1);
  // LabelPrint("Test:.", 10, 30, 85, object_buffer, 1);
}
