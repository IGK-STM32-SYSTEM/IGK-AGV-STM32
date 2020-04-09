#include "sys.h"
/***********************************************/
//PID变量初始化
/***********************************************/
void PID_Init(void)
{
	*IgkSystem.PID.SetTarget = 0; 
	*IgkSystem.PID.SumError = 0; 
	*IgkSystem.PID.Error = 0;
	*IgkSystem.PID.LastError = 0;
	*IgkSystem.PID.PrevError = 0;
	*IgkSystem.PID.Kp = 6;
	*IgkSystem.PID.Ki = 2;
	*IgkSystem.PID.Kd = 0;
}
/******************************************************/
//位置式PID计算
//公式:U(k) = Kp*[E(n)] + Ki*SumError + Kd*[E(n)-E(n-1)]
//参数说明:Error = E(n) 
/*******************************************************/
float PosPIDCalc(int NextTarget)
{
	float Error1;
	float Out = 0;
	u16 Kp = *IgkSystem.PID.Kp;
	u16 Ki = *IgkSystem.PID.Ki;
	u16 Kd = *IgkSystem.PID.Kd;
	
	
	*IgkSystem.PID.Error = NextTarget;
	*IgkSystem.PID.SumError +=*IgkSystem.PID.Error;							//SumError比例
	//防止饱和
	*IgkSystem.PID.SumError = *IgkSystem.PID.SumError>200?200:*IgkSystem.PID.SumError;
	*IgkSystem.PID.SumError = *IgkSystem.PID.SumError<-200?-200:*IgkSystem.PID.SumError;
	Error1 = IgkSystem.PID.Error - IgkSystem.PID.LastError;					//[E(n)-E(n-1)]微分

	IgkSystem.PID.LastError = IgkSystem.PID.Error;						//更新偏差值

	Out = Kp/10.0**IgkSystem.PID.Error + Ki/1000.0**IgkSystem.PID.SumError + Kd*Error1;	
	return(Out);
} 

