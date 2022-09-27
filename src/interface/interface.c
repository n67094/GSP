#include <seven/base/attributes.h>
#include <seven/hw/memory.h>
#include <seven/hw/video.h>
#include <seven/video/oam.h>

#include "../../data/sprites/caret.tiles.h"
#include "../../data/sprites/interface-0.palette.h"
#include "../../data/sprites/interface-1.palette.h"
#include "../../data/sprites/label-1.palette.h"
#include "../../data/sprites/label-2.palette.h"
#include "../../data/sprites/facecam-0.palette.h"
#include "../../data/sprites/facecam-1.palette.h"

#include "../global.h"
#include "../types.h"

#include "../core/memory.h"
#include "../core/object.h"
#include "../core/tile.h"

#include "axes.h"
#include "metrics.h"
#include "facecam.h"
#include "stage.h"
#include "interface-data.h"

#include "interface.h"

void InterfaceInit(void)
{
  OamInit(object_buffer, 128);

  MemCpy32(OBJ_PALETTE_BANK[0], interface_0_palette, interface_0_palette_size);
  MemCpy32(OBJ_PALETTE_BANK[1], interface_1_palette, interface_1_palette_size);

  MemCpy32(OBJ_PALETTE_BANK[2], label_1_palette, label_1_palette_size);
  MemCpy32(OBJ_PALETTE_BANK[3], label_2_palette, label_2_palette_size);

  MemCpy32(OBJ_PALETTE_BANK[4], facecam_0_palette, facecam_0_palette_size);
  MemCpy32(OBJ_PALETTE_BANK[5], facecam_1_palette, facecam_1_palette_size);

  MemCpy32(&OBJ4_CHARBLOCKS[0][TILE_CARET], caret_tiles, caret_tiles_size);

  AxesInit();
  MetricsInit();
  FacecamInit(object_buffer, OAM_FACECAM);
  StageInit(object_buffer, OAM_STAGE);
}

void InterfaceUpdate(s32 roll, s32 pitch, s32 yaw, s32 altitude, s32 speed, s32 throttle)
{
  AxesUpdate(roll, pitch, yaw);
  MetricsUpdate(altitude, speed, throttle);
  FacecamUpdate();
  StageUpdate();
}

void InterfaceDraw(void)
{
  oamWriteObjects(0, object_buffer, 128);
}

void InterfaceVBlank(void) {}
