#include <seven/base/attributes.h>
#include <seven/hw/memory.h>
#include <seven/hw/video.h>

#include "../global.h"
#include "../types.h"
#include "../core/utils.h"

#include "../../data/sprites/ui.palette.h"

#include "interface.h"

void InterfaceInit(void) {
  // TODO this will be fixed on libseven to repalce with OBJ_PALETTE
  u16 *palette = (u16*)MEM_PALETTE + 256;
  MemCpy8(palette, ui_palette, ui_palette_size);
}

void InterfaceUpdate(s32 roll, s32 pitch, s32 yaw) {}

void InterfaceDraw(void) {}

void InterfaceVBlank(void) {}
