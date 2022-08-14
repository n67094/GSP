#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

typedef struct scene {
	void (*Open)(void);
	void (*Update)(void);
	void (*Draw)(void);
	void (*VBlank)(void);
	void (*Close)(void);
} Scene;

extern Scene title_scene, mission_scene;

void SceneManagerOpen(void);

void SceneManagerUpdate(void);

void SceneManagerDraw(void);

void SceneManagerVBlank(void);

void SceneManagerGoTo(Scene scene);

#endif
