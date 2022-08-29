#ifndef RENDERER_SPACESHIP_H
#define RENDERER_SPACESHIP_H

#include <seven/base/types.h>

void DrawCylinderWall(cu8 *GfxPtr, u32 GfxWidth, u32 Scale, u32 Radius, vu8 *BufferPtr, u32 GfxHeight);
void SetupPosTableCylinder(s32 pitch, u32 radius, u32 center_X); 
void SetupPosTableCone(u32 pitch, u32 radius_1, u32 center_X, u32 center_Y, u32 radius_2, u32 Height);

void ClearBuffer(vu8 *BufferPtr);
void TransferBuffer(vu8 *BufferPtr, vu16 *VramPtr);

void SpaceshipInit();
void SpaceshipDraw(s32 pitch, s32 spin);

extern cu8 TestWallTextureBitmap[];

#endif
