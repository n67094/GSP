#ifndef INTERFACE_MANAGER_H
#define INTERFACE_MANAGER_H

#include "../types.h"

void InterfaceInit(void);

void InterfaceUpdate(s32 roll, s32 pitch, s32 yaw);

void InterfaceDraw(void);

void InterfaceVBlank(void);

#endif
