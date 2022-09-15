#include <seven/video/object.h>

#include "../../data/sprites/stage-parachute.tiles.h"
#include "../../data/sprites/stage-engine.tiles.h"
#include "../../data/sprites/stage-decoupler-h.tiles.h"
#include "../../data/sprites/stage-decoupler-v.tiles.h"
#include "../../data/sprites/stage-separator.tiles.h"
#include "../../data/sprites/stage-gauge.tiles.h"

#include "../global.h"
#include "../types.h"

#include "../core/memory.h"
#include "../core/object.h"
#include "../core/tile.h"
#include "../core/label.h"
#include "../core/font.h"

#include "../debug/log.h"

#include "interface-data.h"
#include "stage.h"
#include "types.h"

int stage_previous = STAGES_SIZE;
int stage_current = STAGES_SIZE;

int stage_start = -1;
int stage_end = -1;

static void StageHide(int start,int end) {
  int i;
  for(i = start; i < end; ++i) {
      ObjectHide(stages[i].number);
      ObjectHide(stages[i].separator);

    int j = 0;
    for(j = 0; j < stages[i].size; ++j) {
      ObjectHide(stages[i].items[j].icon);
      if(stages[i].items[j].consumable_amount > -1) {
        ObjectHide(stages[i].items[j].gauge);
        ObjectHide(stages[i].items[j].caret);
      }
    }
  }
}

int StageInit(Object *oam_buffer, u32 oam_start){
  MemCpy32(
    &TILE4_CHARBLOCKS[4][TILE_PARACHUTE],
    stage_parachute_tiles, stage_parachute_tiles_size
  );

  MemCpy32(
    &TILE4_CHARBLOCKS[4][TILE_DECOUPLER_V],
    stage_decoupler_v_tiles, stage_decoupler_v_tiles_size
  );

  MemCpy32(
    &TILE4_CHARBLOCKS[4][TILE_DECOUPLER_H],
    stage_decoupler_h_tiles, stage_decoupler_h_tiles_size
  );

  MemCpy32(
    &TILE4_CHARBLOCKS[4][TILE_ENGINE],
    stage_engine_tiles, stage_engine_tiles_size
  );

  MemCpy32(
    &TILE4_CHARBLOCKS[4][TILE_SEPARATOR],
    stage_separator_tiles, stage_separator_tiles_size
  );

  MemCpy32(
    &TILE4_CHARBLOCKS[4][TILE_GAUGE],
    stage_gauge_tiles, stage_gauge_tiles_size
  );

  // construct the whole stages position doesn't matter yet, hide sprites
  int i = STAGES_SIZE - 1;
  int object_count = 0;
  while(i >= 0) {

    // TODO HIDE sprites
    int j = 0;
    while(j < stages[i].size) {
      // the icons
      stages[i].items[j].icon = &oam_buffer[oam_start + object_count];
      ObjectSetAttr(
        stages[i].items[j].icon,
        OBJ_SHAPE_SQUARE,
        OBJ_SIZE_16X16,
        OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(stages[i].items[j].tile_id)
      );
      ObjectSetPos(stages[i].items[j].icon, 0, 0);
      ObjectHide(stages[i].items[j].icon);
      ++object_count;

      if(stages[i].items[j].consumable_amount > -1) {
        // the gauge
        stages[i].items[j].gauge = &oam_buffer[oam_start + object_count];
        ObjectSetAttr(
          stages[i].items[j].gauge,
          OBJ_SHAPE_WIDE,
          OBJ_SIZE_8X16,
          OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_GAUGE)
        );
        ObjectSetPos(stages[i].items[j].gauge, 0, 0);
        ObjectHide(stages[i].items[j].gauge);
        ++object_count;

        // the caret
        stages[i].items[j].caret = &oam_buffer[oam_start + object_count];
        ObjectSetAttr(
          stages[i].items[j].caret,
          OBJ_SHAPE_SQUARE,
          OBJ_SIZE_8X8,
          OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_STAGE_CARET)
        );
        ObjectSetPos(stages[i].items[j].caret, 0, 0);
        ObjectHide(stages[i].items[j].caret);
        ++object_count;
      }

      ++j;
    }

    // the number
    stages[i].number = &oam_buffer[oam_start + object_count];
    ObjectSetAttr(
      stages[i].number,
      OBJ_SHAPE_SQUARE,
      OBJ_SIZE_8X8,
      OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_FONT_DIGITS + i + 1)
    );
    ObjectSetPos(stages[i].number, 0, 0);
    ObjectHide(stages[i].number);
    ++object_count;

    // the separator
    stages[i].separator = &oam_buffer[oam_start + object_count];
    ObjectSetAttr(
      stages[i].separator,
      OBJ_SHAPE_WIDE,
      OBJ_SIZE_8X32,
      OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_SEPARATOR)
    );
    ObjectSetPos(stages[i].separator, 0, 0);
    ObjectHide(stages[i].separator);
    ++object_count;

    --i;
  }

  stage_previous = -1;
  stage_current = STAGES_SIZE - 1;

  return object_count;
}

void StageUpdate() {
  if(stage_current != stage_previous) {
    // Here update stages visible on screen - position on show/hide
    // from latest in the array to first

    // mask before redraw
    if(stage_previous > -1) { // if not init state
      StageHide(0, 4);
    }

    // compute what to draw
    int height = 0;
    int i = stage_current;
    while(i >= 0 && height < STAGE_MAX_HEIGHT) {
      height += stages[i].height;

      if(height < STAGE_MAX_HEIGHT) {
        --i;
      }
    }

    // draw
    stage_start = i + 1;
    stage_end = stage_current;

    LOG_DEBUG("start %d end %d", stage_start, stage_end);

    int y = 0;
    while (stage_start < stage_end + 1) {
      // number
      ObjectSetPos(
        stages[stage_start].number,
        POS_STAGE_X, 
        POS_STAGE_Y + y
      );
      ObjectUnhide(stages[stage_start].number, OBJ_MODE_REGULAR);

      // separator
      ObjectSetPos(
        stages[stage_start].separator,
        POS_STAGE_X + POS_SEPARATOR_MARGIN_X, 
        POS_STAGE_Y + y
      );
      ObjectUnhide(stages[stage_start].separator, OBJ_MODE_REGULAR);

      y += STAGE_SEPARATOR_HEIGHT;

      int i;
      for(i = 0; i < stages[stage_start].size; ++i) {
        ObjectSetPos(
          stages[stage_start].items[i].icon,
          POS_STAGE_X, 
          POS_STAGE_Y + y
        );
        ObjectUnhide(stages[stage_start].items[i].icon, OBJ_MODE_REGULAR);

        if(
          stages[stage_start].items->consumable_amount > -1 && 
          stage_start == stage_current
        ) {
          ObjectSetPos(
            stages[stage_start].items[i].gauge,
            POS_STAGE_X + POS_GAUGE_MARGIN_X, 
            POS_STAGE_Y + y + POS_GAUGE_MARGIN_Y
          );
          ObjectUnhide(stages[stage_start].items[i].gauge, OBJ_MODE_REGULAR);
        }
        y += STAGE_ICON_HEIGHT;
      }
      ++stage_start;
    }
    stage_previous = stage_current;
  }

  // if(stage_visible_consumable) {
  // here update carret for visible consumable items
  // loop trougth disaple stage to find consumable
  // }
}

void StageNext() {
  if(stage_current > 0) {
    stage_current -= 1;
  }
}
