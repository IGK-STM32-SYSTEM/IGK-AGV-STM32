#ifndef __DELAY_H
#define __DELAY_H 	
#include "sys.h"

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void osdelay(u16 hh,u16 mm,u16 ss,u16 ms);
void osdelay_s(u16 ns);
void osdelay_ms(u16 ns);
#endif





























