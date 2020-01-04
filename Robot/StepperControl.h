#ifndef  _STEPPERCONTROL_H_
#define  _STEPPERCONTROL_H_
#include "sys.h"

typedef struct {
	float alpha;
	float beta;
	float gamma;
}Angle;

typedef struct {
	float x;
	float y;
	float z;
}Point;

//����������Ŷ���
#define StepPin1 OUT0
#define DirPin1 OUT1
#define HomePin1 IN0

#define StepPin2 OUT2
#define DirPin2 OUT3
#define HomePin2 IN1

#define StepPin3 OUT4
#define DirPin3 OUT5
#define HomePin3 IN2

//�����Ƕ�
#define StepAngle  1.8f

//���ϸ��
#define Subdivide1  32.0f
#define Subdivide2  32.0f
#define Subdivide3  32.0f

//���ٱ�
#define ReducteRatio1  13.7f
#define ReducteRatio2  5.18f
#define ReducteRatio3  13.7f

//ÿ�������Ч�Ƕȷ�Χ
#define AxisMinAngle1  5
#define AxisMinAngle2  5
#define AxisMinAngle3  5

#define AxisMaxAngle1  70
#define AxisMaxAngle2  200
#define AxisMaxAngle3  90

//ÿ���������ֵ��Χ,ϵͳ����ʱ���ݽǶ��Զ�����һ��,���ϵͳ����Ч��
#define AxisMinStep1  AxisMinAngle1/StepAngle*Subdivide1*ReducteRatio1
#define AxisMinStep2  AxisMinAngle2/StepAngle*Subdivide2*ReducteRatio2
#define AxisMinStep3  AxisMinAngle3/StepAngle*Subdivide3*ReducteRatio3

#define AxisMaxStep1  AxisMaxAngle1/StepAngle*Subdivide1*ReducteRatio1
#define AxisMaxStep2  AxisMaxAngle2/StepAngle*Subdivide2*ReducteRatio2
#define AxisMaxStep3  AxisMaxAngle3/StepAngle*Subdivide3*ReducteRatio3

//��ǰ�Ƕ�
#define AxisNowAngle1  AxisNowSteps1*StepAngle/Subdivide1/ReducteRatio1
#define AxisNowAngle2  AxisNowSteps2*StepAngle/Subdivide2/ReducteRatio2
#define AxisNowAngle3  AxisNowSteps3*StepAngle/Subdivide3/ReducteRatio3

//�������λ״̬,true:�ѻ�ԭ�㣬false:δ��ԭ��
extern u8 Axis1HomeFlag;
extern u8 Axis2HomeFlag;
extern u8 Axis3HomeFlag;


//�����ǰ����ֵ
extern long int AxisNowSteps1;
extern long int AxisNowSteps2;
extern long int AxisNowSteps3;
//���Ŀ�경��ֵ
extern long int AxisTargetSteps1;
extern long int AxisTargetSteps2;
extern long int AxisTargetSteps3;
//��ǰ����
extern Point AxisPoint;
//��ǰ�ٶ�
extern float AxisNowSpeed; 
extern float Axis1Speed; 
extern float Axis2Speed; 
extern float Axis3Speed; 

//��ʱʱ�� ��1000Ϊ����������ֵΪ���ļ��ٱ�*����ϸ��*�ٶȡ�
#define TimerTotle1  1000000/(Subdivide1*ReducteRatio1)/AxisNowSpeed*Axis1Speed
#define TimerTotle2  1000000/(Subdivide1*ReducteRatio1)/AxisNowSpeed*Axis2Speed
#define TimerTotle3  1000000/(Subdivide1*ReducteRatio1)/AxisNowSpeed*Axis3Speed
//����ʱ������ʱ0-100��Ӧ10us-1ms����������
extern long int TimerCouter1;
extern long int TimerCouter2;
extern long int TimerCouter3;

//�������Ƿ���ת��0:ֹͣ,1:���С�
extern u8 Axis1Run;
extern u8 Axis2Run;
extern u8 Axis3Run;

//��������
void GoHome(void);
//ĳ������ĳ�������������˶�
void RobotRun(int Axis, u8 dir);
//ĳ������ĳ�������߹̶�����,����˶�
void RobotRunSteps(int Axis, u8 dir, long int steps);
//ĳ������ĳ�������߹̶��Ƕȣ�����˶�
void RobotRunAngle(int Axis, u8 dir, float Angle);
//�������ߵ�ָ������,�����˶�
void RobotToSteps(int Axis, long int steps);
//�������ߵ�ָ���Ƕ�,�����˶�
void RobotToAngle(int Axis, float Angle);
//������ͬʱ�ߵ�ָ������,�����˶�
void RobotAllToSteps(long int steps1, long int steps2, long int steps3);
//������ͬʱ�ߵ�ָ���Ƕ�,�����˶�
void RobotAllToAngle(float Angle1,float Angle2,float Angle3);
//ĳ����ֹͣ�˶�
void RobotStopOne(int Axis);
//���е��ֹͣ�˶�
void RobotStopAll(void);
//�Զ�����
void AutoRun(void);


//�Ƕ�ת����
int AngleToSteps(int axis,float angle);
//����ת�Ƕ�
float StepsToAngle(int axis,long int steps);



#endif   // _STEPPERCONTROL_H_

