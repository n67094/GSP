#ifndef CORE_TILE_H
#define CORE_TILE_H

#include <seven/base/addresses.h>

#include "../types.h"

#define VRAM_BG_SIZE 0x10000
#define VRAM_OBJ_SIZE 0x08000

#define MEM_VRAM_OBJ (MEM_VRAM + VRAM_BG_SIZE)

typedef struct { u32 data[8]; } Tile4;
typedef struct { u32 data[16]; } Tile8;

typedef Tile4 Charblock4[512];
typedef Tile8 Charblock8[256];

#define TILE4_CHARBLOCKS MEMARRAY(MEM_VRAM, Charblock4, 4)
#define TILE8_CHARBLOCKS MEMARRAY(MEM_VRAM, Charblock8, 4)

#define OBJ4_CHARBLOCKS MEMARRAY(MEM_VRAM_OBJ, Charblock4, 2)
#define OBJ8_CHARBLOCKS MEMARRAY(MEM_VRAM_OBJ, Charblock8, 2)

#define TILE4_MEM ((Charblock4*)MEM_VRAM)

#endif
