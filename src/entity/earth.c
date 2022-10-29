#include <seven/hw/video.h>

#include "../../data/bitmaps/earth.tiles.h"

#include "../core/memory.h"
#include "../entity/earth.h"
#include "../renderer/sphere.h"
#include "../types.h"

#include "earth.h"
#include "spaceship.h" //for the clearbuffer function

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
  MemCpy16(BG_PALETTE, earth_palette, 512);
  ClearBuffer(GFX_BASE_ADDR(0));
  ClearBuffer(GFX_BASE_ADDR(1));
  EarthInitTileMap();
}

void EarthDraw(SphereData *earth) {
  SphereRender(earth_tiles, (u8 *)MEM_VRAM, earth->pitch, earth->spin, 7, 9);
}
