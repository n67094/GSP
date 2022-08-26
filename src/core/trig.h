#ifndef CORE_TRIG_H
#define CORE_TRIG_H

#include "types.h"

//All inputs and outputs are in 24.8 signed format (24 whole number bits, 8 fractional bits)

extern s32 TrigGetSin(s32) IWRAM_CODE;
extern s32 TrigGetCos(s32) IWRAM_CODE;
extern s32 TrigGetArcSin(s32) IWRAM_CODE;
extern s32 TrigArcCos(s32) IWRAM_CODE;
extern s32 TrigGetPythSqrt(s32) IWRAM_CODE;
extern s32 TrigGetInvPythSqrt(s32) IWRAM_CODE;
extern s32 TrigGetSec(s32) IWRAM_CODE;
extern s32 TrigGetCsc(s32) IWRAM_CODE;
extern s32 TrigGetTan(s32) IWRAM_CODE;
extern s32 TrigGetCot(s32) IWRAM_CODE;

extern u32 trig_table_group_1[256];
extern u32 trig_table_group_2[256];

#endif
