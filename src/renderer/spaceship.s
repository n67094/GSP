.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global DrawCylinderWall
	.type DrawCylinderWall STT_FUNC
	
@register usage
@r0: texture pointer center
@r1: texture width (2^n pixels), 
@r2: horizontal scale
@r3: radius - vertical counter
@r4: buffer address, middle point
@r5: position table address
@r6: buffer address
@r7: texture height (2^n pixels), shift value
@r8: texture pointer
@r9: horizontal counter
@r11: scratch
@r12: scratch
@r13: texture width (pixels)
@r14: original radius

DrawCylinderWall:
	push	{r4-r12, r14}			@store the existing registers
	ldr		r4, [r13, #40]			@load the fourth argument, the buffer address, from the stack
	ldr		r7, [r13, #44]			@load the texture height
	str		r13, R13Temp			@store the stack pointer
	ldr		r5, =pos_table_1		@load the position table address
	mov		r14, r3
	mov		r10, #1
	mov		r13, r10, lsl r1		@calculate the width of the texture in pixels
	sub		r7, r7, #2				@divide the height of the texture in 4 
	rsb		r7, r7, #8				@find the shift value needed to link the angle to texture vertical position.
DrawOneLineUp:	
	ldr		r11, [r5], #4			@load the value at the position table
	mov		r12, r11, lsr #24		@isolate the angle value
	mov		r12, r12, lsr r7
	sub		r8, r0, r12, lsl r1		
	sub		r6, r4, r3, lsl #7		@get the correct row of the buffer
	and		r11, r11, #0xff
	add		r6, r6, r11				@get the correct starting pixel of the buffer
	mov		r9, r13, lsl #8
DrawOnePixelUp:
	ldrb	r11, [r8, r9, lsr #8]	@load the pixel from the texture bitmap
	strb	r11, [r6], #1			@store the pixel in the buffer, post increment
	subs	r9, r9, r2				@subtract the horizontal scale to reach the next pixel
	bpl		DrawOnePixelUp			@continue until we run out of horizontal pixels in the texture
	subs	r3, r3, #1				@decrement the vertical counter
	bpl		DrawOneLineUp
BottomHalf:							@now we repeat the process, this time for the bottom half of the cylinder
	ldr		r5, =pos_table_1		@reset the position table address
	mov		r3, r14
DrawOneLineDown:	
	ldr		r11, [r5], #4			@load the value at the position table
	mov		r12, r11, lsr #24		@isolate the angle value
	mov		r12, r12, lsr r7
	add		r8, r0, r12, lsl r1		@this time add to get the texture pointer
	add		r6, r4, r3, lsl #7		@and add to get the buffer address
	and		r11, r11, #0xff
	add		r6, r6, r11				@get the correct starting pixel of the buffer
	mov		r9, r13, lsl #8
DrawOnePixelDown:
	ldrb	r11, [r8, r9, lsr #8]	@load the pixel from the texture bitmap
	strb	r11, [r6], #1			@store the pixel in the buffer, post increment
	subs	r9, r9, r2				@subtract the horizontal scale to reach the next pixel
	bpl		DrawOnePixelDown		@continue until we run out of horizontal pixels in the texture
	subs	r3, r3, #1				@decrement the vertical counter
	bpl		DrawOneLineDown	
	ldr		r13, R13Temp
	pop		{r4-r12, r14}
	bx		lr
	
.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global SetupPosTableCylinder
	.global InverseTable
	.type SetupPosTableCylinder STT_FUNC

@Register Usage

@r0: cos(pitch)
@r1: radius
@r2: center xPos
@r3: current yPos
@r4: pointer to position table
@r5: yPos Update Value
@r6: trig table 1
@r7: scratch
@r8: scratch

@angle is in top byte, xpos is in bottom byte

SetupPosTableCylinder:
	push	{r4-r9}					@store the existing registers
	ldr		r4, =pos_table_1
	mov		r3, #0x10000			@starting y pos is 1
	mov		r5, r1, lsl #1
	adr		r8, InverseTable
	ldrh	r5, [r8, r5]			@load the inverse of radius into the y update value
	ldr		r6, =trig_table_group_1
	orr		r8, r2, #0xff000000		@first pixel has texture value of pi/2			
	str		r8, [r4], #4			@first pixel is always at same x value as center x	
	subs	r3, r3, r5				@add the update value to the ypos
setupPixel:
	mov		r8, r3, lsr #8
	ldr		r8, [r6, r8, lsl #2]	@load the sqrt(1-Y^2) and the arcsin(Y)
	and		r9, r8, #0xff
	and		r8, r8, #0xff00
	mul		r7, r8, r1				@multiply by radius
	mul		r8, r7, r0				@multiply by cos(pitch)
	add		r8, r2, r8, lsr #24		@store the x pos in the bottom byte
	orr		r8, r8, r9, lsl #24		@store the angle at the upper byte
	str		r8, [r4], #4			@store the x value at the corrosponding y position in the table
	subs	r3, r3, r5				@add the update value to the ypos
	bhi		setupPixel
	mul		r8, r1, r0				@one special case for the center pixel
	add		r8, r2, r8, asr #8
	str		r8, [r4]				
	pop		{r4-r9}
	bx		lr
	
	
.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global ClearBuffer
	.type ClearBuffer STT_FUNC
	
@this function sets 16kb of memory to zero
@r0 contains the pointer to the start of the buffer
@takes ~ 4,575 cycles

R13Temp:
	.word	0
ClearBuffer:
	push	{r4-r12, r14}
	str		r13, R13Temp
	mov		r1, #0				@set registers 1-13 to 0
	mov		r2, #0
	mov		r3, #0
	mov		r4, #0
	mov		r5, #0
	mov		r6, #0
	mov		r7, #0
	mov		r8, #0
	mov		r9, #0
	mov		r10, #0
	mov		r11, #0
	mov		r12, #0
	mov		r13, #0
	mov		r14, #32			@r14 holds the number of repetitions
Clear512Bytes:
	stmia	r0!, {r1-r13}		@clear 52 bytes
	stmia	r0!, {r1-r13}		@clear another 52 bytes
	stmia	r0!, {r1-r13}		@repeat a total of 9 times
	stmia	r0!, {r1-r13}
	stmia	r0!, {r1-r13}
	stmia	r0!, {r1-r13}
	stmia	r0!, {r1-r13}
	stmia	r0!, {r1-r13}
	stmia	r0!, {r1-r13}
	stmia	r0!, {r1-r11}		@finish by clearing 44 bytes
	subs	r14, r14, #1		@reduce the repetitions counter
	bne		Clear512Bytes		@repeat clearing 512 bytes if not done
	ldr		r13, R13Temp
	pop		{r4-r12, r14}
	bx		lr
	
.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global TransferBuffer
	.type TransferBuffer STT_FUNC

@This function transfers a 16kb bitmap buffer to VRAM, while converting it into tiled format.
@r0 contains the pointer to the start of the buffer
@r1 contains the pointer to the start of this BGs VRAM
@takes ~17,750 cycles
@for reference, a 16kb dma from the same addresses would take ~12,290 cycles
	
TransferBuffer:
	push	{r4-r12, r14}
	str		r13, R13Temp
Transfer1TileRow:	
	mov		r2, #8
Transfer1PixelRow:
	ldmia	r0!, {r3-r14}		@load 48 bytes from the bitmap buffer
	stmia	r1, {r3-r4}			@store 8 bytes into VRAM (one row of one tile)
	add		r1, r1, #64			@advance the VRAM pointer to the next tile
	stmia	r1, {r5-r6}			@store the next 8 bytes into VRAM...
	add		r1, r1, #64
	stmia	r1, {r7-r8}
	add		r1, r1, #64
	stmia	r1, {r9-r10}
	add		r1, r1, #64
	stmia	r1, {r11-r12}
	add		r1, r1, #64
	stmia	r1, {r13-r14}
	add		r1, r1, #64
	ldmia	r0!, {r3-r14}		@load another 48 bytes from the bitmap buffer
	stmia	r1, {r3-r4}
	add		r1, r1, #64
	stmia	r1, {r5-r6}
	add		r1, r1, #64
	stmia	r1, {r7-r8}
	add		r1, r1, #64
	stmia	r1, {r9-r10}
	add		r1, r1, #64
	stmia	r1, {r11-r12}
	add		r1, r1, #64
	stmia	r1, {r13-r14}
	add		r1, r1, #64
	ldmia	r0!, {r3-r10}		@load a final 32 bytes from the bitmap buffer
	stmia	r1, {r3-r4}
	add		r1, r1, #64
	stmia	r1, {r5-r6}
	add		r1, r1, #64
	stmia	r1, {r7-r8}
	add		r1, r1, #64
	stmia	r1!, {r9-r10}		@write-back the address this time, because if that was the last row from a tile, the next address is sequential
	subs	r2, r2, #1
	subne	r1, r1, #0x3c0 		@Unless we just finished row 8, move the VRAM pointer back to the next row of Tile 1
	bne		Transfer1PixelRow
	movs	r3, r1, lsl #18		@check if the lower 14 bits of the VRAM address are all 0 (which means we reached the end of the 16kb)
	bne		Transfer1TileRow
	ldr		r13, R13Temp
	pop		{r4-r12, r14}
	bx		lr

InverseTable:
	.hword	0xFFFF, 0xFFFF, 0x8000, 0x5555, 0x4000, 0x3333, 0x2AAA, 0x2492, 0x2000, 0x1C71, 0x1999, 0x1745, 0x1555, 0x13B1, 0x1249, 0x1111
	.hword	0x1000, 0x0F0F, 0x0E38, 0x0D79, 0x0CCC, 0x0C30, 0x0BA2, 0x0B21, 0x0AAA, 0x0A3D, 0x09D8, 0x097B, 0x0924, 0x08D3, 0x0888, 0x0842
	.hword	0x0800, 0x07C1, 0x0787, 0x0750, 0x071C, 0x06EB, 0x06BC, 0x0690, 0x0666, 0x063E, 0x0618, 0x05F4, 0x05D1, 0x05B0, 0x0590, 0x0572
	.hword	0x0555, 0x0539, 0x051E, 0x0505, 0x04EC, 0x04D4, 0x04BD, 0x04A7, 0x0492, 0x047D, 0x0469, 0x0456, 0x0444, 0x0432, 0x0421, 0x0410, 0x0400
	
	.align  2
pos_table_1:
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 
	
pos_table_2:
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0