#include <seven/base/addresses.h>
#include <seven/hw/memory.h>

#include "../types.h"

#include "utils.h"

void MemCpy8(void* dest, const void* src, size_t size) {
	u8 *_src = (u8 *)src;
	u8 *_dest = (u8 *)dest;

	for (u32 i = 0; i < size; ++i) {
    _dest[i] = _src[i];
	}
}

void MemCpy16(void* dest, const void* src, size_t size) {
	u16 *_src = (u16 *)src;
	u16 *_dest = (u16 *)dest;
	size = size >> 1;
	
	for (u32 i = 0; i < size; ++i) {
    _dest[i] = _src[i];
	}
}

void MemCpy32(void* dest, const void* src, size_t size) {
	u32 *_src = (u32 *)src;
	u32 *_dest = (u32 *)dest;
	size = size >> 2;
	
	for (u32 i = 0; i < size; ++i) {
    _dest[i] = _src[i];
	}
}
