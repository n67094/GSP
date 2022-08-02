@--------------------------------------------------------------
@Function:
	@render_sphere (
	
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
	@r2: VRAM entry Left temp
	@r3: X coordinate reletive to center (X)
	@r4: Y coordinate reletive to center (Y)
	@r5: Scratch 1
	@r6: Scratch 2
	@r7: Scratch 3
	@r8: Scratch 4
	@r9: Pointer to table_group_1
	@r10: Pointer to table_group_2
	@r11: VRAM entry 1 Right
	@r12: VRAM entry 2 Right
	@r13: VRAM entry 3 Right
	@r14: VRAM entry 4 Right
	@r15: program counter
	@rfiq_10: VRAM Pointer Left
	@rfiq_11: VRAM entry 1 Left
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
	.global render_sphere
	.global table_group_1
	.global table_group_2
	.global sphere_rander_stack_pointer
	.type render_sphere STT_FUNC
	
sphere_render_r13:
	@this is a temporary storage place for the stack pointer, as r13 will be used for other purposes by this function
	.word 0x0
	
render_sphere:
	@first comes some housekeeping to handle the function arguments
	@store all the existing registers
	push	{r4-r12, r14}
	str		r13, sphere_render_r13
	mov		r5, r2 @put the pitch into r5 temporarilly. This will be replaced by self-modifying code |*****|
	
	@add the spin to the gfx Ptr, while moving it into a range of 0-1023
	@the shift amount will need to change depending on gfxSize|*****|
	mov		r3, r3, lsl #22
	add		r0, r3, lsr #22
	add		r0, r0, #0x80000
	@setup the lut pointers. Change these to adr instructions once the rest of the function is done |*****|
	ldr		r9, =table_group_1
	ldr		r10, =table_group_2
	@set the starting Y position. It starts at radius-1 and will decrement towards 0
	mov		r4, #63 @this number will change depending on scale|*****|
map_row:
	@load the starting X position. It starts at sqrt(1-Y^2) pixels to the right, and decrements towards 0
	add		r6, r9, #1
	ldrb	r3, [r6, r4, lsl #4]
	mov		r3, r3, lsr #2
	sub		r3, r3, #1
	@calculate the correct VRAM pointer (in tile format) based on the X and Y coordinates
	mov		r1, #0x6000000 @This number will change depending on VRAMPtr |*****|
	@first get the correct row of tiles
	rsb		r6, r4, #64
	mov		r7, r6, lsr #3
	add		r1, r1, r7, lsl #10 
	@then get the correct column of tiles
	add		r7, r3, #64
	mov		r8, r7, lsr #3
	add		r1, r1, r8, lsl #6
	@then get the correct row within a tile
	and		r6, r6, #7
	add		r1, r1, r6, lsl #3
	@lastly get the correct 4 pixels within a tile row
	and		r7, r7, #4
	add		r1, r1, r7
	add		r3, r3, #1
map_pixel:
	@This is where the X position, Y position, and Pitch (P) are plugged into the function to get the coordinates of the texture.
	@The first part of the formula is this: E = sqrt(1 - X^2) * sin(P + arcsin(y / (sqrt(1 - X^2))))
	@First step is to find the sqrt(1 - X^2) and its inverse.
	ldr		r6, [r9, r3, lsl #4]
	and		r7, r6, #0xff00
	mov		r6, r6, lsr #16
	@then multiply by Y and take the arcsin of that result
	muls	r8, r6, r4
	mov		r6, r8, lsr #6
	ldrb	r6, [r9, r6, lsl #2]
	@then add the pitch, and take the sin of that result. Hold onto the first result for later use
	adds	r8, r6, r5 @This will need to be replaced by a self-modifying immediate for Pitch |*****|
	cmp		r8, #0x100
	rsbgt	r8, r8, #0x200
	ldrb	r8, [r10, r8, lsl #2]
	@multiply by sqrt(1 - X^2) which is still in register 7 from before
	mul		r8, r7, r8
	mov		r8, r8, asr #16
	@The next step is to convert this intermediate value (E) into the X and Y coordinates on the texture
	@The Y coordinate is arcsin(E), the X coordinate is arcsin(X / (sqrt(1 - E^2)))
	ldr		r7, [r9, r8, lsl #2]
	and 	r8, r7, #0xff
	mov		r7, r7, asr #16
	mul		r6, r7, r3
	mov		r6, r6, asr #6
	ldrb	r6, [r9, r6, lsl #2]
	@now we grab the pixel at those X and Y coordinates, and store them in a register
	sub		r7, r6, r8, lsl #11 @This will have to change depending on gfxSize |*****|
	ldrb	r7, [r0, r7]
	orr		r11, r11, r7
	@If X is a multiple of 4, then we need to swap out the storage register for an empty one.
	subs	r3, r3, #1
	tst		r3, #3
	beq		storage_full
	mov		r11, r11, lsl #8 @this will have to change depending on bpp |*****|
next_pixel:
	bne		map_pixel
next_row:
	subs	r4, r4, #1
	bne		map_row
	ldr		r13, sphere_render_r13
	pop		{r4-r12, r14}
	bx lr
	
	storage_full:
	str		r11, [r1], #-4
	mov		r11, #0
	tst		r3, #7
	bne		next_pixel
	cmp		r3, #0
	beq		next_row
	sub		r1, r1, #56
	b		next_pixel
	
table_group_1:
	@this group consists of the 1/(sqrt(1-X^2)) table, the sqrt(1-X^2) table, and the arcsin(X) table, in that order.
	.word 0x0100FF00, 0x0100FF00, 0x0100FF01, 0x0100FF01, 0x0100FF02, 0x0100FF03, 0x0100FF03, 0x0100FF04
	.word 0x0100FF05, 0x0100FF05, 0x0100FF06, 0x0100FF07, 0x0100FF07, 0x0100FF08, 0x0100FF08, 0x0100FF09
	.word 0x0100FF0A, 0x0100FF0A, 0x0100FF0B, 0x0100FF0C, 0x0100FF0C, 0x0100FF0D, 0x0100FF0E, 0x0101FE0E 
	.word 0x0101FE0F, 0x0101FE0F, 0x0101FE10, 0x0101FE11, 0x0101FE11, 0x0101FE12, 0x0101FE13, 0x0101FE13 
	.word 0x0102FD14, 0x0102FD15, 0x0102FD15, 0x0102FD16, 0x0102FD16, 0x0102FD17, 0x0102FD18, 0x0103FD18 
	.word 0x0103FC19, 0x0103FC1A, 0x0103FC1A, 0x0103FC1B, 0x0103FC1C, 0x0104FC1C, 0x0104FB1D, 0x0104FB1E 
	.word 0x0104FB1E, 0x0104FB1F, 0x0105FB20, 0x0105FA20, 0x0105FA21, 0x0105FA21, 0x0105FA22, 0x0106FA23 
	.word 0x0106F923, 0x0106F924, 0x0106F925, 0x0107F925, 0x0107F826, 0x0107F827, 0x0107F827, 0x0108F828 
	.word 0x0108F729, 0x0108F729, 0x0108F72A, 0x0109F72B, 0x0109F62B, 0x0109F62C, 0x010AF62D, 0x010AF52D 
	.word 0x010AF52E, 0x010BF52F, 0x010BF52F, 0x010BF430, 0x010CF431, 0x010CF431, 0x010CF332, 0x010DF333 
	.word 0x010DF333, 0x010DF234, 0x010EF235, 0x010EF235, 0x010FF136, 0x010FF137, 0x010FF137, 0x0110F038 
	.word 0x0110F039, 0x0111F039, 0x0111EF3A, 0x0111EF3B, 0x0112EE3B, 0x0112EE3C, 0x0113EE3D, 0x0113ED3D 
	.word 0x0114ED3E, 0x0114EC3F, 0x0115EC40, 0x0115EC40, 0x0116EB41, 0x0116EB42, 0x0117EA42, 0x0117EA43 
	.word 0x0118E944, 0x0118E944, 0x0119E945, 0x0119E846, 0x011AE846, 0x011AE747, 0x011BE748, 0x011CE649 
	.word 0x011CE649, 0x011DE54A, 0x011DE54B, 0x011EE44B, 0x011FE44C, 0x011FE34D, 0x0120E34E, 0x0121E24E 
	.word 0x0121E24F, 0x0122E150, 0x0123E150, 0x0123E051, 0x0124DF52, 0x0125DF53, 0x0126DE53, 0x0126DE54 
	.word 0x0127DD55, 0x0128DD56, 0x0129DC56, 0x0129DB57, 0x012ADB58, 0x012BDA59, 0x012CDA59, 0x012DD95A 
	.word 0x012ED85B, 0x012FD85C, 0x012FD75C, 0x0130D65D, 0x0131D65E, 0x0132D55F, 0x0133D55F, 0x0134D460 
	.word 0x0135D361, 0x0136D262, 0x0137D262, 0x0138D163, 0x0139D064, 0x013AD065, 0x013BCF66, 0x013DCE66 
	.word 0x013ECD67, 0x013FCD68, 0x0140CC69, 0x0141CB69, 0x0142CA6A, 0x0144CA6B, 0x0145C96C, 0x0146C86D
	.word 0x0147C76E, 0x0149C76E, 0x014AC66F, 0x014BC570, 0x014DC471, 0x014EC372, 0x0150C272, 0x0151C273
	.word 0x0153C174, 0x0154C075, 0x0156BF76, 0x0157BE77, 0x0159BD78, 0x015BBC78, 0x015DBB79, 0x015EBA7A
	.word 0x0160B97B, 0x0162B87C, 0x0164B77D, 0x0166B77E, 0x0168B67F, 0x016AB57F, 0x016CB480, 0x016EB381
	.word 0x0170B182, 0x0172B083, 0x0174AF84, 0x0176AE85, 0x0179AD86, 0x017BAC87, 0x017DAB88, 0x0180AA89
	.word 0x0183A98A, 0x0185A88B, 0x0188A78C, 0x018BA58D, 0x018DA48E, 0x0190A38F, 0x0193A290, 0x0196A191
	.word 0x019A9F92, 0x019D9E93, 0x01A09D94, 0x01A49B95, 0x01A79A96, 0x01AB9997, 0x01AF9798, 0x01B39699
	.word 0x01B7959A, 0x01BB939B, 0x01BF929C, 0x01C4909D, 0x01C88F9F, 0x01CD8EA0, 0x01D28CA1, 0x01D78AA2
	.word 0x01DC89A3, 0x01E287A4, 0x01E886A6, 0x01EE84A7, 0x01F482A8, 0x01FB81A9, 0x02027FAB, 0x02097DAC
	.word 0x02107BAD, 0x02187AAE, 0x022078B0, 0x022976B1, 0x023274B3, 0x023C72B4, 0x024770B5, 0x02516EB7
	.word 0x025D6CB8, 0x02696ABA, 0x027767BB, 0x028565BD, 0x029463BF, 0x02A560C0, 0x02B65EC2, 0x02CA5BC4
	.word 0x02DF59C6, 0x02F756C7, 0x031053C9, 0x032D50CB, 0x034E4DCD, 0x03724AD0, 0x039C46D2, 0x03CE43D4
	.word 0x04083FD7, 0x044E3BD9, 0x04A537DC, 0x051532DF, 0x05AD2DE3, 0x068D27E7, 0x08041FEB, 0x0B5316F1
	
table_group_2:
	@this group consists of the csc(X) table, the cos(X) table, and the sin(X) table, in that order.
	.word 0xFFFFFF00, 0xA2F9FF01, 0x517DFF03, 0x3653FF04, 0x28BFFF06, 0x2099FF07, 0x1B2BFF09, 0x174AFF0A
	.word 0x1461FF0C, 0x121EFF0E, 0x104EFF0F, 0x0ED3FF11, 0x0D97FF12, 0x0C8CFF14, 0x0BA7FF15, 0x0AE1FE17
	.word 0x0A33FE19, 0x099AFE1A, 0x0912FE1C, 0x0898FE1D, 0x082BFE1F, 0x07C8FD20, 0x076EFD22, 0x071CFD24
	.word 0x06D0FD25, 0x068BFC27, 0x064BFC28, 0x0610FC2A, 0x05D9FC2B, 0x05A6FB2D, 0x0576FB2E, 0x054AFB30
	.word 0x0520FB31, 0x04F8FA33, 0x04D4FA35, 0x04B1FA36, 0x0490F938, 0x0471F939, 0x0453F93B, 0x0438F83C
	.word 0x041DF83E, 0x0404F73F, 0x03ECF741, 0x03D5F742, 0x03BFF644, 0x03ABF645, 0x0397F547, 0x0384F548
	.word 0x0371F44A, 0x0360F44B, 0x034FF44D, 0x033FF34E, 0x0330F350, 0x0321F251, 0x0312F253, 0x0305F154
	.word 0x02F7F156, 0x02EBF057, 0x02DEEF59, 0x02D2EF5A, 0x02C7EE5C, 0x02BCEE5D, 0x02B1ED5F, 0x02A7ED60
	.word 0x029CEC61, 0x0293EB63, 0x0289EB64, 0x0280EA66, 0x0277EA67, 0x026FE969, 0x0266E86A, 0x025EE86C
	.word 0x0256E76D, 0x024FE66E, 0x0247E670, 0x0240E571, 0x0239E473, 0x0232E374, 0x022BE375, 0x0225E277
	.word 0x021FE178, 0x0218E17A, 0x0212E07B, 0x020DDF7C, 0x0207DE7E, 0x0201DD7F, 0x01FCDD80, 0x01F7DC82
	.word 0x01F1DB83, 0x01ECDA84, 0x01E8D986, 0x01E3D987, 0x01DED888, 0x01D9D78A, 0x01D5D68B, 0x01D1D58C
	.word 0x01CCD48E, 0x01C8D38F, 0x01C4D390, 0x01C0D292, 0x01BCD193, 0x01B8D094, 0x01B5CF95, 0x01B1CE97
	.word 0x01ADCD98, 0x01AACC99, 0x01A6CB9B, 0x01A3CA9C, 0x01A0C99D, 0x019CC89E, 0x0199C79F, 0x0196C6A1
	.word 0x0193C5A2, 0x0190C4A3, 0x018DC3A4, 0x018AC2A6, 0x0187C1A7, 0x0185C0A8, 0x0182BFA9, 0x017FBEAA
	.word 0x017DBDAB, 0x017ABCAD, 0x0178BBAE, 0x0175BAAF, 0x0173B9B0, 0x0170B8B1, 0x016EB7B2, 0x016CB6B3
	.word 0x016AB5B5, 0x0167B3B6, 0x0165B2B7, 0x0163B1B8, 0x0161B0B9, 0x015FAFBA, 0x015DAEBB, 0x015BADBC
	.word 0x0159ABBD, 0x0157AABE, 0x0155A9BF, 0x0153A8C0, 0x0152A7C1, 0x0150A6C2, 0x014EA4C3, 0x014CA3C4
	.word 0x014BA2C5, 0x0149A1C6, 0x01479FC7, 0x01469EC8, 0x01449DC9, 0x01439CCA, 0x01419BCB, 0x014099CC
	.word 0x013E98CD, 0x013D97CE, 0x013B95CF, 0x013A94D0, 0x013993D1, 0x013792D2, 0x013690D3, 0x01358FD3
	.word 0x01338ED4, 0x01328CD5, 0x01318BD6, 0x01308AD7, 0x012F88D8, 0x012D87D9, 0x012C86D9, 0x012B84DA
	.word 0x012A83DB, 0x012982DC, 0x012880DD, 0x01277FDD, 0x01267EDE, 0x01257CDF, 0x01247BE0, 0x01237AE1
	.word 0x012278E1, 0x012177E2, 0x012075E3, 0x011F74E3, 0x011E73E4, 0x011D71E5, 0x011C70E6, 0x011C6EE6
	.word 0x011B6DE7, 0x011A6CE8, 0x01196AE8, 0x011869E9, 0x011867EA, 0x011766EA, 0x011664EB, 0x011563EB
	.word 0x011561EC, 0x011460ED, 0x01135FED, 0x01135DEE, 0x01125CEE, 0x01115AEF, 0x011159EF, 0x011057F0
	.word 0x010F56F1, 0x010F54F1, 0x010E53F2, 0x010E51F2, 0x010D50F3, 0x010D4EF3, 0x010C4DF4, 0x010C4BF4
	.word 0x010B4AF4, 0x010B48F5, 0x010A47F5, 0x010A45F6, 0x010944F6, 0x010942F7, 0x010841F7, 0x01083FF7
	.word 0x01073EF8, 0x01073CF8, 0x01073BF9, 0x010639F9, 0x010638F9, 0x010636FA, 0x010535FA, 0x010533FA
	.word 0x010531FB, 0x010430FB, 0x01042EFB, 0x01042DFB, 0x01032BFC, 0x01032AFC, 0x010328FC, 0x010327FC
	.word 0x010225FD, 0x010224FD, 0x010222FD, 0x010220FD, 0x01011FFE, 0x01011DFE, 0x01011CFE, 0x01011AFE
	.word 0x010119FE, 0x010117FE, 0x010015FF, 0x010014FF, 0x010012FF, 0x010011FF, 0x01000FFF, 0x01000EFF
	.word 0x01000CFF, 0x01000AFF, 0x010009FF, 0x010007FF, 0x010006FF, 0x010004FF, 0x010003FF, 0x010001FF, 0x010000FF