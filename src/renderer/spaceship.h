#ifndef RENDERER_SPACESHIP_H
#define RENDERER_SPACESHIP_H

#include <seven/base/types.h>

void DrawCylinderWall(u8 *GfxPtr, u8 *PosTable, u32 Scale, u32 NumRows);
u8 *SetupPosTable(u8 *PosTable, u32 Pitch, u32 Radius, u32 CenterX, u32 CenterY); 

u8 PosTable1[160];
u8 PosTable2[160];
u8 SpaceshipBuffer[0x4000];
#endif
