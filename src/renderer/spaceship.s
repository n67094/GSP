.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global DrawBase
	.type DrawBase STT_FUNC
	
DrawBase:






.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global SetupPosTableCone
	.type SetupPosTableCone STT_FUNC
	
@register usage
@r0: pitch, then sin(pitch)
@r1: radius_1
@r2: radius_2
@r3: xPos offset from center
@r4: current big yPos
@r5: pointer to position table
@r6: big yPos Update Value
@r7: trig table 1
@r8: current small yPos
@r9: small y Pos partial
@r10: small x Pos partial
@r11: scratch
@r12: scratch
@r13: scratch
@r14: small r / big r


SetupPosTableCone:	
	push	{r4-r12, r14}			@store the existing registers
	ldrsb	r8, [r13, #40]			@load the fifth argument, the height, from the stack
	str		r13, R13Temp			@store the stack pointer
	ldr		r5, =pos_table
	ldr		r7, =trig_table_group_2
	adds	r9, r0, #0x100			@\
	cmp		r9, #0x100				@|get the cosine of pitch
	rsbhi	r9, r9, #0x200			@|
	ldrneb	r9, [r7, r9, lsl #2]	@/
	rsbmi	r0, r0, #0				@\
	ldrb	r0, [r7, r0, lsl #2]	@|load the sin of pitch
	rsbmi	r0, r0, #0				@/
	ldr		r7, =trig_table_group_1
	mul		r8, r9, r8				@multiply height by cos(pitch)
	add		r9, r3, r8, asr #8		@add the center x_0 to the result to get the center x_1
	orr		r3, r3, r9, lsl #16		@combine x_0 and x_1
	orr		r3, r3, r1, lsl #8		@combine with y_0
	orr		r3, r3, r2, lsl #24		@combine with y_1
	str		r3, [r5], #4			@store the initial positions into the pos table
	mov		r4, #0x10000			@move 1 into the initial y pos
	ldr		r11, =InverseTable
	mov		r12, r1, lsl #1			@\inverse of big radius to get ypos update value
	ldrh	r6, [r11, r12]			@/
	mul		r14, r6, r2				@multiply small radius by inverse of big radius to get the small update value
	mov		r3, #0					@set the initial x pos offset to 0
	mov		r9, #-0x8001			@reset the small y value
	mov		r10, #-0x8001			@reset the small x value
	subs	r4, r4, r6				@subtract the update value to the ypos
setupPixelCone:
	mov		r11, r4, lsr #8
	ldr		r11, [r7, r11, lsl #2]	@load the sqrt(1-Y^2) and the arcsin(Y)
	and		r12, r11, #0xff			@\isolate the angle
	mov		r12, r12, lsl #8		@/
	and		r11, r11, #0xff00		@isolate the x pos
	mul		r13, r11, r1			@multiply by radius
	mul		r11, r13, r0			@multiply by cos(pitch)
	rsbs	r13, r3, r11, asr #24	@subtract the new xpos from the last xpos
	beq		noXposBigUpdate
	bmi		negativeBigXUpdate
	orr		r12, r12, #1			@set the update into the pos table (positive x)
positiveBigXUpdate:
	adds	r10, r10, r14			@increase the small x pos
	orrpl	r12, r12, #8			@set the small xpos update
	subpl	r10, r10, #0x10000		@reset the small xpos counter
	subs	r13, r13, #1			@check if reached the end of big x pos updates
	strneh	r12, [r5], #2			@store the pixel data if there is more
	bicne	r12, r12, #0x3c			@clear the pixel data to prepare for the next 
	bne		positiveBigXUpdate
	b		BigXUpdateDone
negativeBigXUpdate:
	orr		r12, r12, #3			@set the update into the pos table (negative x)
negativeBigXRepeat:
	adds	r10, r10, r14			@increase the small x pos
	orrpl	r12, r12, #0x18			@set the small xpos negative update if needed
	subpl	r10, r10, #0x10000		@reset the small xpos counter if needed
	adds	r13, r13, #1			@check if reached the end of big pos updates
	strneh	r12, [r5], #2			@store the pixel data if there is more
	bicne	r12, r12, #0x3c			@clear the pixel data to prepare for the next 
	bne		negativeBigXRepeat	
BigXUpdateDone:
	mov		r3, r11, asr #24		@update the last xpos with the new value
noXposBigUpdate:
	adds	r9, r9, r14				@increase the small ypos counter
	orrpl	r12, r12, #0x20			@set the small ypos update
	subpl	r9, r9, #0x10000		@reset the small ypos counter
	orr		r12, r12, #0x4			@set the big ypos update
	strh	r12, [r5], #2			@store the x value at the corrosponding y position in the table
	subs	r4, r4, r6				@subtract the update value to the ypos
	bpl		setupPixelCone
	bic		r12, r12, #0x3f			@sometimes due to rounding errors, one more entry is needed
	orr		r12, r12, #0x24			@same angle as last entry, both y decrement.
	strh	r12, [r5], #2			@store the special value
	ldr		r13, R13Temp
	pop		{r4-r12, r14}
	bx		lr

.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global DrawConeWall
	.type DrawConeWall STT_FUNC
	
@register usage
@r0: buffer top pointer
@r1: buffer bottom pointer
@r2: texture width (2^n pixels)
@r3: texture height (2^n pixels)
@r4: delta x
@r5: delta y
@r6: texture top pointer
@r7: texture bottom pointer
@r8: error value
@r9: buffer x update value
@r10: inverse table pointer
@r11: counter
@r12: scale
@r13: scratch
@r14: scratch
@rfiq8: original x_0 big
@rfiq9: original y_0 big
@rfiq10: original x_0 small
@rfiq11: original y_0 small
@rfiq12: center buffer address
@rfiq13: texture middle pointer
@rfiq14: pos table pointer

@pos table format:
@0: big x increment, 0 = no increment
@1: big x sign, 0 = positive
@2: big y decrement, 0 = no decrement (always positive)
@3: small x increment, 0 = no increment
@4: small x sign, 0 = positive
@5: small y decrement, 0 = no decrement (always positive)
@8-15: angle value

@first entry
@byte 1:x_0 big 
@byte 2: y_0 big offset from center
@byte 3: x_0 small
@byte 4: y_0 small offset from center

DrawConeWall:
	push	{r4-r12, r14}			@store the existing registers
	str		r13, R13Temp			@store the stack pointer
	ldr		r10, =InverseTable
	msr		cpsr_c, #0x11			@switch to fiq mode, registers 8-14 now reference the fiq registers
	mov		r12, r1					@move the buffer center pointer to the right register
	mov		r13, r0					@move the texture center pointer to the right register
	ldr		r14, =pos_table
	ldr		r4, [r14], #4			@load the initial x and y coordinates into r4
	and		r8, r4, #0xff			@get the initial x pos big from the pos table
	and		r6, r4, #0xff00			@isolate the y_0 big offset
	mov		r9, r6, lsr #8			@place the big y_0 offset in r9
	and		r6, r4, #0xff0000		@isolate the x_0 small
	mov		r10, r6, lsr #16		@place it into the correct register
	mov		r11, r4, lsr #24		@place the y_0 small offset in r11
@									@below this point is a section copy-pasted from the normal loop.
	ldrh	r4, [r14], #2			@load the next pos table entry, post increment
	movs	r5, r4, lsl #27			@check bits 5 and 4. 
	submi	r10, r10, #2			@decrease small x by 2 if bit 4 is set
	subcs	r11, r11, #1			@decrement small y if bit 5 is set
	movs	r5, r5, lsl #2			@check bits 3 and 2
	addcs	r10, r10, #1			@increment small x if bit 3 is set
	submis	r9, r9, #1				@decrement big y if bit 2 is set
	bmi		DrawConeEnd				@if we are done already, branch to the end of the function
ConeNewLine:
	movs	r5, r5, lsl #2			@check bits 1 and 0
	addmi	r8, r8, #1				@increment big x if bit 0 is set
	subcs	r8, r8, #2				@decrease big x by 2 if bit 1 is set
	rsb		r5, r3, #18				@\convert the angle to texture vertical offset
	mov		r4, r4, lsr r5			@/
	sub		r6, r13, r4, lsl r2		@multiply by texture width and subtract from texture middle to get texture top
	add		r7, r13, r4, lsl r2		@multiply by texture width and add to texture middle to get texture bottom
	sub		r0, r12, r9, lsl #7		@subtract the big y from the middle buffer pointer to get the upper pointer
	add		r0, r0, r8				@add the big x to finish the upper pointer
	add		r1, r12, r9, lsl #7		@add the big y from the middle buffer pointer to get the lower pointer
	add		r1, r1, r8				@add the big x to finish the lower pointer
	sub		r5, r9, r11				@calculate delta y, should always be positive
	subs	r4, r10, r8				@calculate the delta x
	msr		cpsr_c, #0x1F			@switch to system mode
	rsbmi	r4, r4, #0				@make sure the delta x is positive
	movmi	r9, #-1					@\set the buffer update value accordingly
	movpl	r9, #1					@/
	mov		r13, #0x10000			@\set the counter to the texture width, lsl 16
	mov		r11, r13, lsl r2		@/
	cmp		r4, r5					@compare delta x to delta y
	bmi		YGreater
XGreater:
	rsb		r8, r4, r5, lsl #1		@error value starts out as 2*dy - dx
	sub		r8, r8, r5, lsl #1		@undo the first error update
	mov		r13, r4, lsl #1			@\load the inverse of dx
	ldrh	r13, [r10, r13]			@/
	mov		r12, r13, lsl r2		@multiply by the texture width
PlotXPixel:
	ldrb	r13, [r6, r11, lsr #16]	@load the upper pixel from the texture
	strb	r13, [r0], r9			@store the color into the upper buffer, update x value
	ldrb	r14, [r7, r11, lsr #16]	@load the lower pixel from the texture
	strb	r14, [r1], r9			@store the color into the lower buffer, update x value
	adds	r8, r8, r5, lsl #1		@update the error value
	strplb	r13, [r0], #128			@store the corner, and update the upper buffer value if y is changed
	strplb	r14, [r1], #-128		@store the corner, and update the lower buffer value if y is changed
	subpl	r8, r8, r4, lsl #1		@update the error value is y is changed
	subs	r11, r11, r12			@update the counter
	bpl		PlotXPixel				@repeat if not done
	b		BresenhamEnd
YGreater:
	rsb		r8, r5, r4, lsl #1		@error value starts out as 2*dx - dy
	sub		r8, r8, r4, lsl #1		@undo the first error update
	mov		r13, r5, lsl #1			@\load the inverse of dy
	ldrh	r13, [r10, r13]			@/
	mov		r12, r13, lsl r2		@multiply by the texture width
PlotYPixel:
	ldrb	r13, [r6, r11, lsr #16]	@load the upper pixel from the texture
	strb	r13, [r0], #128			@store the color into the upper buffer, update x value
	ldrb	r14, [r7, r11, lsr #16]	@load the lower pixel from the texture
	strb	r14, [r1], #-128		@store the color into the lower buffer, update x value
	adds	r8, r8, r4, lsl #1		@update the error value
	strplb	r13, [r0], r9			@store the corner, and update the upper buffer value if y is changed
	strplb	r14, [r1], r9			@store the corner, and update the lower buffer value if y is changed
	subpl	r8, r8, r5, lsl #1		@update the error value is y is changed
	subs	r11, r11, r12			@update the counter
	bpl		PlotYPixel				@repeat if not done
BresenhamEnd:
	msr		cpsr_c, #0x11			@switch to fiq mode, registers 8-14 now reference the fiq registers
	ldrh	r4, [r14], #2			@load the next pos table entry, post increment
	movs	r5, r4, lsl #27			@check bits 5 and 4. 
	submi	r10, r10, #2			@decrease small x by 2 if bit 4 is set
	subcs	r11, r11, #1			@decrement small y if bit 5 is set
	movs	r5, r5, lsl #2			@check bits 3 and 2
	addcs	r10, r10, #1			@increment small x if bit 3 is set
	submis	r9, r9, #1				@decrement big y if bit 2 is set
	bpl		ConeNewLine				@if we are done, exit the loop, otherwise, reset for new line
DrawConeEnd:
	msr		cpsr_c, #0x1f
	ldr		r13, R13Temp
	pop		{r4-r12, r14}
	bx		lr
	
.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global DrawConeWallBack
	.type DrawConeWallBack STT_FUNC
	
@This is a nearly ideitical copy of the DrawConeWall funciton, with just a few lines of code changed.
@If we run short of IWRAM, self-modifying code can be used to reuse the above section.
	
	DrawConeWallBack:
	push	{r4-r12, r14}			@store the existing registers
	str		r13, R13Temp			@store the stack pointer
	ldr		r10, =InverseTable
	msr		cpsr_c, #0x11			@switch to fiq mode, registers 8-14 now reference the fiq registers
	mov		r12, r1					@move the buffer center pointer to the right register
	mov		r13, r0					@move the texture center pointer to the right register
	ldr		r14, =pos_table
	ldr		r4, [r14], #4			@load the initial x and y coordinates into r4
	and		r8, r4, #0xff			@get the initial x pos big from the pos table
	and		r6, r4, #0xff00			@isolate the y_0 big offset
	mov		r9, r6, lsr #8			@place the big y_0 offset in r9
	and		r6, r4, #0xff0000		@isolate the x_0 small
	mov		r10, r6, lsr #16		@place it into the correct register
	mov		r11, r4, lsr #24		@place the y_0 small offset in r11
@									@below this point is a section copy-pasted from the normal loop.
	ldrh	r4, [r14], #2			@load the next pos table entry, post increment
	movs	r5, r4, lsl #27			@check bits 5 and 4. 
	addmi	r10, r10, #2			@increase small x by 2 if bit 4 is set ||DIFFERENT||
	subcs	r11, r11, #1			@decrement small y if bit 5 is set
	movs	r5, r5, lsl #2			@check bits 3 and 2
	subcs	r10, r10, #1			@decrement small x if bit 3 is set ||DIFFERENT||
	submis	r9, r9, #1				@decrement big y if bit 2 is set
	bmi		DrawConeBackEnd			@if we are done already, branch to the end of the function
ConeNewLineBack:
	movs	r5, r5, lsl #2			@check bits 1 and 0
	submi	r8, r8, #1				@decrement big x if bit 0 is set ||DIFFERENT||
	addcs	r8, r8, #2				@increase big x by 2 if bit 1 is set ||DIFFERENT||
	rsb		r4, r4, #0x20000		@||NEW||
	rsb		r5, r3, #18				@\convert the angle to texture vertical offset
	mov		r4, r4, lsr r5			@/
	sub		r6, r13, r4, lsl r2		@multiply by texture width and subtract from texture middle to get texture top
	add		r7, r13, r4, lsl r2		@multiply by texture width and add to texture middle to get texture bottom
	sub		r0, r12, r9, lsl #7		@subtract the big y from the middle buffer pointer to get the upper pointer
	add		r0, r0, r8				@add the big x to finish the upper pointer
	add		r1, r12, r9, lsl #7		@add the big y from the middle buffer pointer to get the lower pointer
	add		r1, r1, r8				@add the big x to finish the lower pointer
	sub		r5, r9, r11				@calculate delta y, should always be positive
	subs	r4, r10, r8				@calculate the delta x
	msr		cpsr_c, #0x1F			@switch to system mode
	rsbmi	r4, r4, #0				@make sure the delta x is positive
	movmi	r9, #-1					@\set the buffer update value accordingly
	movpl	r9, #1					@/
	mov		r13, #0x10000			@\set the counter to the texture width, lsl 16
	mov		r11, r13, lsl r2		@/
	cmp		r4, r5					@compare delta x to delta y
	bmi		YGreaterBack
XGreaterBack:
	rsb		r8, r4, r5, lsl #1		@error value starts out as 2*dy - dx
	sub		r8, r8, r5, lsl #1		@undo the first error update
	mov		r13, r4, lsl #1			@\load the inverse of dx
	ldrh	r13, [r10, r13]			@/
	mov		r12, r13, lsl r2		@multiply by the texture width
PlotXPixelBack:
	ldrb	r13, [r6, r11, lsr #16]	@load the upper pixel from the texture
	strb	r13, [r0], r9			@store the color into the upper buffer, update x value
	ldrb	r14, [r7, r11, lsr #16]	@load the lower pixel from the texture
	strb	r14, [r1], r9			@store the color into the lower buffer, update x value
	adds	r8, r8, r5, lsl #1		@update the error value
	strplb	r13, [r0], #128			@store the corner, and update the upper buffer value if y is changed
	strplb	r14, [r1], #-128		@store the corner, and update the lower buffer value if y is changed
	subpl	r8, r8, r4, lsl #1		@update the error value is y is changed
	subs	r11, r11, r12			@update the counter
	bpl		PlotXPixelBack			@repeat if not done
	b		BresenhamEndBack
YGreaterBack:
	rsb		r8, r5, r4, lsl #1		@error value starts out as 2*dx - dy
	sub		r8, r8, r4, lsl #1		@undo the first error update
	mov		r13, r5, lsl #1			@\load the inverse of dy
	ldrh	r13, [r10, r13]			@/
	mov		r12, r13, lsl r2		@multiply by the texture width
PlotYPixelBack:
	ldrb	r13, [r6, r11, lsr #16]	@load the upper pixel from the texture
	strb	r13, [r0], #128			@store the color into the upper buffer, update x value
	ldrb	r14, [r7, r11, lsr #16]	@load the lower pixel from the texture
	strb	r14, [r1], #-128		@store the color into the lower buffer, update x value
	adds	r8, r8, r4, lsl #1		@update the error value
	strplb	r13, [r0], r9			@store the corner, and update the upper buffer value if y is changed
	strplb	r14, [r1], r9			@store the corner, and update the lower buffer value if y is changed
	subpl	r8, r8, r5, lsl #1		@update the error value is y is changed
	subs	r11, r11, r12			@update the counter
	bpl		PlotYPixelBack			@repeat if not done
BresenhamEndBack:
	msr		cpsr_c, #0x11			@switch to fiq mode, registers 8-14 now reference the fiq registers
	ldrh	r4, [r14], #2			@load the next pos table entry, post increment
	movs	r5, r4, lsl #27			@check bits 5 and 4. 
	addmi	r10, r10, #2			@increase small x by 2 if bit 4 is set ||DIFFERENT||
	subcs	r11, r11, #1			@decrement small y if bit 5 is set
	movs	r5, r5, lsl #2			@check bits 3 and 2
	subcs	r10, r10, #1			@decrement small x if bit 3 is set ||DIFFERENT||
	submis	r9, r9, #1				@decrement big y if bit 2 is set
	bpl		ConeNewLineBack			@if we are done, exit the loop, otherwise, reset for new line
DrawConeBackEnd:
	msr		cpsr_c, #0x1f
	ldr		r13, R13Temp
	pop		{r4-r12, r14}
	bx		lr
	
	
.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global DrawCylinderWall
	.type DrawCylinderWall STT_FUNC
	
@register usage
@r0: texture pointer top start
@r1: texture width (2^n pixels) 
@r2: texture height (2^n pixels)
@r3: buffer address, top
@r4: texture pointer bottom start
@r5: buffer address, bottom
@r6: position table
@r7: horizontal counter
@r8: horizontal scale 
@r9: texture width (in pixels)
@r11: texture pointer partial
@r12: 
@r13: scratch
@r14: scratch

DrawCylinderWall:
	push	{r4-r12, r14}			@store the existing registers
	ldr		r12, [r13, #40]			@load the fourth argument, the horizontal length (in pixels) from the stack
	ldr		r14, [r13, #44]			@load the radius
	str		r13, R13Temp			@store the stack pointer
	ldr		r6, =pos_table			@load the position table address
	add		r5, r3, r14, lsl #8		@add double the radius to get the buffer pointer for the bottom
	mov		r14, #1					@\calculate the texture width in pixels
	mov		r9, r14, lsl r1			@/
	adr		r11, InverseTable		@\
	movs	r12, r12, lsl #1		@|
	rsbmi	r12, r12, #0			@|calculate the horizontal scale
	ldrh	r12, [r11, r12]			@|
	mov		r8, r12, lsl r1			@/
	add		r13, r1, r2				@\
	sub		r13, #1					@|calculate the starting texture pointer for the lower portion of the cylinder
	add		r4, r0, r14, lsl r13	@/
	mov		r11, #0
DrawOneLine:	
	ldr		r13, [r6], #4			@load the value at the position table
	mov		r14, r13, lsr #24		@isolate the angle value
	add		r11, r11, r14			@add the angle to the partial angle counter
	mov		r7, r9, lsl #16			@set the maximum horizontal counter
	and		r13, r13, #0xff			@isolate the xpos
	add		r3, r3, r13				@get the starting upper buffer pointer
	add		r5, r5, r13				@get the starting lower buffer pointer
	rsb		r13, r2, #10			@\
	mov		r14, r11, lsr r13		@|
	sub		r11, r11, r14, lsl r13	@|update the texture pointer for the next row
	add		r0, r0, r14, lsl r1		@|
	sub		r4, r4, r14, lsl r1		@/
DrawOnePixel:
	ldrb	r13, [r0, r7, lsr #16]	@load the upper pixel from the texture bitmap
	strb	r13, [r3], #1			@store the upper pixel in the buffer, post increment
	ldrb	r13, [r4, r7, lsr #16]	@load the lower pixel from the texture bitmap
	strb	r13, [r5], #1			@store the lower pixel in the buffer, post increment
	subs	r7, r7, r8				@subtract the horizontal scale to reach the next pixel
	bpl		DrawOnePixel			@continue until we run out of horizontal pixels in the texture
	add		r3, r3, #0x80			@\
	bic		r3, r3, #0x7f			@|update the buffer pointers
	sub		r5, r5, #0x80			@|
	bic		r5, r5, #0x7f			@/
	sub		r14, r5, r3				@\
	cmp		r14, #-80				@|repeat if the lower buffer address is greater than the upper buffer address
	bgt		DrawOneLine				@/
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

@r0: sin(pitch)
@r1: radius
@r2: center xPos
@r3: current yPos
@r4: pointer to position table
@r5: yPos Update Value
@r6: trig table 1
@r7: scratch
@r8: scratch
@r9: current angle
@r10: last angle

@angle is in top byte, xpos is in bottom byte

SetupPosTableCylinder:
	push	{r4-r10}				@store the existing registers
	ldr		r4, =pos_table
	mov		r3, #0x10000			@starting y pos is 1
	mov		r5, r1, lsl #1
	adr		r8, InverseTable
	ldrh	r5, [r8, r5]			@load the inverse of radius into the y update value
	ldr		r6, =trig_table_group_1
	mov		r10, #0xff000000		@first pixel has texture value of pi/2	
	str		r2, [r4], #4			@first pixel is always at same x value as center x	
	subs	r3, r3, r5				@add the update value to the ypos
setupPixel:
	mov		r8, r3, lsr #8
	ldr		r8, [r6, r8, lsl #2]	@load the sqrt(1-Y^2) and the arcsin(Y)
	and		r9, r8, #0xff			@isolate the angle
	rsb		r9, r9, r10, lsr #24	@find the difference between this angle and the last
	sub		r10, r10, r9, lsl #24	@store the new angle in r10
	and		r8, r8, #0xff00			@isolate the x pos
	mul		r7, r8, r1				@multiply by radius
	mul		r8, r7, r0				@multiply by sin(pitch)
	add		r8, r2, r8, lsr #24		@store the x pos in the bottom byte
	orr		r8, r8, r9, lsl #24		@store the delta angle at the upper byte
	str		r8, [r4], #4			@store the x value at the corrosponding y position in the table
	subs	r3, r3, r5				@add the update value to the ypos
	bhi		setupPixel
	mul		r8, r1, r0				@one special case for the center pixel
	add		r8, r2, r8, asr #8
	str		r8, [r4]				
	pop		{r4-r10}
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
	.hword	0x0555, 0x0539, 0x051E, 0x0505, 0x04EC, 0x04D4, 0x04BD, 0x04A7, 0x0492, 0x047D, 0x0469, 0x0456, 0x0444, 0x0432, 0x0421, 0x0410
	.hword	0x0400, 0x03F0, 0x03E0, 0x03D2, 0x03C3, 0x03B5, 0x03A8, 0x039B, 0x038E, 0x0381, 0x0375, 0x0369, 0x035E, 0x0353, 0x0348, 0x033D
	.hword	0x0333, 0x0329, 0x031F, 0x0315, 0x030C, 0x0303, 0x02FA, 0x02F1, 0x02E8, 0x02E0, 0x02D8, 0x02D0, 0x02C8, 0x02C0, 0x02B9, 0x02B1
	.hword	0x02AA, 0x02A3, 0x029C, 0x0295, 0x028F, 0x0288, 0x0282, 0x027C, 0x0276, 0x0270, 0x026A, 0x0264, 0x025E, 0x0259, 0x0253, 0x024E
	.hword	0x0249, 0x0243, 0x023E, 0x0239, 0x0234, 0x0230, 0x022B, 0x0226, 0x0222, 0x021D, 0x0219, 0x0214, 0x0210, 0x020C, 0x0208, 0x0204, 0x0200
	
	.align  2
pos_table:
	.word 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 
	
