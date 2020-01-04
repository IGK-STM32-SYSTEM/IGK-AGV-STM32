#ifndef __PID_H
#define __PID_H

/***********************************************/
//PID结构体定义
/***********************************************/
struct Agv_Pid
{
	float SetTarget;
	float SumError;
	float Error;							//Error[n]
	float LastError;					//Error[n-1]
	float PrevError;					//Error[n-2]
	float Kp;
	float Ki;
	float Kd;
};

void PID_Init(void);
float IncPIDCalc(int error);
float PosPIDCalc(int NextTarget);
extern struct  Agv_Pid PID;
#endif
