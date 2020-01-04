#ifndef __TLC5620_H
#define __TLC5620_H

#include "sys.h"

#define TLC5620RNG 1//电压的倍数

#define DADATA PFout(1)	//TLC5620 DA转换器的DATA位
#define DACLK  PFout(0)	//TLC5620 DA转换器的CLK位
#define DALOAD PFout(2)	//TLC5620 DA转换器的LOAD位
#define DALDAC PFout(3)	//TLC5620 DA转换器的LDAC位

void TLC5620_Init(void);//初始化		 	
void TLC5620_OUT(u8 CHn,double date);//
void TLC5620_OUTnum(u8 CHn,u8 date);
#endif
