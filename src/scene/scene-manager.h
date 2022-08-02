#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <seven/base/attributes.h>

typedef enum {
	TitleSceneState,
	DevlopperSceneState,
} SceneStates;

typedef struct SceneManager {
	SceneStates state;
} SceneManager;

extern SceneManager sceneManager;

void SceneManagerInit(void);

void SceneManagerUpdate(void);

void SceneManagerDraw(void);

void SceneManagerDestroy(void);

void SceneManagerLoad(SceneStates state);

void SceneManagerGoTo(SceneStates state);

#endif
