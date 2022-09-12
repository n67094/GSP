#include <seven/video/object.h>

#include "../../data/sprites/stage-icons.tiles.h"
#include "../../data/sprites/stage-separator.tiles.h"
#include "../../data/sprites/stage-gauge.tiles.h"

#include "../global.h"
#include "../types.h"
#include "../core/memory.h"
#include "../core/object.h"
#include "../core/tile.h"
#include "../core/label.h"

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
    &TILE4_CHARBLOCKS[4][TILE_ICONS],
    stage_icons_tiles, stage_icons_tiles_size
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
  int i = STAGES_SIZE;
  int object_count = 0;
  while(i > 0) {
    StageItem *items;

    int j = 0;
    while( j < stages[i].size) {
      // the icons
      items[j].icons = &oam_buffer[oam_start + object_count];
      ObjectSetAttr(
        items[j].icons,
        OBJ_SHAPE_SQUARE,
        OBJ_SIZE_16X16,
        OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(items[j].tile_id)
      );
      ObjectSetPos(items[j].icons, 0, 0);
      ++object_count;

      if(stages[i].items[j].consumable_amount > -1) {
        // the gauge
        items[j].icons = &oam_buffer[oam_start + object_count];
        ObjectSetAttr(
          items[j].icons,
          OBJ_SHAPE_SQUARE,
          OBJ_SIZE_16X16,
          OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_GAUGE)
        );
        ObjectSetPos(items[j].icons, 0, 0);

        ++object_count;

        // the caret
        items[j].icons = &oam_buffer[oam_start + object_count];
        ObjectSetAttr(
          items[j].icons,
          OBJ_SHAPE_SQUARE,
          OBJ_SIZE_16X16,
          OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_CARET)
        );
        ObjectSetPos(items[j].icons, 0, 0);
        ++object_count;
      }
    }

    // the number
    items[j].icons = &oam_buffer[oam_start + object_count];
    ObjectSetAttr(
      items[j].icons,
      OBJ_SHAPE_SQUARE,
      OBJ_SIZE_16X16,
      OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER()// TODO)
    );
    ObjectSetPos(items[j].icons, 0, 0);
    ++object_count;


    // the separator
    items[j].icons = &oam_buffer[oam_start + object_count];
    ObjectSetAttr(
      items[j].icons,
      OBJ_SHAPE_SQUARE,
      OBJ_SIZE_16X16,
      OBJ_PALETTE_NUMBER(OBJ_PALETTE_0) | OBJ_TILE_NUMBER(TILE_SEPARATOR)
    );
    ObjectSetPos(items[j].icons, 0, 0);
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
