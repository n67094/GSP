#include <seven/video/object.h>

#include "../../data/sprites/stage-icons.tiles.h"
#include "../../data/sprites/stage-separator.tiles.h"
#include "../../data/sprites/stage-gauge.tiles.h"

#include "../global.h"
#include "../types.h"
#include "../core/memory.h"
#include "../core/object.h"
#include "../core/tile.h"

#include "interface-data.h"
#include "stage.h"

u32 stage_previous = STAGES_SIZE;
u32 stage_current = STAGES_SIZE;

u32 stage_visible_start = 0;
u32 stage_visible_end = 0;
bool stage_visible_consumable = false;

void StageInit() {
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
