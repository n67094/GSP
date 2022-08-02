#include <acsl/stdlib.h>

#include "title.h"

TitleScene* TitleCreate(void) {
  TitleScene *scene = malloc(sizeof(TitleScene));

  scene->test1 = 0xDEAD;
  scene->test2 = 0xDEAD;

  return scene;
}

void TitleUpdate(TitleScene* scene) {

}

void TitleDraw(TitleScene* scene) {

}

void TitleDestroy(TitleScene* scene) {

}

