#include "../global.h"
#include "../types.h"

#include "axes.h"

void AxesInit() {
  Object *label = &object_buffer[OAM_AXES_LABEL];
  Object *roll_cursor = &object_buffer[OAM_AXES_ROLL_CURSOR];
  Object *pitch_cursor = &object_buffer[OAM_AXES_PITCH_CURSOR];
  Object *yaw_curosr = &object_buffer[OAM_AXES_YAW_CURSOR];

  // TODO init object attr
}

void AxesUpdate(s32 roll, s32 pitch, s32 yaw) {

}
