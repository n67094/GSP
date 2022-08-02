#include "scene-manager.h"

#include "title.h"
#include "devlopper.h"

void* scene;

void SceneManagerInit()
{
  // The first scene on screen
  SceneManagerLoad(TitleSceneState);
}

void SceneManagerLoad(SceneStates state)
{
  switch (state) {
  case TitleSceneState:
    scene = TitleCreate();
    break;
  case DevlopperSceneState:
    scene = DevlopperCreate();
    break;
  }
}

void SceneManagerUpdate()
{
  switch (sceneManager.state) {
  case TitleSceneState:
    TitleUpdate(scene);
    break;
  case DevlopperSceneState:
    DevlopperUpdate(scene);
    break;
  }
}

void SceneManagerDraw()
{
  switch (sceneManager.state) {
  case TitleSceneState:
    TitleDraw(scene);
    break;
  case DevlopperSceneState:
    DevlopperDraw(scene);
    break;
  }
}

void SceneManagerDestroy()
{
  switch (sceneManager.state) {
  case TitleSceneState:
    TitleDestroy(scene);
    break;
  case DevlopperSceneState:
    DevlopperDestroy(scene);
    break;
  }
}

void SceneManagerGoTo(SceneStates state)
{
  SceneManagerDestroy();

  SceneManagerLoad(state);

  sceneManager.state = state;
}
