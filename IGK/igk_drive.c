#include "igk_drive.h"
#include "motec.h"
//初始化
void DriverInitial()
{
}

//电机正转
void MotorZhengZhuan(u8 Num,u16 speed)
{
//	u8 buf[3]={0};
//	buf[0] = Num;//电机编号
//	buf[1] = 0;//方向
//	buf[2] = speed;//速度
//	Igk_CAN1_Send(2,buf,3);
    Motor_Zzhuan(Num,speed*15);
}
//电机反转
void MotorFanZhuan(u8 Num,u16 speed)
{
//	u8 buf[3]={0};
//	buf[0] = Num;//电机编号
//	buf[1] = 1;//方向
//	buf[2] = speed;//速度
//	Igk_CAN1_Send(2,buf,3);
    Motor_Fzhuan(Num,speed*15);
}

//前进,带单轮速度
void DriverQinJinSpeed(u16 s1,u16 s2)
{
	MotorZhengZhuan(1,s1);
	MotorFanZhuan(2,s2);
}
//后退,带单轮速度
void DriverHouTuiSpeed(u16 s1,u16 s2)
{
	MotorFanZhuan(1,s1);
	MotorZhengZhuan(2,s2);
}
//左移
void DriverZuoYi()
{
	
}
//右移
void DriverYouYi()
{
	
}
//左旋
void DriverZuoXuan(u16 speed)
{
	MotorFanZhuan(1,speed);
	MotorFanZhuan(2,speed);
}
//右旋
void DriverYouXuan(u16 speed)
{
	MotorZhengZhuan(1,speed);
	MotorZhengZhuan(2,speed);
}
//停止
void DriverTingZhi()
{
	MotorZhengZhuan(1,0);
	MotorZhengZhuan(2,0);
}








