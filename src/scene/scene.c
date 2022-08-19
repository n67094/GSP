#include "scene.h"

static void Empty() {}

Scene scene = {.Open = Empty, .Update = Empty, .Draw = Empty, .VBlank = Empty, .Close = Empty};

void SceneUpdate() { scene.Update(); }

void SceneDraw() { scene.Draw(); }

void SceneVBlank() { scene.VBlank(); }

void SceneGoTo(Scene nextScene)
{
  scene.Close();
  scene = nextScene;
  scene.Open();
}
