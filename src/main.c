#include <seven/base/types.h>
#include <seven/hw/input.h>
#include <seven/hw/irq.h>
#include <seven/hw/video.h>
#include <seven/svc/wait.h>

#include "debug/assert.h"
#include "debug/log.h"

#include "scene/scene.h"

void Update() { SceneUpdate(); }

void Draw() { SceneDraw(); }

void VBlank(u16 irqs) { 
	SceneVBlank();
}

extern u32 earth_in_progress;

int main(void)
{
  LOG_INIT();

  irqInitDefault();
  irqEnableFull(IRQ_VBLANK);
  irqCallbackSet(IRQ_VBLANK, VBlank, 0);

  SceneGoTo(title_scene);

  while (true) {
	if(earth_in_progress == 0){
      svcVBlankIntrWait(); //wait for Vblank
	}
	irqDisable(IRQ_VBLANK); //disable Vertical interrupts
    inputPoll(); //poll inputs
    Draw(); //perform all calculations that must be done during Vblank
    Update(); //perform any calculations that can be done outside of Vblank, but must be done before next Vblank
	irqEnable(IRQ_VBLANK); //reenable Vertical interrupts, next frame will resume at irqDisable
  } //repeat

  LOG_CLOSE();
}
