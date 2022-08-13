#ifndef CORE_UTILS_H
#define CORE_UTILS_H

#include "../types.h"

void MemCpy8(void* dest, const void* src, size_t size);

void MemCpy16(void* dest, const void* src, size_t size);

void MemCpy32(void* dest, const void* src, size_t size);

#endif
