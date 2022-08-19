#ifndef INTERFACE_METRICS_H
#define INTERFACE_METRICS_H

#include "../types.h"

void MetricsInit();

void MetricsUpdate(s32 altitude, s32 speed, s32 throttle);

#endif
