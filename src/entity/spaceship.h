#ifndef ENTITIY_SPACESHIP_H
#define ENTITIY_SPACESHIP_H

#include "../types.h"
#include <seven/svc/affine.h>

void DrawCylinderWall(cu8 *GfxPtr, u32 GfxWidth, u32 GfxHeight, vu8 *BufferPtr, u32 horiz_len, u32 Radius);
void DrawConeWall(cu8 *GfxCenterPtr, vu8 *BufferCenterPtr, u32 GfxWidth, u32 GfxHeight);
void DrawConeWallBack(cu8 *GfcCenterPtr, vu8 *BufferCenterPtr, u32 GfxWidth, u32 GfxHeight);
void SetupPosTableCylinder(s32 pitch, u32 radius, u32 center_X); 
void SetupPosTableCone(s32 pitch, u32 radius_1, u32 radius_2, u32 center_X, s32 Height);
void BresenhamLine(cu8 *GfxPtr, vu8 *BufferPtr, u32 x_0, u32 x_1, u32 y_0, u32 y_1, u32 scale);
void DrawBase(cu8 *GfxPtr, u32 GfxWidth, vu8 *BufferPtr, s32 pitch, u32 radius, u32 center_X);


void ClearBuffer(vu8 *BufferPtr);
void TransferBuffer(vu8 *BufferPtr, vu16 *VramPtr);

void SpaceshipInit();
void SpaceshipDraw(ShipData *, struct BgAffineDstData *, CameraData *, s32);

void SortColumns(ColumnData *[], ShipData *, s32);
void CreateMatrix(RotationMatrix *, s32, s32, s32);
void MultiplyCameraMatrix(RotationMatrix *, s32, s32, RotationMatrix *);
void PrepareAffine(s32, struct BgAffineDstData *);
s32 GetRotate(RotationMatrix *);
s32 GetPitch(RotationMatrix *);
s32 GetSpin(RotationMatrix *, s32);

extern u16 InverseTable[128];

#endif
