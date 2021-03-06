/******************************************
爱极客网络科技有限公司
系统方法
***********************************************/
#ifndef __IGK_SYSTEM_H
#define __IGK_SYSTEM_H
#include "sys.h"
#include "cdhdata.h"
typedef unsigned short int u16;
typedef unsigned char u8;
typedef  unsigned  int u32;


//定义Flash地址段分配
//#define Flash_System_StartAddress 0;//系统参数
//#define Flash_Map_StartAddress 2000;//地图 13*1000=13k
//#define Flash_Map_StartAddress 2000;//路径 







#define igk_SetBit(x,y) x|=(1<<y)//指定的某一位数置1
#define igk_ClearBit(x,y) x&=~(1<<y)//指定的某一位数置0
#define igk_ReverseBit(x,y) x^=(1<<y)//指定的某一位数取反
#define igk_GetBit(x,y) ((x) >> (y)&1)//获取的某一位的值





//按键枚举,全局通用
enum ButtonEnum {Enum_KeyUp,Enum_KeyDown};
//工作模式【手动，本地自动，远程控制】
enum WorkModeEnum {Enum_LocalManual,Enum_LocalAuto,Enum_RemoteControl};
//启停状态【启动，停止】启动和停止按键或者直行路径时的启动停止标志，和障碍物的停止不一样
enum RunOrStopEnum {Enum_Run,Enum_Stop};
//方向枚举 0:前进,1:后退,2:平移
enum EnumDir{ Enum_QianJin, Enum_HouTui, Enum_PingYi};
//动作枚举 0:左分叉，1:右分叉，2:左移，3:右移，4:直行，5:左旋，6:右旋
enum EnumAction{ Enum_ZuoFenCha, Enum_YouFenCha, Enum_ZuoYi, Enum_YouYi, Enum_ZhiXing, Enum_ZuoXuan, Enum_YouXuan};
//充放电枚举 0:放电，1：充电
enum EnumCharge{ Enum_DisCharging, Enum_Charging };
//bool 0:False，1：True
enum EnumBool{ Enum_False, Enum_True };



//摇杆结构体
typedef struct {
	signed char x;//[-100,100]
	signed char y;//[-100,100]
	signed char z;//[-100,100]
	enum ButtonEnum key;//KeyDown,KeyUp
}IGK_Struct_YaoGan;

//时间结构体
typedef struct {
	u16 Hour;//时
	u8 Minute;//分
	u8 Second;//秒
	u16 Millisecond;//毫秒
}IGK_Struct_DateTime;

//任务结构体
typedef struct {
	u16 *SerialNum;  //任务编号【系统自动增加，可通过接口更新】
	u16 *Target;     //目标标签
	u16 *Next;   //下一个位置
	u16 *Avoid[10];   //避让站点
	u16 *Execute;    //执行
	u16 *Cancel;     //取消
//	u16 *State;      //任务状态【完成，失败，未找到路径，】
}IGK_Struct_Task;

//PID结构体定义
typedef struct {
	s16 *SetTarget;
	s16 *SumError;
	s16 *Error;			//Error[n]
	s16 *LastError;	//Error[n-1]
	s16 *PrevError;//Error[n-2]  此处改为U16或s16或指针类型，都会造成内存错误
	u16 *Kp;
	u16 *Ki;
	u16 *Kd;
}IGK_Struct_PID;

//电池结构体定义
typedef struct {
	u16 *Percent;  //电量百分比
	u16 *Voltage;   //电压【0.01V】
	u16 *Current;	//电流【0.01A】
	u16 *Total;		//电池容量【0.1AH】
	u16 *Temperature;		//温度【0.1℃】
	enum EnumCharge *Charge;//0：放电 ，1:充电
}IGK_Struct_Battery;

//电池配置结构体定义
typedef struct {
	u16 *Code;	//功能码
	u16 *Data;  //Data1在高位，Data2在低位
	enum EnumBool *Set;		//为1时触发
	u16 *Percent;//当前电量百分比
	u16 *Total;//额定容量
	u16 *UnderVoltage;//欠压值
	u16 *FullVoltage;//满电电压
}IGK_Struct_BatteryConfig;

//系统结构体,全局通用
typedef struct {
	IGK_Struct_DateTime OsTime;	             //系统运行时间
	IGK_Struct_YaoGan YaoGan;	               //摇杆控制器
	enum WorkModeEnum WorkMode;	             //当前工作模式
	u16 *ManualSpeed;	                       //手动模式速度
	u16 *AutoSpeed;	                         //自动模式速度
	u16 *RFID;	                             //实时RFID值,和读卡器读到的值同步
	enum RunOrStopEnum RunOrStop;	           //启停状态
	enum EnumDir Dir;	                       //自动运动方向
	enum EnumAction Action;	                 //自动运动的行为
	Fencha_struct QianCiDaoHang;	           //前磁导航
	Fencha_struct HouCiDaoHang;	             //后磁导航
	IGK_Struct_Task Task;                    //AGV任务
	IGK_Struct_PID PID;	                     //PID
	IGK_Struct_Battery Battery;	             //电池
	IGK_Struct_BatteryConfig BatteryConfig;	 //电池配置
}IgkSystemStruct;

//爱极客AGV系统结构体
extern IgkSystemStruct IgkSystem;
//毫秒转时间结构体
void MillisecondToDateTime(u32 millis,IGK_Struct_DateTime *dt);
//获取系统运行时间
void GetSysRunTime(IGK_Struct_DateTime *dt,void *p_arg);
//系统寄存器指针及初始值配置
void Igk_System_Init(void);
//计算校验和
u8 IGK_CheckSum(u8 *buf,u16 len);
//修正角度,避免出现无用的旋转【执行任务内部函数】
void RepairAngle(int16_t angle);
#endif












