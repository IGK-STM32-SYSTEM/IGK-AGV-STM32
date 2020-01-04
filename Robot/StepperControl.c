#include "StepperControl.h"
//�������λ״̬,true:�ѻ�ԭ�㣬false:δ��ԭ��
u8 Axis1HomeFlag = 0;
u8 Axis2HomeFlag = 0;
u8 Axis3HomeFlag = 0;
//�����ǰ���� ��0:����1:����
u8 AxisDir1 = 0;
u8 AxisDir2 = 0;
u8 AxisDir3 = 0;
//�����ǰ����ֵ
long int AxisNowSteps1 = 0;
long int AxisNowSteps2 = 0;
long int AxisNowSteps3 = 0;

//��ǰ����
Point AxisPoint = {175,0,215};
//Ŀ������
Point TargetAxisPoint = {175,0,215};

//��ǰ�ٶ�
float AxisNowSpeed = 30; 
float Axis1Speed = 1; 
float Axis2Speed = 1; 
float Axis3Speed = 1; 
//���Ŀ�경��ֵ
long int AxisTargetSteps1 = 0;
long int AxisTargetSteps2 = 0;
long int AxisTargetSteps3 = 0;

//����ʱ������ʱ0-100��Ӧ10us-1ms����������
long int TimerCouter1 = 0;
long int TimerCouter2 = 0;
long int TimerCouter3 = 0;
//�������Ƿ���ת��0:ֹͣ,1:���С�
u8 Axis1Run = 0;
u8 Axis2Run = 0;
u8 Axis3Run = 0;

//ĳ������ĳ�������������˶�
void RobotRun(int Axis, u8 dir)
{
	switch(Axis)
	{
		case 1:
			//����Ŀ�경��
			if(dir)
				AxisTargetSteps1 = AxisMaxStep1;
			else
				AxisTargetSteps1 = AxisMinStep1;
			//�������˶�
			Axis1Run = 1;
			break;
		case 2:
			//����Ŀ�경��
			if(dir)
				AxisTargetSteps2 = AxisMaxStep2;
			else
				AxisTargetSteps2 = AxisMinStep2;
			//�������˶�
			Axis2Run = 1;
			break;
		case 3:
			//����Ŀ�경��
			if(dir)
				AxisTargetSteps3 = AxisMaxStep3;
			else
				AxisTargetSteps3 = AxisMinStep3;
			//�������˶�
			Axis3Run = 1;
			break;
		default:
			break;
	}
}
//ĳ������ĳ�������߹̶�����,����˶�
void RobotRunSteps(int Axis, u8 dir, long int steps)
{
	switch(Axis)
	{
		case 1:
			//����Ŀ�경��
			if(dir)
				AxisTargetSteps1 += steps;
			else
				AxisTargetSteps1 -= steps;
			//�������˶�
			Axis1Run = 1;
			break;
		case 2:
			//����Ŀ�경��
			if(dir)
				AxisTargetSteps2 += steps;
			else
				AxisTargetSteps2 -= steps;
			//�������˶�
			Axis2Run = 1;
			break;
		case 3:
			//����Ŀ�경��
			if(dir)
				AxisTargetSteps3 += steps;
			else
				AxisTargetSteps3 -= steps;
			//�������˶�
			Axis3Run = 1;
			break;
		default:
			break;
	}
}
//ĳ������ĳ�������߹̶��Ƕȣ�����˶�
void RobotRunAngle(int Axis, u8 dir, float Angle)
{
	//���ݽǶȼ��㲽��
	long int steps = AngleToSteps(Axis,Angle);
	//��ĳ�������߹̶�����
	RobotRunSteps(Axis,dir,steps);
}

//�������ߵ�ָ������,�����˶�
void RobotToSteps(int Axis, long int steps)
{
	switch(Axis)
	{
		case 1:
			//����Ŀ�경��
			AxisTargetSteps1 = steps;
			//�������˶�
			Axis1Run = 1;
			break;
		case 2:
			//����Ŀ�경��
			AxisTargetSteps2 = steps;
			//�������˶�
			Axis2Run = 1;
			break;
		case 3:
			//����Ŀ�경��
			AxisTargetSteps3 = steps;
			//�������˶�
			Axis3Run = 1;
			break;
		default:
			break;
	}
}

//�������ߵ�ָ���Ƕ�,�����˶�
void RobotToAngle(int Axis, float Angle)
{
	//���ݽǶȼ��㲽��
	long int steps = AngleToSteps(Axis,Angle);
	//�������ߵ�ָ������,�����˶�
	RobotToSteps(Axis,steps);
}

//������ͬʱ�ߵ�ָ������,�����˶�
void RobotAllToSteps(long int steps1, long int steps2, long int steps3)
{
	//�ж���û�г�����Χ����
	if(steps1<AxisMaxStep1||steps1>AxisMaxStep1)
		return;
	if(steps2<AxisMinStep2||steps2>AxisMaxStep2)
		return;
	if(steps3<AxisMinStep3||steps3>AxisMaxStep3)
		return;
	RobotToSteps(1,steps1);
	RobotToSteps(2,steps2);
	RobotToSteps(3,steps3);
}

//������ͬʱ�ߵ�ָ���Ƕ�,�����˶�
void RobotAllToAngle(float Angle1,float Angle2,float Angle3)
{
	//�ж���û�г�����Χ����
	if(Angle1<AxisMinAngle1||Angle1>AxisMaxAngle1)
		return;
	if(Angle2<AxisMinAngle2||Angle2>AxisMaxAngle2)
		return;
	if(Angle3<AxisMinAngle3||Angle3>AxisMaxAngle3)
		return;
	
	//���ݽǶȼ��㲽��
	long int steps1 = AngleToSteps(1,Angle1);
	long int steps2 = AngleToSteps(2,Angle2);
	long int steps3 = AngleToSteps(3,Angle3);
	RobotToSteps(1,steps1);
	RobotToSteps(2,steps2);
	RobotToSteps(3,steps3);
}
//ĳ����ֹͣ�˶�
void RobotStopOne(int Axis)
{
	switch(Axis)
	{
		case 1:
			//��ֹ���˶�
			Axis1Run = 0;
			break;
		case 2:
			//��ֹ���˶�
			Axis2Run = 0;
			break;
		case 3:
			//��ֹ���˶�
			Axis3Run = 0;
			break;
		default:
			break;
	}
}
//���е��ֹͣ�˶�
void RobotStopAll()
{
	//��ֹ���˶�
	Axis1Run = 0;
	Axis2Run = 0;
	Axis3Run = 0;
}

//��Ҫ�ڶ�ʱ�ж��������еĺ���
void AutoRun()
{
	/************************************
		��  1
	************************************/
	//����ﵽĿ�꣬��ֹͣ�˶�
	if(AxisTargetSteps1 == AxisNowSteps1)
		//ֹͣ
		RobotStopOne(1);
	
	//�ж��Ƿ������˶�
	if(Axis1Run)
	{
		//����Ŀ��λ���жϷ���
		if(AxisTargetSteps1 <= AxisNowSteps1)
			AxisDir1 = 0;
		else
			AxisDir1 = 1;
		//���·���
		DirPin1 = AxisDir1;

		//�ж��Ƿ񵽻�е��λ
		if (AxisDir1 == 0 && HomePin1 == 0 ) {
			//ֹͣ
			RobotStopOne(1);
		}
		//�ж��Ƿ�����λ,��ԭ���ʱ���ж�
		if(Axis1HomeFlag){
			if (AxisDir1 == 1 && AxisNowSteps1 >= AxisMaxStep1) {
				//ֹͣ
				RobotStopOne(1);
			}
			if (AxisDir1 == 0 && AxisNowSteps1 <= AxisMinStep1) {
				//ֹͣ
				RobotStopOne(1);
			}
		}
		if(Axis1Run)
		{
			//�����������һ��
			if(TimerCouter1==0)
				StepPin1 = 1;
			TimerCouter1++;
			if(TimerCouter1 >= TimerTotle1)
			{
				TimerCouter1 = 0;
				StepPin1 = 0;
				//���ݷ���,�Զ��ۼƵ�ǰλ��,��ԭ��ʱ���ۼ�
				if(Axis1HomeFlag)
				{
					if (AxisDir1)
						AxisNowSteps1++;
					else
						AxisNowSteps1--;
				}
			}
		}
	}
	/************************************
		��  2
	************************************/
	//����ﵽĿ�꣬��ֹͣ�˶�
	if(AxisTargetSteps2 == AxisNowSteps2)
		//ֹͣ
		RobotStopOne(2);
	
	//�ж��Ƿ������˶�
	if(Axis2Run)
	{
		//����Ŀ��λ���жϷ���
		if(AxisTargetSteps2 <= AxisNowSteps2)
			AxisDir2 = 0;
		else
			AxisDir2 = 1;
		//���·���
		DirPin2 = AxisDir2;

		//�ж��Ƿ񵽻�е��λ
		if (AxisDir2 == 0 && HomePin2 == 0 ) {
			//ֹͣ
			RobotStopOne(2);
		}
		//�ж��Ƿ�����λ,��ԭ���ʱ���ж�
		if(Axis2HomeFlag){
			if (AxisDir2 == 1 && AxisNowSteps2 >= AxisMaxStep2) {
				//ֹͣ
				RobotStopOne(2);
			}
			if (AxisDir2 == 0 && AxisNowSteps2 <= AxisMinStep2) {
				//ֹͣ
				RobotStopOne(2);
			}
		}		
		if(Axis2Run)
		{
			//�����������һ��
			if(TimerCouter2==0)
				StepPin2 = 1;
			TimerCouter2++;
			if(TimerCouter2 >= TimerTotle2)
			{
				TimerCouter2 = 0;
				StepPin2 = 0;
				//���ݷ���,�Զ��ۼƵ�ǰλ��,��ԭ��ʱ���ۼ�
				if(Axis2HomeFlag)
				{
					if (AxisDir2)
						AxisNowSteps2++;
					else
						AxisNowSteps2--;
				}
			}
		}
	}
	/************************************
	��  3 :�������1��2�෴����Ҫע��
	************************************/
	//����ﵽĿ�꣬��ֹͣ�˶�
	if(AxisTargetSteps3 == AxisNowSteps3)
		//ֹͣ
		RobotStopOne(3);
	
	//�ж��Ƿ������˶�
	if(Axis3Run)
	{
		//����Ŀ��λ���жϷ���
		if(AxisTargetSteps3 <= AxisNowSteps3)
			AxisDir3 = 0;
		else
			AxisDir3 = 1;
		//���·���
		DirPin3 = !AxisDir3;

		//�ж��Ƿ񵽻�е��λ
		if (AxisDir3 == 0 && HomePin3 == 0 ) {
			//ֹͣ
			RobotStopOne(3);
		}
		//�ж��Ƿ�����λ,��ԭ���ʱ���ж�
		if(Axis3HomeFlag){
			if (AxisDir3 == 1 && AxisNowSteps3 >= AxisMaxStep3) {
				//ֹͣ
				RobotStopOne(3);
			}
			if (AxisDir3 == 0 && AxisNowSteps3 <= AxisMinStep3) {
				//ֹͣ
				RobotStopOne(3);
			}
		}		
		if(Axis3Run)
		{
			//�����������һ��
			if(TimerCouter3==0)
				StepPin3 = 1;
			TimerCouter3++;
			if(TimerCouter3 >= TimerTotle3)
			{
				TimerCouter3 = 0;
				StepPin3 = 0;
				//���ݷ���,�Զ��ۼƵ�ǰλ��,��ԭ��ʱ�ۼ�
				if(Axis3HomeFlag)
				{
					if (AxisDir3)
						AxisNowSteps3++;
					else
						AxisNowSteps3--;
				}
			}
		}
	}
}

//��ԭ�㣬��е�۸�λ
void GoHome()
{
	Axis1Speed = 1;
	Axis2Speed = 1;
	Axis3Speed = 1;
	
	my_printf(USART3,"ϵͳ��λ��...\r\n");
  AxisNowSpeed = 50;
  /*************��-1*************/
	//��ԭ�㷽���˶�
	RobotToSteps(1,-1);
	//�ȴ�����ԭ��
  while (HomePin1 == 1) {
		osdelay_ms(5);
  }
	//��λ��1��ԭ���־
  Axis1HomeFlag = 1;
	//�˶���ָ��λ��
	RobotToSteps(1,1671);
	//�ȴ�����Ŀ��λ��
	while (AxisNowSteps1!=1671) {
		osdelay_ms(5);
  }
	my_printf(USART3,"1�ž���...\r\n");
  /*************��-3*************/

	//��ԭ�㷽���˶�
	RobotToSteps(3,-1);
	//�ȴ�����ԭ��
  while (HomePin3 == 1) {
		osdelay_ms(5);
  }
	//��λ��3��ԭ���־
  Axis3HomeFlag = 1;
	//�˶���ָ��λ��
	RobotToSteps(3,1663);
	//�ȴ�����Ŀ��λ��
	while (AxisNowSteps3!=1663) {
		osdelay_ms(5);
  }
	my_printf(USART3,"3�ž���...\r\n");
  /*************��-2*************/
	//��ԭ�㷽���˶�
	RobotToSteps(2,-1);
	//�ȴ�����ԭ��
  while (HomePin2 == 1) {
		osdelay_ms(5);
  }
	//��λ��2��ԭ���־
  Axis2HomeFlag = 1;
	//�˶���ָ��λ��
	RobotToSteps(2,8288);
	//�ȴ�����Ŀ��λ��
	while (AxisNowSteps2!=8288) {
		osdelay_ms(5);
  }
	my_printf(USART3,"2�ž���...\r\n");
	my_printf(USART3,"ϵͳ�Ѿ��������´����ָ��!\r\n");
}

//�Ƕ�ת����
int AngleToSteps(int axis,float angle)
{
  switch (axis)
  {
    case 1:
      return angle/StepAngle*Subdivide1*ReducteRatio1;
    case 2:
      return angle/StepAngle*Subdivide2*ReducteRatio2;
    case 3:
      return angle/StepAngle*Subdivide3*ReducteRatio3;
    default:
      return 0;
  }
}

//����ת�Ƕ�
float StepsToAngle(int axis,long int steps)
{
  switch (axis)
  {
    case 1:
      return  steps*StepAngle/(Subdivide1*ReducteRatio1);
    case 2:
      return steps*StepAngle/(Subdivide2*ReducteRatio2);
    case 3:
      return steps*StepAngle/(Subdivide3*ReducteRatio3);
    default:
      return 0;
  }
}
