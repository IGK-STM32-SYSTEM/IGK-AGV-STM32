#ifndef  _CALCULATE_H_
#define  _CALCULATE_H_
#include "steppercontrol.h"

//�������
void CalculateReverse(Point point);
Angle calculateReverse(Point point);

//�������
//angle:�Ƕ�ֵ�������ڲ����Զ�תΪ����
Point CalculatePositive(Angle angle);

//��ȡʵ�ʽǶ�
//����:�㷨�Ƕ�
//���:ʵ�ʽǶ�
//angle.beta ��Ӧ��1
//angle.alpha ��Ӧ��2
//angle.gamma ��Ӧ��3
Angle GetRealAngle(Angle angle);
//��ȡ�㷨�Ƕ�
//����:ʵ�ʽǶ�
//���:�㷨�Ƕ�
Angle GetCalculateAngle(Angle angle);


#endif


