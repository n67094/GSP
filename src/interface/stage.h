#ifndef INTERFACE_STAGE_H
#define INTERFACE_STAGE_H

#include "types.h"

#include "interface-data.h"

#define STAGES_SIZE 4

#define STAGE_MAX_HEIGHT 100
#define STAGE_SEPARATOR_HEIGHT 8
#define STAGE_ICON_HEIGHT 16
#define STAGE_MAX_CONSUMABLE 9

extern int stage_previous;
extern int stage_current;

extern int stage_star;
extern int stage_end;

extern Stage *stages;

/*
 * return the number of object taken on the oam_buffer
 */
int StageInit(Object *oam_buffer, u32 oam_start);

void StageUpdate();

void StageNext();

#endif
