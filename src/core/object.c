#include "object.h"

void OamInit(Object *object, u32 count)
{
    u32 nn = count;
    u32 *dst= (u32*)object;

    while(nn--)
    {
        *dst++= OBJ_MODE_HIDE;
        *dst++= 0;
    }

    oamWriteObjects(0, object, count);
}

