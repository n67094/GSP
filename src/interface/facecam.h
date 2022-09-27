#ifndef INTERFACE_FACECAM_H
#define INTERFACE_FACECAM_H

#include "types.h"

#define FACECAM_NAME_1 ""
#define FACECAM_NAME_2 ""
#define FACECAM_NAME_2_NAME_3 ""

#define FACECAM_1_ID 1
#define FACECAM_2_ID 2
#define FACECAM_3_ID 3
#define FACECAM_SIZE 3

int FacecamInit(Object *oam_buffer, u32 oam_start);

void FacecamUpdate(void);

void FacecamSetNormal(int id);

void FacecamSetHappy(int id);

void FacecamSetSick(int id);

#endif
