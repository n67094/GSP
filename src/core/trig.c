#include "trig.h"
#include "types.h"

//All angular inputs and outputs are in units of pi/512 radians.
//All linear inputs and outputs are in units of 1/256.

s32 TrigGetSin(s32 theta){
	u32 negative = 0;
	s32 value;
	theta = theta & 0x3ff; //shift any Theta into a range of 0-1023 (1 complete sin wave)
	if(theta > 0x200){ //if theta is in the negative portion of the sin wave
		negative = 1;
		theta -= 0x200;
	}
	if(theta > 0x100){ //if theta is in the second or fourth quarter of the sin wave.
		theta = 0x200 - theta;
	}
	if(theta < 0x100){
		value = trig_table_group_2[theta] & 0xff;
	}
	else{
		value = 0x100;
	}
	if (negative){
		value = -value;
	}
	return value;
}

s32 TrigGetCos(s32 theta){
	s32 value;
	value = TrigGetSin(theta + 0x100);
	return value;
}

s32 TrigGetArcSin(s32 length){ //length must be between -256 (-1) and 256 (1)
	s32 theta;
	u32 negative = 0;
	if (length < 0){
		negative = 1;
		length = -length;
	}
	if (length != 0x100){
		theta = trig_table_group_1[length] & 0xff;
	}
	else {
		theta = 0x100;
	}
	if(negative){
		theta = -theta;
	}
	return theta;
}

s32 TrigGetArcCos(s32 length){ //length must be between -0x100 (-1) and 0x100 (1)
	s32 theta;
	theta = 0x100 - TrigGetArcSin(length);
	return theta;
}

s32 TrigGetPythSqrt(s32 length){ //length must be between -0x100 (-1) and 0x100 (1)
	u32 value;
	if (length < 0){
		length = -length;
	}
	if (length != 0x100){
		value = (trig_table_group_1[length] & 0xff00) >> 8;
	}
	else{
		value = 0;
	}
	return value;
}

s32 TrigGetInvPythSqrt(s32 length){ //length must be between -0xff and 0xff. 0x100 is infinity, but returns 0xffff
	u32 value;
	if (length < 0){
		length = -length;
	}
	if (length != 0x100){
		value = (trig_table_group_1[length] & 0xffff0000) >> 16;
	}
	else{
		value = 0xffff;
	}
	return value;
}

s32 TrigGetCsc(s32 theta){ //length should not be a multiple of pi (512). csc(pi) is undefined, but this function returns 0xffff
	u32 negative = 0;
	s32 value;
	theta = theta & 0x3ff; //shift any Theta into a range of 0-1023 (1 complete csc wave)
	if(theta > 0x200){ //if theta is in the negative portion of the csc wave
		negative = 1;
		theta -= 0x200;
	}
	if(theta > 0x100){ //if theta is in the second or fourth quarter of the csc wave.
		theta = 0x200 - theta;
	}
	if(theta < 0x100){
		value = (trig_table_group_2[theta] & 0xffff0000) >> 16;
	}
	else{
		value = 0x100;
	}
	if (negative){
		value = -value;
	}
	return value;
}

s32 TrigGetSec(s32 theta){ //length should not be a multiple of pi (512) - pi/2. csc(pi/2) is undefined, but this function returns 0xffff
	s32 value;
	value = TrigGetCsc(theta - 0x100);
	return value;
}

s32 TrigGetTan(s32 theta){ //theta should not be a multiple of pi (256) - pi/2. Answer is undefined, but this function returns 0xffff
	s32 value;
	value = (TrigGetSec(theta) * TrigGetSin(theta)) >> 8;
	return value;
}

s32 TrigGetCot(s32 theta){ //theta should not be a multiple of pi (256). Answer is undefined, but this function returns 0xffff
	s32 value;
	value = (TrigGetCsc(theta) * TrigGetCos(theta)) >> 8;
	return value;
}

cu8 sqrtTable[1025] = {
	0x0, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 0x5, 
	0x5, 0x5, 0x5, 0x5, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x6, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 0x7, 
	0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x8, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 0x9, 
	0x9, 0x9, 0x9, 0x9, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 
	0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xB, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 
	0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xC, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 0xD, 
	0xD, 0xD, 0xD, 0xD, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 0xE, 
	0xE, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 0xF, 
	0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
	0x10,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,
	0x11,0x11,0x11,0x11,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,
	0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x12,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,
	0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x13,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,
	0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x15,0x15,0x15,0x15,0x15,0x15,0x15,
	0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,0x15,
	0x15,0x15,0x15,0x15,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,
	0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,
	0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,0x17,
	0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,
	0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,
	0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,0x19,
	0x19,0x19,0x19,0x19,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,
	0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1A,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,
	0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,
	0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1B,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,
	0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,
	0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,
	0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,0x1D,
	0x1D,0x1D,0x1D,0x1D,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,
	0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,
	0x1E,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,
	0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,
	0x20
};

s32 TrigGetPythHyp(s32 legs){ //only valid for very small hypotenuses, up to 32
	if(legs < 0){
		legs = -legs;
	}
	return sqrtTable[legs];
}

s32 TrigGetInverse(s32 num){
	u32 negative;
	if(num < 0){
		negative = 1;
		num = -num;
	}
	num = InverseTable[num];
	if(negative){
		num = -num;
	}
	return num;
}	