#ifndef CORE_OBJECT_H
#define CORE_OBJECT_H

#include <seven/base/bits.h>

#include "../types.h"

#define BF_ATTR0_Y_LENGTH 8
#define BF_ATTR0_Y_OFFSET 0

#define BF_ATTR1_X_LENGTH 9
#define BF_ATTR1_X_OFFSET 0

#define OBJECT_ATTR0_SET(y, shape, bpp, mode, mos, bld, win)                                                           \
  (((y)&255) | (((mode)&3) << 8) | (((bld)&1) << 10) | (((win)&1) << 11) | (((mos)&1) << 12) | (((bpp)&8) << 10) |     \
   (((shape)&3) << 14))

#define OBJECT_ATTR1_SET(x, size, hflip, vflip)                                                                        \
  (((x)&511) | (((hflip)&1) << 12) | (((vflip)&1) << 13) | (((size)&3) << 14))

#define OBJECT_ATTR2_SET(id, pbank, prio) (((id)&0x3FF) | (((pbank)&15) << 12) | (((prio)&3) << 10))

inline void obj_set_pos(Object *object, u32 x, u32 y)
{
  BF_SET(object->attr0, ATTR0_Y, y);
  BF_SET(object->attr1, ATTR1_X, x);
}

#endif
