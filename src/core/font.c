#include "../../data/sprites/alpha.tiles.h"
#include "../../data/sprites/digits.tiles.h"

#include "memory.h"
#include "tile.h"
#include "font.h"

void FontInit() {
  MemCpy32(&OBJ4_CHARBLOCKS[0][TILE_FONT_ALPHA], alpha_tiles, alpha_tiles_size);
  MemCpy32(&OBJ4_CHARBLOCKS[0][TILE_FONT_DIGITS], digits_tiles, digits_tiles_size);
}
