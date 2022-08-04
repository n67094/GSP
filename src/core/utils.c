#include <seven/base/types.h>
#include <seven/base/addresses.h>
#include <seven/hw/memory.h>

#include "utils.h"

void TransferPalette(const u16 *data)
{
  volatile u16 *palette_pointer = (volatile u16 *)MEM_PALETTE;

  for(u32 i = 0; i < 256; ++i) {
    palette_pointer[i] = data[i];
  }
}

// What this does ?
void TransferTilemap()
{
  volatile u16 * tilemap_ptr = (volatile u16 *)MEM_VRAM;

  for(u32 i = 0; i < 128; ++i) {
    tilemap_ptr[i] = (((i << 1) + 1) << 8) + (i << 1);
  }

  for(u32 i = 0; i < 2; ++i) {
    tilemap_ptr[i] = 0xFFFF;
  }
}
