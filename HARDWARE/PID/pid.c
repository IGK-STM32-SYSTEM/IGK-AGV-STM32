#include "sys.h"
struct Agv_Pid PID;
/***********************************************/
//PID变量初始化
/***********************************************/
void PID_Init(void)
{
	PID.SetTarget = 0;
	PID.SumError = 0;
	PID.Error = 0;
	PID.LastError = 0;
	PID.PrevError = 0;
	PID.Kp = 5;
	PID.Ki = 0.03;
	PID.Kd = 0;
}
/******************************************************/
//位置式PID计算
//公式:U(k) = Kp*[E(n)] + Ki*SumError + Kd*[E(n)-E(n-1)]
//参数说明:Error = E(n) 
/*******************************************************/
float PosPIDCalc(int NextTarget)
{
	float Error1;
	float Out;
//	PID.Error = PID.SetTarget - NextTarget;			//E(n)当前偏差E(n)	
	PID.Error = NextTarget;
	PID.SumError +=PID.Error;							//SumError比例
	//防止饱和
	PID.SumError = PID.SumError>200?200:PID.SumError;
	PID.SumError = PID.SumError<-200?-200:PID.SumError;
	Error1 = PID.Error - PID.LastError;					//[E(n)-E(n-1)]微分

	PID.LastError = PID.Error;						//更新偏差值

	Out = PID.Kp*PID.Error + PID.Ki*PID.SumError + PID.Kd*Error1;	
	return(Out);
} 

/***********************************************/
//增量式PID计算
//公式:U(k+1)- u(K)=Kp*(E(n)-E(n-1)) + Ki*E(n) + Kd*(E(n)-2E(n-1)+E(n-2))
//参数说明:Error = E(n) 设置与实际的偏差量
/***********************************************/
float IncPIDCalc(int error)
{
	float Uk1;
	PID.Error = error;		//E(n)
	Uk1 = PID.Kp*(PID.Error - PID.LastError) + PID.Ki*PID.Error + PID.Kd*(PID.Error - 2*PID.LastError+PID.PrevError);
	PID.PrevError = PID.LastError;			//更新误差值
	PID.LastError = PID.Error;	
	return(Uk1);
}
