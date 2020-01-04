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

//步进电机引脚定义
#define StepPin1 OUT0
#define DirPin1 OUT1
#define HomePin1 IN0

#define StepPin2 OUT2
#define DirPin2 OUT3
#define HomePin2 IN1

#define StepPin3 OUT4
#define DirPin3 OUT5
#define HomePin3 IN2

//步进角度
#define StepAngle  1.8f

//电机细分
#define Subdivide1  32.0f
#define Subdivide2  32.0f
#define Subdivide3  32.0f

//减速比
#define ReducteRatio1  13.7f
#define ReducteRatio2  5.18f
#define ReducteRatio3  13.7f

//每个轴的有效角度范围
#define AxisMinAngle1  5
#define AxisMinAngle2  5
#define AxisMinAngle3  5

#define AxisMaxAngle1  70
#define AxisMaxAngle2  200
#define AxisMaxAngle3  90

//每个轴的允许步值范围,系统启动时根据角度自动计算一次,提高系统运行效率
#define AxisMinStep1  AxisMinAngle1/StepAngle*Subdivide1*ReducteRatio1
#define AxisMinStep2  AxisMinAngle2/StepAngle*Subdivide2*ReducteRatio2
#define AxisMinStep3  AxisMinAngle3/StepAngle*Subdivide3*ReducteRatio3

#define AxisMaxStep1  AxisMaxAngle1/StepAngle*Subdivide1*ReducteRatio1
#define AxisMaxStep2  AxisMaxAngle2/StepAngle*Subdivide2*ReducteRatio2
#define AxisMaxStep3  AxisMaxAngle3/StepAngle*Subdivide3*ReducteRatio3

//当前角度
#define AxisNowAngle1  AxisNowSteps1*StepAngle/Subdivide1/ReducteRatio1
#define AxisNowAngle2  AxisNowSteps2*StepAngle/Subdivide2/ReducteRatio2
#define AxisNowAngle3  AxisNowSteps3*StepAngle/Subdivide3/ReducteRatio3

//各个轴归位状态,true:已回原点，false:未回原点
extern u8 Axis1HomeFlag;
extern u8 Axis2HomeFlag;
extern u8 Axis3HomeFlag;


//电机当前步进值
extern long int AxisNowSteps1;
extern long int AxisNowSteps2;
extern long int AxisNowSteps3;
//电机目标步进值
extern long int AxisTargetSteps1;
extern long int AxisTargetSteps2;
extern long int AxisTargetSteps3;
//当前坐标
extern Point AxisPoint;
//当前速度
extern float AxisNowSpeed; 
extern float Axis1Speed; 
extern float Axis2Speed; 
extern float Axis3Speed; 

//计时时间 【1000为基数，具体值为最大的减速比*最大的细分*速度】
#define TimerTotle1  1000000/(Subdivide1*ReducteRatio1)/AxisNowSpeed*Axis1Speed
#define TimerTotle2  1000000/(Subdivide1*ReducteRatio1)/AxisNowSpeed*Axis2Speed
#define TimerTotle3  1000000/(Subdivide1*ReducteRatio1)/AxisNowSpeed*Axis3Speed
//脉冲时间间隔计时0-100对应10us-1ms的脉冲周期
extern long int TimerCouter1;
extern long int TimerCouter2;
extern long int TimerCouter3;

//三个轴是否运转【0:停止,1:运行】
extern u8 Axis1Run;
extern u8 Axis2Run;
extern u8 Axis3Run;

//声明函数
void GoHome(void);
//某个轴沿某个方向做连续运动
void RobotRun(int Axis, u8 dir);
//某个轴沿某个方向走固定步长,相对运动
void RobotRunSteps(int Axis, u8 dir, long int steps);
//某个轴沿某个方向走固定角度，相对运动
void RobotRunAngle(int Axis, u8 dir, float Angle);
//单个轴走到指定步长,绝对运动
void RobotToSteps(int Axis, long int steps);
//单个轴走到指定角度,绝对运动
void RobotToAngle(int Axis, float Angle);
//三个轴同时走到指定步长,绝对运动
void RobotAllToSteps(long int steps1, long int steps2, long int steps3);
//三个轴同时走到指定角度,绝对运动
void RobotAllToAngle(float Angle1,float Angle2,float Angle3);
//某个轴停止运动
void RobotStopOne(int Axis);
//所有电机停止运动
void RobotStopAll(void);
//自动运行
void AutoRun(void);


//角度转步数
int AngleToSteps(int axis,float angle);
//步数转角度
float StepsToAngle(int axis,long int steps);



#endif   // _STEPPERCONTROL_H_

