#include <seven/hw/irq.h>
#include <seven/svc/wait.h>
#include <seven/hw/video.h>
#include <acsl/stdlib.h>
#include <stdbool.h>

#include "debug/log.h"
#include "debug/assert.h"

#include "scene/scene-manager.h"

bool gba_can_draw = false;

SceneManager sceneManager;

void Init() {
    LOG_INIT();

    SceneManagerInit();
}

void Update() {
    SceneManagerUpdate();
}

void Draw() {
    SceneManagerDraw();
}

void Destroy() {
    SceneManagerDestroy();

    LOG_CLOSE();
}

// VBlank callback, called only on vBlank
void VBlank(u16 irqs) {
  if(gba_can_draw == true) {
    Draw();
    gba_can_draw = false;
  }
}

int main(void)
{
    irqInitDefault();
    irqEnableFull(IRQ_VBLANK);
    irqCallbackSet(IRQ_VBLANK, VBlank, 0);

    Init();

    while (true) {
        Update();
        gba_can_draw = true;
        svcVBlankIntrWait();
    }

    Destroy();
}
