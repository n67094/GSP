#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include <seven/base/types.h>
#include <seven/video/object.h>

typedef volatile u8  vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;

typedef volatile s8  vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef const u8  cu8;
typedef const u16 cu16;
typedef const u32 cu32;
typedef const u64 cu64;

typedef const s8  cs8;
typedef const s16 cs16;
typedef const s32 cs32;
typedef const s64 cs64;

typedef struct Object Object;

typedef struct LabelGlyph {
  u16 width;
  u16 height;
  u16 tile_id;
  char ascii_code;
} LabelGlyph;

// start_alpha_idx start a A, finish at Z
// start_digit_idx start afther z at 0 and finish at 9
// start_special_idx start after 9 at " " next are ":", "."
typedef struct LabelFont {
  LabelGlyph* glyphs;
  u16 start_alpha_id;
  u16 start_digit_id;
  u16 start_special_id;
  u16 size;
} LabelFont;

#endif
