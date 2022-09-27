#include "../../data/sprites/facecam-border.tiles.h"
#include "../../data/sprites/facecam-normal-bottom.tiles.h"
#include "../../data/sprites/facecam-normal-top.tiles.h"
#include "../../data/sprites/facecam-happy-bottom.tiles.h"
#include "../../data/sprites/facecam-sick-bottom.tiles.h"
#include "../../data/sprites/facecam-sick-top.tiles.h"
#include "../../data/sprites/facecam-name-1.tiles.h"
#include "../../data/sprites/facecam-name-2.tiles.h"
#include "../../data/sprites/facecam-name-3.tiles.h"

#include "../core/types.h"
#include "../core/object.h"
#include "../core/tile.h"
#include "../core/memory.h"

#include "seven/video/object.h"
#include "types.h"
#include "facecam.h"
#include "interface-data.h"

bool facecam_changed = false;

Facecam facecams[3] = {
  { .top_state = FACECAM_STATE_NORMAL , .bottom_state = FACECAM_STATE_NORMAL},
  { .top_state = FACECAM_STATE_NORMAL , .bottom_state = FACECAM_STATE_NORMAL},
  { .top_state = FACECAM_STATE_NORMAL , .bottom_state = FACECAM_STATE_NORMAL},
};

int FacecamInit(Object *oam_buffer, u32 oam_start) {

  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_FACECAM_BORDER], facecam_border_tiles, facecam_border_tiles_size);

  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_FACECAM_NORMAL_TOP], facecam_normal_top_tiles, facecam_normal_top_tiles_size);
  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_FACECAM_NORMAL_BOTTOM], facecam_normal_bottom_tiles, facecam_normal_bottom_tiles_size);

  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_FACECAM_HAPPY_BOTTOM], facecam_happy_bottom_tiles, facecam_happy_bottom_tiles_size);

  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_FACECAM_SICK_TOP], facecam_sick_top_tiles, facecam_sick_top_tiles_size);
  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_FACECAM_SICK_BOTTOM], facecam_sick_bottom_tiles, facecam_sick_bottom_tiles_size);

  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_FACECAM_NAME_1], facecam_name_1_tiles, facecam_name_1_tiles_size);
  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_FACECAM_NAME_2], facecam_name_2_tiles, facecam_name_2_tiles_size);
  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_FACECAM_NAME_3], facecam_name_3_tiles, facecam_name_3_tiles_size);

  // BORDER FACECAM 1

  int object_count = 0;

  Object *facecam_1 = &oam_buffer[oam_start + object_count];
  ObjectSetAttr(
      facecam_1,
      OBJ_SHAPE_SQUARE,
      OBJ_SIZE_32X32,
      OBJ_PALETTE_NUMBER(FACECAM_PALETTE_0) | OBJ_TILE_NUMBER(TILE_FACECAM_BORDER)
    );
  ObjectSetPos(facecam_1, POS_FACECAM_1_X, POS_FACECAM_1_Y);
  ObjectUnhide(facecam_1, OBJ_MODE_REGULAR);
  ++object_count;

  // FACE FACECAM 1

  facecams[0].top = &oam_buffer[oam_start + object_count];
  ObjectSetAttr(
      facecams[0].top,
      OBJ_SHAPE_WIDE,
      OBJ_SIZE_16X8,
      OBJ_PALETTE_NUMBER(FACECAM_PALETTE_0) | OBJ_TILE_NUMBER(TILE_FACECAM_NORMAL_TOP)
    );
  ObjectSetPos(facecams[0].top, POS_FACECAM_1_X + POS_FACECAM_TOP_OFFSET_X, POS_FACECAM_1_Y + POS_FACECAM_TOP_OFFSET_Y);
  ObjectUnhide(facecams[0].top, OBJ_MODE_REGULAR);
  ++object_count;

  facecams[0].bottom = &oam_buffer[oam_start + object_count];
  ObjectSetAttr(
      facecams[0].bottom,
      OBJ_SHAPE_WIDE,
      OBJ_SIZE_16X8,
      OBJ_PALETTE_NUMBER(FACECAM_PALETTE_0) | OBJ_TILE_NUMBER(TILE_FACECAM_NORMAL_BOTTOM)
    );
  ObjectSetPos(facecams[0].bottom, POS_FACECAM_1_X + POS_FACECAM_BOTTOM_OFFSET_X, POS_FACECAM_1_Y + POS_FACECAM_BOTTOM_OFFSET_Y);
  ObjectUnhide(facecams[0].bottom, OBJ_MODE_REGULAR);
  ++object_count;

  // NAME FACECAME 1

  Object *name_1 = &oam_buffer[oam_start + object_count];
  ObjectSetAttr(
      name_1,
      OBJ_SHAPE_WIDE,
      OBJ_SIZE_32X8,
      OBJ_PALETTE_NUMBER(FACECAM_PALETTE_0) | OBJ_TILE_NUMBER(TILE_FACECAM_NAME_1)
    );
  ObjectSetPos(name_1, POS_FACECAM_NAME_1_X, POS_FACECAM_NAME_1_Y);
  ObjectUnhide(name_1, OBJ_MODE_REGULAR);
  ++object_count;

  // BORDER FACECAM 2

  Object *facecam_2 = &oam_buffer[oam_start + object_count];
  ObjectSetAttr(
      facecam_2,
      OBJ_SHAPE_SQUARE,
      OBJ_SIZE_32X32,
      OBJ_PALETTE_NUMBER(FACECAM_PALETTE_1) | OBJ_TILE_NUMBER(TILE_FACECAM_BORDER)
    );
  ObjectSetPos(facecam_2, POS_FACECAM_2_X, POS_FACECAM_2_Y);
  ObjectUnhide(facecam_2, OBJ_MODE_REGULAR);
  ++object_count;

  // FACE FACECAM 2

  facecams[1].top = &oam_buffer[oam_start + object_count];
  ObjectSetAttr(
      facecams[1].top,
      OBJ_SHAPE_WIDE,
      OBJ_SIZE_16X8,
      OBJ_PALETTE_NUMBER(FACECAM_PALETTE_1) | OBJ_TILE_NUMBER(TILE_FACECAM_NORMAL_TOP)
    );
  ObjectSetPos(facecams[1].top, POS_FACECAM_2_X + POS_FACECAM_TOP_OFFSET_X, POS_FACECAM_2_Y + POS_FACECAM_TOP_OFFSET_Y);
  ObjectUnhide(facecams[1].top, OBJ_MODE_REGULAR);
  ++object_count;

  facecams[1].bottom = &oam_buffer[oam_start + object_count];
  ObjectSetAttr(
      facecams[1].bottom,
      OBJ_SHAPE_WIDE,
      OBJ_SIZE_16X8,
      OBJ_PALETTE_NUMBER(FACECAM_PALETTE_1) | OBJ_TILE_NUMBER(TILE_FACECAM_NORMAL_BOTTOM)
    );
  ObjectSetPos(facecams[1].bottom, POS_FACECAM_2_X + POS_FACECAM_BOTTOM_OFFSET_X, POS_FACECAM_2_Y + POS_FACECAM_BOTTOM_OFFSET_Y);
  ObjectUnhide(facecams[1].bottom, OBJ_MODE_REGULAR);
  ++object_count;

  // NAME FACECAM 2

  Object *name_2 = &oam_buffer[oam_start + object_count];
  ObjectSetAttr(
      name_2,
      OBJ_SHAPE_WIDE,
      OBJ_SIZE_32X8,
      OBJ_PALETTE_NUMBER(FACECAM_PALETTE_1) | OBJ_TILE_NUMBER(TILE_FACECAM_NAME_2)
    );
  ObjectSetPos(name_2, POS_FACECAM_NAME_2_X, POS_FACECAM_NAME_2_Y);
  ObjectUnhide(name_2, OBJ_MODE_REGULAR);
  ++object_count;

  // BORDER FACACAM 3

  Object *facecam_3 = &oam_buffer[oam_start + object_count];
  ObjectSetAttr(
      facecam_3,
      OBJ_SHAPE_SQUARE,
      OBJ_SIZE_32X32,
      OBJ_PALETTE_NUMBER(FACECAM_PALETTE_0) | OBJ_TILE_NUMBER(TILE_FACECAM_BORDER)
    );
  ObjectSetPos(facecam_3, POS_FACECAM_3_X, POS_FACECAM_3_Y);
  ObjectUnhide(facecam_3, OBJ_MODE_REGULAR);
  ++object_count;

  // FACE FACECAM 3

  facecams[2].top = &oam_buffer[oam_start + object_count];
  ObjectSetAttr(
      facecams[2].top,
      OBJ_SHAPE_WIDE,
      OBJ_SIZE_16X8,
      OBJ_PALETTE_NUMBER(FACECAM_PALETTE_0) | OBJ_TILE_NUMBER(TILE_FACECAM_NORMAL_TOP)
    );
  ObjectSetPos(facecams[2].top, POS_FACECAM_3_X + POS_FACECAM_TOP_OFFSET_X, POS_FACECAM_3_Y + POS_FACECAM_TOP_OFFSET_Y);
  ObjectUnhide(facecams[2].top, OBJ_MODE_REGULAR);
  ++object_count;

  facecams[2].bottom = &oam_buffer[oam_start + object_count];
  ObjectSetAttr(
      facecams[2].bottom,
      OBJ_SHAPE_WIDE,
      OBJ_SIZE_16X8,
      OBJ_PALETTE_NUMBER(FACECAM_PALETTE_0) | OBJ_TILE_NUMBER(TILE_FACECAM_NORMAL_BOTTOM)
    );
  ObjectSetPos(facecams[2].bottom, POS_FACECAM_3_X + POS_FACECAM_BOTTOM_OFFSET_X, POS_FACECAM_3_Y + POS_FACECAM_BOTTOM_OFFSET_Y);
  ObjectUnhide(facecams[2].bottom, OBJ_MODE_REGULAR);
  ++object_count;

  // NAME FACECAME 3

  Object *name_3 = &oam_buffer[oam_start + object_count];
  ObjectSetAttr(
      name_3,
      OBJ_SHAPE_WIDE,
      OBJ_SIZE_32X8,
      OBJ_PALETTE_NUMBER(FACECAM_PALETTE_0) | OBJ_TILE_NUMBER(TILE_FACECAM_NAME_3)
    );
  ObjectSetPos(name_3, POS_FACECAM_NAME_3_X, POS_FACECAM_NAME_3_Y);
  ObjectUnhide(name_3, OBJ_MODE_REGULAR);
  ++object_count;

  return object_count;
}

void FacecamUpdate(void) {
  if(facecam_changed) {
    facecam_changed = false;
  }
}

void FacecamSetNormal(int id) {
  if(id < 0 || id > FACECAM_SIZE) return;

  facecam_changed = true;
  facecams[id].top_state = FACECAM_STATE_NORMAL;
  facecams[id].bottom_state = FACECAM_STATE_NORMAL;
}

void FacecamSetHappy(int id) {
  if(id < 0 || id > FACECAM_SIZE) return;

  facecam_changed = true;
  facecams[id].top_state = FACECAM_STATE_NORMAL;
  facecams[id].bottom_state = FACECAM_STATE_HAPPY;
}

void FacecamSetSick(int id) {
  if(id < 0 || id > FACECAM_SIZE) return;

  facecam_changed = true;
  facecams[id].top_state = FACECAM_STATE_SICK;
  facecams[id].bottom_state = FACECAM_STATE_SICK;
}
