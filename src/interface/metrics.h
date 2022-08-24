#ifndef INTERFACE_METRICS_H
#define INTERFACE_METRICS_H

#include "../types.h"

void MetricsInit();

void MetricsUpdate(u16 altitude, u16 speed, u16 throttle);

#endif
