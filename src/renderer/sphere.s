@--------------------------------------------------------------
@Function:
	@SphereRender (
	
@Requirements:
	@2^(2*scale) bytes of VRAM.
	@2^((2*gfxSize)+2) bytes of ROM for the sphere texture.
	@xxxx bytes in IWRAM for the function itself
	@xxxxxx CPU cycles.
	
@Inputs:
	@cu8 *gfxPtr: a pointer to the top left pixel of the equirectangular texture. The texture must...
	@...be 2^gfxSize pixels tall and 4*(2^gfxSize) pixels wide, formatted as a 8bpp indexed bitmap.
	
	@u8 *VRAMPtr: a pointer to the start of VRAM where the sphere it to be drawn. Must be... 
	@...2^(2*scale) bytes large
	
	@s32 pitch: the angular pitch of the sphere to be drawn, in radians, where 256 = pi/2 radians...
	@...The pitch must be within the range -255 to 255.
	
	@s32 spin: the angular spin of the sphere to be drawn, in radians, where 256 = pi/2 radians...
	@...no range limitations. The value represents the meridian to be drawn at the center.
	
	@u32 scale: the diameter of the sphere to be drawn, in tiles, where diameter = 2^scale...
	@...The scale must be within a range of 1 - 4.
	
	@u32 gfxSize: the height of the equirectangular texture to be mapped onto the sphere, where...
	@...height = 2^gfxSize. The width of the texture must be 4 times larger than the height...
	@...gfxSize must be within a range of 1 - 11. The texture must be in bitmap format, with...
	@...the texture duplicated once horizontally.
	
@Outputs:
	@A 2^(scale+1) - 1 px diameter sphere in the allocated VRAM space, formatted as 8bpp tiles

@Returns:
	@None

@Notes:
	@The VRAM space is not cleared prior to drawing. This function overwrights existing data.

@Register Utilization:
	@This is how the registers are utilized during the main loop. 
	@r0: Texture Pointer
	@r1: VRAM Pointer Right
	@r2: VRAM entry 1 Left
	@r3: X coordinate reletive to center (X)
	@r4: Y coordinate reletive to center (Y)
	@r5: VRAM entry 1 Right
	@r6: VRAM Pointer Left
	@r7: Scratch
	@r8: Scratch
	@r9: Pointer to table_group_1
	@r10: Pointer to table_group_2
	@r11: Scratch
	@r12: Scratch
	@r13: Stack pointer
	@r14: Pitch
	@r15: Program counter
	@rfiq_8: unused
	@rfiq_9: VRAM entry 2 Right
	@rfiq_10: VRAM entry 3 Right
	@rfiq_11: VRAM entry 4 Right
	@rfiq_12: VRAM entry 2 Left
	@rfiq_13: VRAM entry 3 Left
	@rfiq_14: VRAM entry 4 Left
	
@Lookup Tables
	@1/(sqrt(1-x^2)) table interleaved with a sqrt(1-X^2) table and an arcsin(X) table.
	@sin(X) table interleaved with a cos(X) table and a csc(X) table.
	@2 kilobytes of lookup tables total (yeesh...)

.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global SphereRender
	.global table_group_1
	.global table_group_2
	.type SphereRender STT_FUNC
	
SphereRender:
	push	{r4-r12, r14}			@first comes some housekeeping to handle the function arguments							
	mov		r14, r2					@move pitch to r14
	mov		r3, r3, lsl #22			@add the spin to the gfx Ptr, while moving it into a range of 0-1023
	mov		r3, r3, lsr #22
	add		r3, r3, #512
	add		r0, r0, r3	 			
	add		r0, r0, #0x80000		@move the texture pointer to the equator						
	ldr		r9, =trig_table_group_1	@setup the lut pointers.
	ldr		r10, =trig_table_group_2									
	mov		r4, #63					@set the starting Y position. It starts at radius-1 and will decrement towards 0
	mov		r2, #0					@clear the VRAM entry registers
	mov		r5, #0
map_row:
	ldr		r6, [r9, r4, lsl #4]	@starting X position is sqrt(1-Y^2)
	and		r6, r6, #0xff00
	mov		r3, r6, lsr #10
	mov		r1, r1, lsr #14			@reset the VRAM position to the start
	mov		r1, r1, lsl #14
	rsb		r12, r4, #0x40			@subtract the ypos from 64
	mov		r7, r12, lsr #3			@divide by 8 to get tile row
	add		r1, r1, r7, lsl #10		@add 1024 for each tile row
	add		r8, r3, #0x3f			@add 63 to the xpos 
	mov		r11, r8, lsr #3			@divide by 8 to get tile column
	rsb		r7, r11, #15			@get the tile column for the other side
	add		r6, r1, r7, lsl #6		
	add		r1, r1, r11, lsl #6		@add 64 for each tile column
	and		r12, r12, #0x7			@isolate the lower 3 bbits to fins the row within a tile
	add		r1, r1, r12, lsl #3		@add 8 for each row within a tile
	add		r6, r6, r12, lsl #3		@same for the left side
	tst		r8, #0x4				@find out which half of the tile row we are in
	addne	r1, r1, #0x4			@move the pointer to the other half
	addeq	r6, r6, #0x4

map_pixel:							@This is where the X position, Y position, and Pitch (P) are plugged into the function to get the coordinates of the texture.
	ldr		r12, [r9, r3, lsl #4]	@The first part of the formula is this: E = sqrt(1 - X^2) * sin(P + arcsin(y / (sqrt(1 - X^2))))
	and		r7, r12, #0xff00		@First step is to find the sqrt(1 - X^2) and its inverse.
	mov		r12, r12, lsr #16			
	muls	r8, r12, r4				@then multiply by Y and take the arcsin of that result
	mov		r12, r8, lsr #6
	ldrb	r12, [r9, r12, lsl #2]	@then add the pitch, and take the sin of that result. Hold onto the first result for later use
	adds	r12, r12, r14		 				
	mov		r11, r12
	rsbmi	r12, r12, #0
	cmp		r12, #0x100
	rsbgt	r12, r12, #0x200
	ldrb	r12, [r10, r12, lsl #2]								
	mul		r12, r7, r12				@multiply by sqrt(1 - X^2) which is still in register 7 from before
	mov		r12, r12, asr #16
	ldr		r7, [r9, r12, lsl #2]	@The next step is to convert this intermediate value (E) into the X and Y coordinates on the texture
	and 	r12, r7, #0xff			@The Y coordinate is arcsin(E), the X coordinate is arcsin(X / (sqrt(1 - E^2)))
	mov		r7, r7, asr #16
	mul		r8, r7, r3
	mov		r8, r8, asr #6
	ldrb	r8, [r9, r8, lsl #2]
	rsbgt	r8, r8, #512
	movs	r11, r11
	rsbmi	r12, r12, #0				@now we grab the pixel at those X and Y coordinates, and store them in a register
	sub		r12, r8, r12, lsl #11 	
	ldrb	r7, [r0, r12]
	orr		r5, r7, r5, lsl #8	
	sub		r12, r12, r8, lsl #1
	ldrb	r12, [r0, r12]
	orr		r2, r12, r2, ror #8		
	sub		r3, r3, #1				@If X is a multiple of 4, then we need to swap out the storage register for an empty one.
	tst		r3, #3
	bne		map_pixel
	str		r5, [r1], #-4			@store the 4 pixels
	mov		r2, r2, ror #8
	str		r2, [r6], #4
	tst		r1, #7					@is this at the boundry of a tile?
	subne	r1, r1, #56				@if so, move the address to the start of the next tile
	addne	r6, r6, #56
	mov		r5, #0					@clear the pixel register
	mov		r2, #0
	movs	r3, r3
	bne		map_pixel	
	subs	r4, r4, #1
	bne		map_row
	
@bottom half of the planet
	mov		r4, #0x3f					@put 63 in the ypos
map_row_bottom:
	ldr		r6, [r9, r4, lsl #4]	@starting X position is sqrt(1-Y^2)
	and		r6, r6, #0xff00
	mov		r3, r6, lsr #10
	mov		r1, r1, lsr #14			@reset the VRAM position to the start
	mov		r1, r1, lsl #14
	add		r12, r4, #0x3f			@add 63 to the ypos
	mov		r7, r12, lsr #3			@divide by 8 to get tile row
	add		r1, r1, r7, lsl #10		@add 1024 for each tile row
	add		r8, r3, #0x3f			@add 63 to the xpos 
	mov		r11, r8, lsr #3			@divide by 8 to get tile column
	rsb		r7, r11, #15			@get the tile column for the other side
	add		r6, r1, r7, lsl #6		
	add		r1, r1, r11, lsl #6		@add 64 for each tile column
	and		r12, r12, #0x7			@isolate the lower 3 bbits to fins the row within a tile
	add		r1, r1, r12, lsl #3		@add 8 for each row within a tile
	add		r6, r6, r12, lsl #3		@same for the left side
	tst		r8, #0x4				@find out which half of the tile row we are in
	addne	r1, r1, #0x4			@move the pointer to the other half
	addeq	r6, r6, #0x4
map_pixel_bottom:					@This is where the X position, Y position, and Pitch (P) are plugged into the function to get the coordinates of the texture.
	ldr		r12, [r9, r3, lsl #4]	@The first part of the formula is this: E = sqrt(1 - X^2) * sin(P + arcsin(y / (sqrt(1 - X^2))))
	and		r7, r12, #0xff00		@First step is to find the sqrt(1 - X^2) and its inverse.
	mov		r12, r12, lsr #16			
	muls	r8, r12, r4				@then multiply by Y and take the arcsin of that result
	mov		r12, r8, lsr #6
	ldrb	r12, [r9, r12, lsl #2]	@then add the pitch, and take the sin of that result. Hold onto the first result for later use
	subs	r12, r12, r14		 				
	mov		r11, r12
	rsbmi	r12, r12, #0
	cmp		r12, #0x100
	rsbgt	r12, r12, #0x200
	ldrb	r12, [r10, r12, lsl #2]								
	mul		r12, r7, r12			@multiply by sqrt(1 - X^2) which is still in register 7 from before
	mov		r12, r12, asr #16
	ldr		r7, [r9, r12, lsl #2]	@The next step is to convert this intermediate value (E) into the X and Y coordinates on the texture
	and 	r12, r7, #0xff			@The Y coordinate is arcsin(E), the X coordinate is arcsin(X / (sqrt(1 - E^2)))
	mov		r7, r7, asr #16
	mul		r8, r7, r3
	mov		r8, r8, asr #6
	ldrb	r8, [r9, r8, lsl #2]
	rsbgt	r8, r8, #512
	movs	r11, r11
	rsbmi	r12, r12, #0			@now we grab the pixel at those X and Y coordinates, and store them in a register
	add		r12, r8, r12, lsl #11 	
	ldrb	r7, [r0, r12]
	orr		r5, r7, r5, lsl #8	
	sub		r12, r12, r8, lsl #1
	ldrb	r12, [r0, r12]
	orr		r2, r12, r2, ror #8		
	sub		r3, r3, #1				@If X is a multiple of 4, then we need to swap out the storage register for an empty one.
	tst		r3, #3
	bne		map_pixel_bottom
	str		r5, [r1], #-4			@store the 4 pixels
	mov		r2, r2, ror #8
	str		r2, [r6], #4
	tst		r1, #7					@is this at the boundry of a tile?
	subne	r1, r1, #56				@if so, move the address to the start of the next tile
	addne	r6, r6, #56
	mov		r5, #0					@clear the pixel register
	mov		r2, #0
	movs	r3, r3
	bne		map_pixel_bottom	
	subs	r4, r4, #1
	bne		map_row_bottom
	mov		r0, #0
	str		r0, earth_in_progress
	pop		{r4-r12, r14}
pauseexit:
	bx lr
	
.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global EarthPause
	.global EarthResume
	.global earth_in_progress
	.type SphereRender STT_FUNC
	
earth_in_progress:
	.word	0
	
earth_saved_progress: @all the registers 0-12, 14, fiq8-fiq14, spsr so that the sphere can resume where it left off
	.word	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	EarthPause:
	adr		r0, earth_saved_progress + 16
	stmia	r0, {r4-r11}							@save registers 4-11
	ldr		r5, [r13, #0x24] 						@load r14 from the system stack
	str		r5, earth_saved_progress + (13 * 4)		@save r14
	ldr		r5, [r13, #0x1c]						@load the psr from the system stack
	str		r5, earth_saved_progress + (23 * 4)		@save the psr
	msr		cpsr_c, #0xd1							@switch to fiq mode
	adr		r4, earth_saved_progress + (15 * 4)		@prepare to save the fiq registers
	stmia	r4, {r8-r14}							@save the fiq registers	
	msr		cpsr_c, #0xd2							@switch to irq mode
	ldmia	r13, {r0-r3, r5, r12}					@load the remaining registers from the interrupt stack
	adr		r4, earth_saved_progress
	stmia	r4, {r0-r3}								@save registers 0-3
	str		r5, earth_saved_progress + (12 * 4)		@save register 12
	str		r12, earth_saved_progress + (22 * 4)	@save the program counter
	adr		r4, pauseexit							@store the exit of the sphere draw function to the interrupt stack
	str		r4, [r13, #20]							@so that the function ends immediately, and the next frame can start	
	msr		cpsr_c, #0xdf							@switch back to system mode
	bx		lr
	
EarthResume:
	push	{r4-r12, r14}							@save all registers
	adr		r0, earth_saved_progress
	ldmia	r0, {r0-r12, r14}						@restore registers 0-12 and 14
	msr		cpsr_c, #0xd1							@switch to fiq mode
	ldr		r9, earth_saved_progress + (23 * 4)		@load the psr
	msr		spsr, r9								@prepare to swap the psr
	adr		r8, earth_saved_progress + (15 * 4)		@prepare to restore the fiq registers
	ldmia	r8, {r8-r15}^							@restore the fiq registers and swap out the psr
