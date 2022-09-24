#ifndef INTERFACE_FACECAM_H
#define INTERFACE_FACECAM_H

#define GERBIL_NAME_1 ""
#define GERBIL_NAME_2 ""
#define GERBIL_NAME_3 ""

typedef enum FacecamState {
 FACECAM_STATE_NORMAL,
 FACECAM_STATE_HAPPY,
 FACECAM_STATE_SICK,
} FacecamState;

extern FacecamState facecam_state;

void FacecamInit(void);

void FacecamUpdate(void);

void FacecamSetNormal(void);

void FacecamSetHappy(void);

void FacecamSetSick(void);

#endif
