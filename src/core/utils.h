#ifndef CORE_UTILS_H
#define CORE_UTILS_H

#include <seven/base/types.h>

// TODO this sould be replace with a memCpy16 asm routine (which is probably faster) ? c.f libtonc routine
void TransferPalette(const u16 *data);

void TransferTilemap();

#endif
