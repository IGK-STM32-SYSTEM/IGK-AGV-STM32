#ifndef __TLC5620_H
#define __TLC5620_H

#include "sys.h"

#define TLC5620RNG 1//��ѹ�ı���

#define DADATA PFout(1)	//TLC5620 DAת������DATAλ
#define DACLK  PFout(0)	//TLC5620 DAת������CLKλ
#define DALOAD PFout(2)	//TLC5620 DAת������LOADλ
#define DALDAC PFout(3)	//TLC5620 DAת������LDACλ

void TLC5620_Init(void);//��ʼ��		 	
void TLC5620_OUT(u8 CHn,double date);//
void TLC5620_OUTnum(u8 CHn,u8 date);
#endif
