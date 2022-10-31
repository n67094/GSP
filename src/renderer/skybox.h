#include <seven/base/types.h>

typedef struct skyboxEntity{
	s32 pitch;
	s32 spin;
}skyboxEntity;

void SkyboxDraw(CameraData *);
void SkyboxInit();

void noPitch(s32);
void RotateEntities(s32, CameraData *);

