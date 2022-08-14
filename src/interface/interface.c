#include <seven/base/attributes.h>
#include <seven/hw/memory.h>
#include <seven/hw/video.h>
#include <seven/video/oam.h>

#include "../../data/sprites/axes.tiles.h"
#include "../../data/sprites/interface.palette.h"

#include "../global.h"
#include "../types.h"
#include "../core/memory.h"
#include "axes.h"
#include "interface.h"

void InterfaceInit(void) {
  AxesInit();

  MemCpy8(OBJ_PALETTE, interface_palette, interface_palette_size);
}

void InterfaceUpdate(s32 roll, s32 pitch, s32 yaw) {
  AxesUpdate(roll, pitch, yaw);
}

void InterfaceDraw(void) {
  oamWriteObjects(0, object_buffer, 128);
}

void InterfaceVBlank(void) {}
