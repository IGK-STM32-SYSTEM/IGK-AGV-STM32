#include "igk_drive.h"
#include "motec.h"
//��ʼ��
void DriverInitial()
{
}

//�����ת
void MotorZhengZhuan(u8 Num,u16 speed)
{
//	u8 buf[3]={0};
//	buf[0] = Num;//������
//	buf[1] = 0;//����
//	buf[2] = speed;//�ٶ�
//	Igk_CAN1_Send(2,buf,3);
    Motor_Zzhuan(Num,speed*15);
}
//�����ת
void MotorFanZhuan(u8 Num,u16 speed)
{
//	u8 buf[3]={0};
//	buf[0] = Num;//������
//	buf[1] = 1;//����
//	buf[2] = speed;//�ٶ�
//	Igk_CAN1_Send(2,buf,3);
    Motor_Fzhuan(Num,speed*15);
}

//ǰ��,�������ٶ�
void DriverQinJinSpeed(u16 s1,u16 s2)
{
	MotorZhengZhuan(1,s1);
	MotorFanZhuan(2,s2);
}
//����,�������ٶ�
void DriverHouTuiSpeed(u16 s1,u16 s2)
{
	MotorFanZhuan(1,s1);
	MotorZhengZhuan(2,s2);
}
//����
void DriverZuoYi()
{
	
}
//����
void DriverYouYi()
{
	
}
//����
void DriverZuoXuan(u16 speed)
{
	MotorFanZhuan(1,speed);
	MotorFanZhuan(2,speed);
}
//����
void DriverYouXuan(u16 speed)
{
	MotorZhengZhuan(1,speed);
	MotorZhengZhuan(2,speed);
}
//ֹͣ
void DriverTingZhi()
{
	MotorZhengZhuan(1,0);
	MotorZhengZhuan(2,0);
}








