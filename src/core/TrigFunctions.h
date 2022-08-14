#include "seven/base/types.h"

//All inputs and outputs are in 24.8 signed format (24 whole number bits, 8 fractional bits)

extern s32 GetSinValue(s32) IWRAM_CODE;
extern s32 GetCosValue(s32) IWRAM_CODE;
extern s32 GetArcSinValue(s32) IWRAM_CODE;
extern s32 GetArcCosValue(s32) IWRAM_CODE;
extern s32 GetPythSqrtValue(s32) IWRAM_CODE;
extern s32 GetInvPythSqrtValue(s32) IWRAM_CODE;
extern s32 GetSecValue(s32) IWRAM_CODE;
extern s32 GetCscValue(s32) IWRAM_CODE;
extern s32 GetTanValue(s32) IWRAM_CODE;
extern s32 GetCotValue(s32) IWRAM_CODE;

extern u32 table_group_1[256];
extern u32 table_group_2[256];

