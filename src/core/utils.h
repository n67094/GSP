#ifndef CORE_UTILS_H
#define CORE_UTILS_H

#include "types.h"

/* Max @value is 999999, min @value is 0
 * return an unsigned integer to a 6digits binary coded decimal
 */
u32 UtilsBCD(u32 value);

/* @bcd is a binary coded decimal
 * @index is the index of the digit/nibble to get
 * return the xth digit of the @bcd
 */
u32 UtilsBCDDigit(u32 bcd, u32 index);

/* @bcd is a binary coded decimal
 * @str should be of length 7, 6 for digits and 1 for the EOF
 * @str will be a 6 digits ex: 000123
 */
void UtilsBCDtoA(u32 bcd, char *str);

#endif
