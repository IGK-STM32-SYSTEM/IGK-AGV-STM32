#ifndef __FUNCTION_H
#define __FUNCTION_H	
#include "sys.h"

void CanSend_data(u8 dir);
long FloatTohex(float HEX);
float HexTofloat(long HEX);
u8 HexToChar(u8 bChar);
extern u8 CAN1_Sebuf[5];
extern void delay(u32 h,u32 m,u32 s,u32 ms);

#endif
