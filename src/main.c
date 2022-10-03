#include <seven/base/types.h>
#include <seven/hw/input.h>
#include <seven/hw/irq.h>
#include <seven/hw/video.h>
#include <seven/svc/wait.h>

#include "debug/assert.h"
#include "debug/log.h"

#include "scene/scene.h"

bool gba_can_draw = false;

void Update() { SceneUpdate(); }

void Draw() { SceneDraw(); }

void VBlank(u16 irqs)
{
  SceneVBlank();

  if (gba_can_draw == true) {
    Draw();
    gba_can_draw = false;
  }
}

int main(void)
{
  LOG_INIT();

  irqInitDefault();
  irqEnableFull(IRQ_VBLANK);
  irqCallbackSet(IRQ_VBLANK, VBlank, 0);

  SceneGoTo(title_scene);
  // SceneManagerGoTo(mission_scene);

  while (true) {
	gba_can_draw = false;
    inputPoll();

    Update();

    gba_can_draw = true;

    svcVBlankIntrWait();
  }

  LOG_CLOSE();
}
