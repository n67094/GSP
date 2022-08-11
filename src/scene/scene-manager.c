#include "scene-manager.h"

static void Empty() {}

Scene scene = {.Open = Empty, .Update = Empty, .Draw = Empty, .VBlank = Empty, .Close = Empty};

void SceneManagerUpdate() { scene.Update(); }

void SceneManagerDraw() { scene.Draw(); }

void SceneManagerVBlank() { scene.VBlank(); }

void SceneManagerGoTo(Scene nextScene)
{
  scene.Close();
  scene = nextScene;
  scene.Open();
}
