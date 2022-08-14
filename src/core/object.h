#ifndef CORE_OBJECT_H
#define CORE_OBJECT_H

#include <seven/base/bits.h>
#include <seven/video/bg_regular.h>
#include <seven/video/object.h>

#include "../types.h"

inline Object *ObjectSetAttr(Object *object, u16 attr0, u16 attr1, u16 attr2)
{
  object->attr0 = attr0;
  object->attr1 = attr1;
  object->attr2 = attr2;

  return object;
}

inline void ObjectSetPos(Object *object, u32 x, u32 y)
{
  object->attr0 = BF_SET(object->attr0, ATTR0_Y, y);
  object->attr1 = BF_SET(object->attr1, ATTR1_X, x);
}

#endif
