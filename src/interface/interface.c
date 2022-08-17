#include <seven/base/attributes.h>
#include <seven/hw/memory.h>
#include <seven/hw/video.h>
#include <seven/video/oam.h>

#include "../../data/sprites/caret.tiles.h"
#include "../../data/sprites/interface.palette.h"

#include "../global.h"
#include "../types.h"
#include "../core/memory.h"
#include "../core/object.h"
#include "../core/tile.h"
#include "axes.h"
#include "metrics.h"
#include "interface-data.h"

#include "interface.h"

void InterfaceInit(void) {

  OamInit(object_buffer, 128);

  MemCpy32(OBJ_PALETTE, interface_palette, interface_palette_size);
  MemCpy32(&TILE4_CHARBLOCKS[4][TILE_CARET], caret_tiles, caret_tiles_size);

  AxesInit();
  MetricsInit();

}

void InterfaceUpdate(s32 roll, s32 pitch, s32 yaw, s32 altitude, s32 speed, s32 throttle) {
  AxesUpdate(roll, pitch, yaw);
  MetricsUpdate(altitude, speed, throttle);
}

void InterfaceDraw(void) {
  oamWriteObjects(0, object_buffer, 128);
}

void InterfaceVBlank(void) {}
