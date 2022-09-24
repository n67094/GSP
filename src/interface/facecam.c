#include "../core/types.h"
#include "../core/object.h"

#include "facecam.h"

bool facecam_changed = false;
FacecamState facecam_state = FACECAM_STATE_NORMAL;

void FacecamInit(void) {
  // load palette
  // load sprites
}

void FacecamUpdate(void) {
  if(facecam_changed) {
    facecam_changed = false;

    switch(facecam_state){
      case FACECAM_STATE_HAPPY:
        // hide other sprite
        // show that sprite
      break;
      case FACECAM_STATE_SICK:
        // hide other sprite
        // show that sprite
      break;
      default:
      break;
    }
  }
}

void FacecamSetNormal(void) {
  facecam_changed = true;
  facecam_state = FACECAM_STATE_NORMAL;
}

void FacecamSetHappy(void) {
  facecam_changed = true;
  facecam_state = FACECAM_STATE_HAPPY;
}

void FacecamSetSick(void) {
  facecam_changed = true;
  facecam_state = FACECAM_STATE_SICK;
}
