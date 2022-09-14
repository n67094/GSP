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

u32 stage_previous = STAGES_SIZE;
u32 stage_current = STAGES_SIZE;

u32 stage_visible_start = 0;
u32 stage_visible_end = 0;
bool stage_visible_consumable = false;

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
  while(i > 0) {

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
      OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_FONT_DIGITS + STAGES_SIZE + 1 - i)
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

  return 0;
}

void StageUpdate() {
  if(stage_current != stage_previous) {
  // Here update stages visible on screen - position on show/hide
  // from latest in the array to first

  // compute size state

  // draw state
  }

  if(stage_visible_consumable) {
  // here update carret for visible consumable items
  // loop trougth disaple stage to find consumable
  }
}

void StageNext() {
  if(stage_current > 0) {
    stage_current -= 1;
  }
}
