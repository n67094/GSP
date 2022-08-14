#include <seven/hw/video.h>

#include "../entity/earth.h"
#include "../core/memory.h"
#include "../renderer/sphere.h"
#include "../types.h"

#include "../../data/bitmaps/earth.palette.h"
#include "../../data/bitmaps/earth.tiles.h"

#include "earth.h"

static void EarthInitTileMap() {
  vu16 *tilemap_ptr = (vu16 *)MEM_VRAM;

  for (u32 i = 0; i < 128; ++i) {
    tilemap_ptr[i] = (((i << 1) + 1) << 8) + (i << 1);
  }

  for (u32 i = 0; i < 2; ++i) {
    tilemap_ptr[i] = 0xFFFF;
  }
}

void EarthInit() {
  MemCpy16(BG_PALETTE, earth_palette, earth_palette_size);

  EarthInitTileMap();
}

void EarthDraw(SphereData *earth) {
  SphereRender(earth_tiles, (u8 *)MEM_VRAM, earth->pitch, earth->spin, 7, 9);
}
