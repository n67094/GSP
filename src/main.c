#include <seven/base/types.h>
#include <seven/hw/input.h>
#include <seven/hw/irq.h>
#include <seven/hw/video.h>
#include <seven/svc/wait.h>

#include "debug/assert.h"
#include "debug/log.h"

#include "scene/scene-manager.h"

bool gba_can_draw = false;

void Update() { SceneManagerUpdate(); }

void Draw() { SceneManagerDraw(); }

void VBlank(u16 irqs)
{
  SceneManagerVBlank();

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

  SceneManagerGoTo(title_scene);
  // SceneManagerGoTo(mission_scene);

  while (true) {
    inputPoll();

    LOG_DEBUG("ok");

    Update();

    gba_can_draw = true;

    svcVBlankIntrWait();
  }

  LOG_CLOSE();
}
