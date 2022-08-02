#ifndef SCENE_TITLE_H
#define SCENE_TITLE_H

#include <seven/base/types.h>

typedef struct TitleScene {
	// ADD attributes here
	u16 test1;
	u16 test2;
} TitleScene;

TitleScene* TitleCreate(void);

void TitleUpdate(TitleScene* scene);

void TitleDraw(TitleScene* scene);

void TitleDestroy(TitleScene* scene);

#endif
