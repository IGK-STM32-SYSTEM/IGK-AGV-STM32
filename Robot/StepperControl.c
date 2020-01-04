#include "StepperControl.h"
//各个轴归位状态,true:已回原点，false:未回原点
u8 Axis1HomeFlag = 0;
u8 Axis2HomeFlag = 0;
u8 Axis3HomeFlag = 0;
//电机当前方向 【0:反向，1:正向】
u8 AxisDir1 = 0;
u8 AxisDir2 = 0;
u8 AxisDir3 = 0;
//电机当前步进值
long int AxisNowSteps1 = 0;
long int AxisNowSteps2 = 0;
long int AxisNowSteps3 = 0;

//当前坐标
Point AxisPoint = {175,0,215};
//目标坐标
Point TargetAxisPoint = {175,0,215};

//当前速度
float AxisNowSpeed = 30; 
float Axis1Speed = 1; 
float Axis2Speed = 1; 
float Axis3Speed = 1; 
//电机目标步进值
long int AxisTargetSteps1 = 0;
long int AxisTargetSteps2 = 0;
long int AxisTargetSteps3 = 0;

//脉冲时间间隔计时0-100对应10us-1ms的脉冲周期
long int TimerCouter1 = 0;
long int TimerCouter2 = 0;
long int TimerCouter3 = 0;
//三个轴是否运转【0:停止,1:运行】
u8 Axis1Run = 0;
u8 Axis2Run = 0;
u8 Axis3Run = 0;

//某个轴沿某个方向做连续运动
void RobotRun(int Axis, u8 dir)
{
	switch(Axis)
	{
		case 1:
			//更新目标步长
			if(dir)
				AxisTargetSteps1 = AxisMaxStep1;
			else
				AxisTargetSteps1 = AxisMinStep1;
			//允许轴运动
			Axis1Run = 1;
			break;
		case 2:
			//更新目标步长
			if(dir)
				AxisTargetSteps2 = AxisMaxStep2;
			else
				AxisTargetSteps2 = AxisMinStep2;
			//允许轴运动
			Axis2Run = 1;
			break;
		case 3:
			//更新目标步长
			if(dir)
				AxisTargetSteps3 = AxisMaxStep3;
			else
				AxisTargetSteps3 = AxisMinStep3;
			//允许轴运动
			Axis3Run = 1;
			break;
		default:
			break;
	}
}
//某个轴沿某个方向走固定步长,相对运动
void RobotRunSteps(int Axis, u8 dir, long int steps)
{
	switch(Axis)
	{
		case 1:
			//更新目标步长
			if(dir)
				AxisTargetSteps1 += steps;
			else
				AxisTargetSteps1 -= steps;
			//允许轴运动
			Axis1Run = 1;
			break;
		case 2:
			//更新目标步长
			if(dir)
				AxisTargetSteps2 += steps;
			else
				AxisTargetSteps2 -= steps;
			//允许轴运动
			Axis2Run = 1;
			break;
		case 3:
			//更新目标步长
			if(dir)
				AxisTargetSteps3 += steps;
			else
				AxisTargetSteps3 -= steps;
			//允许轴运动
			Axis3Run = 1;
			break;
		default:
			break;
	}
}
//某个轴沿某个方向走固定角度，相对运动
void RobotRunAngle(int Axis, u8 dir, float Angle)
{
	//根据角度计算步长
	long int steps = AngleToSteps(Axis,Angle);
	//沿某个方向走固定步长
	RobotRunSteps(Axis,dir,steps);
}

//单个轴走到指定步长,绝对运动
void RobotToSteps(int Axis, long int steps)
{
	switch(Axis)
	{
		case 1:
			//更新目标步长
			AxisTargetSteps1 = steps;
			//允许轴运动
			Axis1Run = 1;
			break;
		case 2:
			//更新目标步长
			AxisTargetSteps2 = steps;
			//允许轴运动
			Axis2Run = 1;
			break;
		case 3:
			//更新目标步长
			AxisTargetSteps3 = steps;
			//允许轴运动
			Axis3Run = 1;
			break;
		default:
			break;
	}
}

//单个轴走到指定角度,绝对运动
void RobotToAngle(int Axis, float Angle)
{
	//根据角度计算步长
	long int steps = AngleToSteps(Axis,Angle);
	//单个轴走到指定步长,绝对运动
	RobotToSteps(Axis,steps);
}

//三个轴同时走到指定步长,绝对运动
void RobotAllToSteps(long int steps1, long int steps2, long int steps3)
{
	//判断有没有超出范围的轴
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

//三个轴同时走到指定角度,绝对运动
void RobotAllToAngle(float Angle1,float Angle2,float Angle3)
{
	//判断有没有超出范围的轴
	if(Angle1<AxisMinAngle1||Angle1>AxisMaxAngle1)
		return;
	if(Angle2<AxisMinAngle2||Angle2>AxisMaxAngle2)
		return;
	if(Angle3<AxisMinAngle3||Angle3>AxisMaxAngle3)
		return;
	
	//根据角度计算步长
	long int steps1 = AngleToSteps(1,Angle1);
	long int steps2 = AngleToSteps(2,Angle2);
	long int steps3 = AngleToSteps(3,Angle3);
	RobotToSteps(1,steps1);
	RobotToSteps(2,steps2);
	RobotToSteps(3,steps3);
}
//某个轴停止运动
void RobotStopOne(int Axis)
{
	switch(Axis)
	{
		case 1:
			//禁止轴运动
			Axis1Run = 0;
			break;
		case 2:
			//禁止轴运动
			Axis2Run = 0;
			break;
		case 3:
			//禁止轴运动
			Axis3Run = 0;
			break;
		default:
			break;
	}
}
//所有电机停止运动
void RobotStopAll()
{
	//禁止轴运动
	Axis1Run = 0;
	Axis2Run = 0;
	Axis3Run = 0;
}

//需要在定时中断里面运行的函数
void AutoRun()
{
	/************************************
		轴  1
	************************************/
	//如果达到目标，则停止运动
	if(AxisTargetSteps1 == AxisNowSteps1)
		//停止
		RobotStopOne(1);
	
	//判断是否允许运动
	if(Axis1Run)
	{
		//根据目标位置判断方向
		if(AxisTargetSteps1 <= AxisNowSteps1)
			AxisDir1 = 0;
		else
			AxisDir1 = 1;
		//更新方向
		DirPin1 = AxisDir1;

		//判断是否到机械限位
		if (AxisDir1 == 0 && HomePin1 == 0 ) {
			//停止
			RobotStopOne(1);
		}
		//判断是否到软限位,回原点的时候不判断
		if(Axis1HomeFlag){
			if (AxisDir1 == 1 && AxisNowSteps1 >= AxisMaxStep1) {
				//停止
				RobotStopOne(1);
			}
			if (AxisDir1 == 0 && AxisNowSteps1 <= AxisMinStep1) {
				//停止
				RobotStopOne(1);
			}
		}
		if(Axis1Run)
		{
			//步进电机运行一步
			if(TimerCouter1==0)
				StepPin1 = 1;
			TimerCouter1++;
			if(TimerCouter1 >= TimerTotle1)
			{
				TimerCouter1 = 0;
				StepPin1 = 0;
				//根据方向,自动累计当前位置,回原点时不累计
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
		轴  2
	************************************/
	//如果达到目标，则停止运动
	if(AxisTargetSteps2 == AxisNowSteps2)
		//停止
		RobotStopOne(2);
	
	//判断是否允许运动
	if(Axis2Run)
	{
		//根据目标位置判断方向
		if(AxisTargetSteps2 <= AxisNowSteps2)
			AxisDir2 = 0;
		else
			AxisDir2 = 1;
		//更新方向
		DirPin2 = AxisDir2;

		//判断是否到机械限位
		if (AxisDir2 == 0 && HomePin2 == 0 ) {
			//停止
			RobotStopOne(2);
		}
		//判断是否到软限位,回原点的时候不判断
		if(Axis2HomeFlag){
			if (AxisDir2 == 1 && AxisNowSteps2 >= AxisMaxStep2) {
				//停止
				RobotStopOne(2);
			}
			if (AxisDir2 == 0 && AxisNowSteps2 <= AxisMinStep2) {
				//停止
				RobotStopOne(2);
			}
		}		
		if(Axis2Run)
		{
			//步进电机运行一步
			if(TimerCouter2==0)
				StepPin2 = 1;
			TimerCouter2++;
			if(TimerCouter2 >= TimerTotle2)
			{
				TimerCouter2 = 0;
				StepPin2 = 0;
				//根据方向,自动累计当前位置,回原点时不累计
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
	轴  3 :方向和轴1轴2相反，需要注意
	************************************/
	//如果达到目标，则停止运动
	if(AxisTargetSteps3 == AxisNowSteps3)
		//停止
		RobotStopOne(3);
	
	//判断是否允许运动
	if(Axis3Run)
	{
		//根据目标位置判断方向
		if(AxisTargetSteps3 <= AxisNowSteps3)
			AxisDir3 = 0;
		else
			AxisDir3 = 1;
		//更新方向
		DirPin3 = !AxisDir3;

		//判断是否到机械限位
		if (AxisDir3 == 0 && HomePin3 == 0 ) {
			//停止
			RobotStopOne(3);
		}
		//判断是否到软限位,回原点的时候不判断
		if(Axis3HomeFlag){
			if (AxisDir3 == 1 && AxisNowSteps3 >= AxisMaxStep3) {
				//停止
				RobotStopOne(3);
			}
			if (AxisDir3 == 0 && AxisNowSteps3 <= AxisMinStep3) {
				//停止
				RobotStopOne(3);
			}
		}		
		if(Axis3Run)
		{
			//步进电机运行一步
			if(TimerCouter3==0)
				StepPin3 = 1;
			TimerCouter3++;
			if(TimerCouter3 >= TimerTotle3)
			{
				TimerCouter3 = 0;
				StepPin3 = 0;
				//根据方向,自动累计当前位置,回原点时累计
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

//找原点，机械臂复位
void GoHome()
{
	Axis1Speed = 1;
	Axis2Speed = 1;
	Axis3Speed = 1;
	
	my_printf(USART3,"系统归位中...\r\n");
  AxisNowSpeed = 50;
  /*************轴-1*************/
	//向原点方向运动
	RobotToSteps(1,-1);
	//等待到达原点
  while (HomePin1 == 1) {
		osdelay_ms(5);
  }
	//置位轴1回原点标志
  Axis1HomeFlag = 1;
	//运动到指定位置
	RobotToSteps(1,1671);
	//等待到达目标位置
	while (AxisNowSteps1!=1671) {
		osdelay_ms(5);
  }
	my_printf(USART3,"1号就绪...\r\n");
  /*************轴-3*************/

	//向原点方向运动
	RobotToSteps(3,-1);
	//等待到达原点
  while (HomePin3 == 1) {
		osdelay_ms(5);
  }
	//置位轴3回原点标志
  Axis3HomeFlag = 1;
	//运动到指定位置
	RobotToSteps(3,1663);
	//等待到达目标位置
	while (AxisNowSteps3!=1663) {
		osdelay_ms(5);
  }
	my_printf(USART3,"3号就绪...\r\n");
  /*************轴-2*************/
	//向原点方向运动
	RobotToSteps(2,-1);
	//等待到达原点
  while (HomePin2 == 1) {
		osdelay_ms(5);
  }
	//置位轴2回原点标志
  Axis2HomeFlag = 1;
	//运动到指定位置
	RobotToSteps(2,8288);
	//等待到达目标位置
	while (AxisNowSteps2!=8288) {
		osdelay_ms(5);
  }
	my_printf(USART3,"2号就绪...\r\n");
	my_printf(USART3,"系统已就绪，请下达控制指令!\r\n");
}

//角度转步数
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

//步数转角度
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
