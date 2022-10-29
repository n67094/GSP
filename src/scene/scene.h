#ifndef SCENE_H
#define SCENE_H

typedef struct scene {
	void (*Open)(void);
	void (*Update)(void);
	void (*Draw)(void);
	void (*VBlank)(void);
	void (*Close)(void);
} Scene;

extern Scene title_scene, help_scene, mission_scene;

void SceneOpen(void);

void SceneUpdate(void);

void SceneDraw(void);

void SceneVBlank(void);

void SceneGoTo(Scene scene);

#endif
