#ifndef __LED_H
#define __LED_H
#include "sys.h"
//端口定义
#define BEEP	PBout(7)

#define LED1 PEout(3)
#define LED2 PEout(4)
#define WIFI_RESET PEout(5)

#define OUT13 PGout(5)
#define OUT12 PGout(4)
#define OUT11 PGout(6)
#define OUT10 PGout(7)
#define OUT9 PGout(9)
#define OUT8 PGout(10)
#define OUT7 PGout(11)
#define OUT6 PGout(12)
#define OUT5 PGout(13)
#define OUT4 PGout(14)
#define OUT3 PGout(15)
#define OUT2 PBout(3)
#define OUT1 PBout(4)
#define OUT0 PEout(0)

#define IN0 PFin(12)
#define IN1 PFin(13)
#define IN2 PFin(14)
#define IN3 PFin(15)

#define IN4 PGin(0)
#define IN5 PGin(1)

#define IN6 !PEin(7)  //启动
#define IN7 !PEin(8)  //停止
#define IN8 PEin(9)
#define IN9 PEin(10)
#define IN10 PEin(11)
#define IN11 PEin(12)
#define IN12 PEin(13)
#define IN13 PEin(14)
#define IN14 PEin(15)

void LED_Init(void);//初始化	


#endif
