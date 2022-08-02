#include "tonc_types.h"
#include "render_sphere.h"
#include "main.h"

int main(){
	s32 pitch = 0;
	s32 spin = 0;
	u32 keypad;
	*(vu16 *)0x4000000 = 0x442; //enable bg2, mode 2
	*(vu16 *)0x400000C = 0x80; //setup bg2
	
	
	transferPalette();
	transferTilemap();
	
	while(1){
		keypad = *(vu16 *)0x4000130;
		if((keypad & 0x40) == 0) {
			pitch++;
		}
		if((keypad & 0x80) == 0) {
			pitch--;
		}
		if((keypad & 0x20) == 0) {
			spin--;
		}
		if((keypad & 0x10) == 0) {
			spin++;
		}
		render_sphere(Earth_Equirectangular, (u8 *)0x6000000, pitch, spin, 7, 9);
	}
}

void transferPalette(){
	vu16 *palettePtr = (vu16 *)0x5000000;
	for (u32 i = 0; i < 256; i++) {
		palettePtr[i] = test_Pal[i];
	}
}

void transferTilemap(){
	vu16 * tilemapPtr = (vu16 *)0x6000000;
	for (u32 i = 0; i < 128; i++) {
		tilemapPtr[i] = (((i << 1) + 1) << 8) + (i << 1);
	}
	for (u32 i = 0; i < 2; i++){
		tilemapPtr[i] = 0xffff;
	}	
	
	
}