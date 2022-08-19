.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global DrawCylinderWall
	.type DrawCylinderWall STT_FUNC
	
DrawCylinderWall:
	bx		lr
	
.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global SetupPosTable
	.type SetupPosTable STT_FUNC

SetupPosTable:
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
	.word	
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
	stmia	r1, {r9-r10}
	subs	r2, r2, #1
	subne	r1, r1, #0x3b8 		@Unless we just finished row 8, move the VRAM pointer back to the next row of Tile 1
	bne		Transfer1PixelRow
	add		r1, r1, #8			@if we are in a new row of tiles, advance the VRAM pointer accordingly
	movs	r3, r1, lsl #18		@check if the lower 14 bits of the VRAM address are all 0 (which means we reached the end of the 16kb)
	bne		Transfer1TileRow
	ldr		r13, R13Temp
	pop		{r4-r12, r14}
	bx		lr
