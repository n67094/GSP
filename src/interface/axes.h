#ifndef INTERFACE_AXES
#define INTERFACE_AXES

#include "../types.h"

void AxesCreate(s32 roll, s32 pitch, s32 yaw);

void AxesUpdate(s32 roll, s32 pitch, s32 yaw);

void AxesDraw(void);

#endif
