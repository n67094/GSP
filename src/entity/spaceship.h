#ifndef RENDERER_SPACESHIP_H
#define RENDERER_SPACESHIP_H

#include <seven/base/types.h>

void DrawCylinderWall(cu8 *GfxPtr, u32 GfxWidth, u32 GfxHeight, vu8 *BufferPtr, u32 horiz_len, u32 Radius);
void DrawConeWall(cu8 *GfxCenterPtr, vu8 *BufferCenterPtr, u32 GfxWidth, u32 GfxHeight);
void SetupPosTableCylinder(s32 pitch, u32 radius, u32 center_X); 
void SetupPosTableCone(s32 pitch, u32 radius_1, u32 radius_2, u32 center_X, s32 Height);
void BresenhamLine(cu8 *GfxPtr, vu8 *BufferPtr, u32 x_0, u32 x_1, u32 y_0, u32 y_1, u32 scale);


void ClearBuffer(vu8 *BufferPtr);
void TransferBuffer(vu8 *BufferPtr, vu16 *VramPtr);

void SpaceshipInit();
void SpaceshipDraw(s32 pitch, s32 spin);

extern cu8 TestWallTextureBitmap[];

#endif
