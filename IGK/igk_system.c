#include "igk_system.h"
#include "os.h"

//爱极客AGV系统结构体
IgkSystemStruct IgkSystem;

//系统寄存器指针及初始值配置
void Igk_System_Init(void)
{
	/*-----------系统指针类型变量指向默认地址---------------*/
	//
	IgkSystem.RFID = &PLC_Data[41];//实时RFID值,和读卡器读到的值同步
	IgkSystem.AutoSpeed = &PLC_Data[120];//自动模式速度
	IgkSystem.ManualSpeed = &PLC_Data[121];//手动模式速度
	
	//任务Task
	IgkSystem.Task.Target = &PLC_Data[40];     //目标标签
	IgkSystem.Task.Next = &PLC_Data[42];   //下一个位置
	IgkSystem.Task.Execute = &PLC_Data[43];    //执行
	IgkSystem.Task.Cancel = &PLC_Data[44];     //取消
	IgkSystem.Task.SerialNum = &PLC_Data[45];  //任务编号【系统自动增加，可通过接口更新】
	IgkSystem.Task.Avoid[0] = &PLC_Data[46];   //避让点46-55，占用10个空间
	IgkSystem.Task.Avoid[1] = &PLC_Data[47];   //避让点46-55，占用10个空间
	IgkSystem.Task.Avoid[2] = &PLC_Data[48];   //避让点46-55，占用10个空间
	IgkSystem.Task.Avoid[3] = &PLC_Data[49];   //避让点46-55，占用10个空间
	IgkSystem.Task.Avoid[4] = &PLC_Data[50];   //避让点46-55，占用10个空间
	IgkSystem.Task.Avoid[5] = &PLC_Data[51];   //避让点46-55，占用10个空间
	IgkSystem.Task.Avoid[6] = &PLC_Data[52];   //避让点46-55，占用10个空间
	IgkSystem.Task.Avoid[7] = &PLC_Data[53];   //避让点46-55，占用10个空间
	IgkSystem.Task.Avoid[8] = &PLC_Data[54];   //避让点46-55，占用10个空间
	IgkSystem.Task.Avoid[9] = &PLC_Data[55];   //避让点46-55，占用10个空间
	
	//电池
	IgkSystem.Battery.Percent = &PLC_Data[100];  //剩余电量百分比
	IgkSystem.Battery.Voltage = &PLC_Data[101];   //电压
	IgkSystem.Battery.Current = &PLC_Data[102];	//电流
	IgkSystem.Battery.Total = &PLC_Data[103];		//电池容量
	IgkSystem.Battery.Temperature = &PLC_Data[104];//温度
	IgkSystem.Battery.Charge = (enum EnumCharge*)&PLC_Data[105];//电流方向【充放电】	//PID	
	
	//电池配置
	IgkSystem.BatteryConfig.Code = &PLC_Data[106];  //功能码
	IgkSystem.BatteryConfig.Data = &PLC_Data[107];   //Data1在高位，Data2在低位
	IgkSystem.BatteryConfig.Save = (enum EnumBool*)&PLC_Data[108];		//为1时触发
	
	//PID配置
	IgkSystem.PID.SetTarget = (s16*)&PLC_Data[200]; 
	IgkSystem.PID.SumError = (s16*)&PLC_Data[201]; 
	IgkSystem.PID.Error = (s16*)&PLC_Data[202]; 
	IgkSystem.PID.LastError = (s16*)&PLC_Data[203]; 
	IgkSystem.PID.Kp = &PLC_Data[205]; 
	IgkSystem.PID.Ki = &PLC_Data[206]; 
	IgkSystem.PID.Kd = &PLC_Data[207]; 
	
	/*------------设定默认参数-------------------------------*/
	//初始化时间
	IgkSystem.OsTime.Hour = 0;
	IgkSystem.OsTime.Minute = 0;
	IgkSystem.OsTime.Second = 0;
	IgkSystem.OsTime.Millisecond = 0;
	
	IgkSystem.WorkMode = Enum_LocalAuto;//自动模式
	IgkSystem.Dir = Enum_QianJin;       //前进方向
	IgkSystem.Action = Enum_ZhiXing;    //直行
	*IgkSystem.RFID = 1;                //RFID值
	IgkSystem.RunOrStop = Enum_Stop;    //停止状态
	*IgkSystem.AutoSpeed = 50;          //自动速度;	
	*IgkSystem.ManualSpeed = 80;        //手动速度;
	
}

//计算校验和
u8 IGK_CheckSum(u8 *buf,u16 len)
{
	u8 add = 0;
	for(int i=0;i<len;i++)
	{
		 add +=*buf;
		 buf++;
	}
	return add;
}

//毫秒转时间结构体
void MillisecondToDateTime(u32 millis,IGK_Struct_DateTime *dt)
{
	//毫秒转时间
	u32 temp = millis;
	u32 hper = 60 * 60 * 1000;
	u32 mper = 60 * 1000;
	u32 sper = 1000;
	u16 hh=0,mm=0,ss=0,mmm=0;
	if (temp / hper > 0) {
		hh  = temp / hper;
	}
	temp = temp % hper;
	if (temp / mper > 0) {
		mm = temp / mper;
	}
	temp = temp % mper;
	if (temp / sper > 0) {
		ss = temp / sper;
	}
	mmm = temp % sper;
	dt->Hour = hh;
	dt->Minute = mm;
	dt->Second = ss;
	dt->Millisecond = mmm;
}

//获取系统运行时间
void GetSysRunTime(IGK_Struct_DateTime *dt,void *p_arg)
{
	//打印系统时间
	u32 tick = OSTimeGet(p_arg);
	//系统节拍是5
	tick*=5;
	MillisecondToDateTime(tick,dt);
}










