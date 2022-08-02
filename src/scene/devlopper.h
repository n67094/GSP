#ifndef SCENE_DEVLOPPER_H
#define SCENE_DEVLOPPER_H

typedef struct DevlopperScene {
	// ADD attributes here
} DevlopperScene;

DevlopperScene* DevlopperCreate(void);

void DevlopperUpdate(DevlopperScene* scene);

void DevlopperDraw(DevlopperScene* scene);

void DevlopperDestroy(DevlopperScene* scene);

#endif
