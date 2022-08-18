#ifndef RENDERER_SPACESHIP_H
#define RENDERER_SPACESHIP_H

#include <seven/base/types.h>

void DrawCylinderWall(u8 *GfxPtr, u8 *PosTable, u32 Scale, u32 NumRows);
u8 *SetupPosTable(u8 *PosTable, u32 Pitch, u32 Radius, u32 CenterX, u32 CenterY); 


#endif
