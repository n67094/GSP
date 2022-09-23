#include <seven/svc/math.h>

#include "../debug/log.h"

#include "types.h"
#include "utils.h"

#define BCD_1ST 0x0000000F
#define BCD_2ND 0x000000F0
#define BCD_3RD 0x00000F00
#define BCD_4TH 0x0000F000
#define BCD_5TH 0x000F0000
#define BCD_6TH 0x00F00000

u32 UtilsBCD(u32 value) {
  if(value <= 0)
    return 0;

  if(value >= 999999)
    return 999999;

  u32 result = 0;

  int i;
  for(i = 20; i >= 0; --i) { // 20 is the number of bit occupied by 999999
    if((result & BCD_1ST) >= 5)
      result += 3;
    if(((result & BCD_2ND) >> 4) >= 5)
      result += (3 << 4);
    if(((result & BCD_3RD) >> 8) >= 5)
      result += (3 << 8);
    if(((result & BCD_4TH) >> 12) >= 5)
      result += (3 << 12);
    if(((result & BCD_5TH) >> 16) >= 5)
      result += (3 << 16);
    if(((result & BCD_6TH) >> 20) >= 5)
      result += (3 << 20);

    result = (result << 1) | ((value >> i) & 1);
  }

  return result;
}

u32 UtilsBCDDigit(u32 bcd, u32 index) {
  return (bcd >> (index << 2)) & BCD_1ST;
}

void UtilsBCDtoA(u32 bcd, char *str) {
  int i;
  for(i = 0; i < 6; ++i) {
    str[5 - i] = UtilsBCDDigit(bcd, i) + 48; // 48 = ascii code of 0
  }

  str[i] = '\0';
}
