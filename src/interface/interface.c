#include <seven/base/attributes.h>
#include <seven/hw/memory.h>
#include <seven/hw/video.h>

#include "../../data/sprites/axes.tiles.h"
#include "../../data/sprites/ui.palette.h"

#include "../core/memory.h"
#include "../global.h"
#include "../types.h"
#include "axes.h"
#include "interface.h"

void InterfaceInit(void) {
  MemCpy8(OBJ_PALETTE, ui_palette, ui_palette_size);
}

void InterfaceUpdate(s32 roll, s32 pitch, s32 yaw) {}

void InterfaceDraw(void) {}

void InterfaceVBlank(void) {}
