#include "sys.h"
/***********************************************/
//PID变量初始化
/***********************************************/
void PID_Init(void)
{
	IgkAgvOs.PID.SetTarget = 0;
	*IgkAgvOs.PID.SumError = 0;
	IgkAgvOs.PID.Error = 0;
	IgkAgvOs.PID.LastError = 0;
	IgkAgvOs.PID.PrevError = 0;
	*IgkAgvOs.PID.Kp = 5;
	*IgkAgvOs.PID.Ki = 30;
	*IgkAgvOs.PID.Kd = 0;
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
	u16 Kp = *IgkAgvOs.PID.Kp;
	u16 Ki = *IgkAgvOs.PID.Ki;
	u16 Kd = *IgkAgvOs.PID.Kd;
	
	
//	PID.Error = PID.SetTarget - NextTarget;			//E(n)当前偏差E(n)	
	IgkAgvOs.PID.Error = NextTarget;
	*IgkAgvOs.PID.SumError +=IgkAgvOs.PID.Error;							//SumError比例
	//防止饱和
	*IgkAgvOs.PID.SumError = *IgkAgvOs.PID.SumError>200?200:*IgkAgvOs.PID.SumError;
	*IgkAgvOs.PID.SumError = *IgkAgvOs.PID.SumError<-200?-200:*IgkAgvOs.PID.SumError;
	Error1 = IgkAgvOs.PID.Error - IgkAgvOs.PID.LastError;					//[E(n)-E(n-1)]微分

	IgkAgvOs.PID.LastError = IgkAgvOs.PID.Error;						//更新偏差值

	Out = Kp*IgkAgvOs.PID.Error + Ki**IgkAgvOs.PID.SumError + Kd*Error1;	
	return(Out);
} 

