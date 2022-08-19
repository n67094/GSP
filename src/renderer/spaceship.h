#ifndef RENDERER_SPACESHIP_H
#define RENDERER_SPACESHIP_H

#include <seven/base/types.h>

void DrawCylinderWall(u8 *GfxPtr, u8 *PosTable, u32 Scale, u32 NumRows);
u8 *SetupPosTable(u8 *PosTable, u32 Pitch, u32 Radius, u32 CenterX, u32 CenterY); 

void ClearBuffer(u8 *BufferPtr);
void TransferBuffer(u8 *BufferPtr, u16 *VramPtr);

#endif
