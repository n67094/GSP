#include <seven/hw/irq.h>
#include <seven/svc/wait.h>
#include <seven/hw/video.h>
#include <stdbool.h>

#include "debug/log.h"
#include "debug/assert.h"

bool gba_can_draw = false;

void Init() {
    LOG_INIT();
}

void Update() {}

void Draw() {}

void Destroy() {
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
    REG_DISPCNT = VIDEO_MODE_AFFINE | VIDEO_BG0_ENABLE | VIDEO_BG2_ENABLE | VIDEO_OBJ_ENABLE | VIDEO_OBJ_MAPPING_1D;

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
