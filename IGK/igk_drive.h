#ifndef __IGK_DRIVE_H
#define __IGK_DRIVE_H
#include "sys.h"
//�����ת
void MotorZhengZhuan(u8 Num,u16 speed);
//�����ת
void MotorFanZhuan(u8 Num,u16 speed);
//��ʼ��
void DriverInitial(void);
//ǰ��,�������ٶ�
void DriverQinJinSpeed(u16 s1,u16 s2);
//����,�������ٶ�
void DriverHouTuiSpeed(u16 s1,u16 s2);
//����
void DriverZuoYi(void);
//����
void DriverYouYi(void);
//����
void DriverZuoXuan(u16 speed);
//����
void DriverYouXuan(u16 speed);
//ֹͣ
void DriverTingZhi(void);



















#endif
