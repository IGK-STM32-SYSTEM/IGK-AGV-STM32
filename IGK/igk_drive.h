#ifndef __IGK_DRIVE_H
#define __IGK_DRIVE_H
#include "sys.h"
//电机正转
void MotorZhengZhuan(u8 Num,u16 speed);
//电机反转
void MotorFanZhuan(u8 Num,u16 speed);
//初始化
void DriverInitial(void);
//前进,带单轮速度
void DriverQinJinSpeed(u16 s1,u16 s2);
//后退,带单轮速度
void DriverHouTuiSpeed(u16 s1,u16 s2);
//左移
void DriverZuoYi(void);
//右移
void DriverYouYi(void);
//左旋
void DriverZuoXuan(u16 speed);
//右旋
void DriverYouXuan(u16 speed);
//停止
void DriverTingZhi(void);



















#endif
