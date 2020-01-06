/******************************************
爱极客网络科技有限公司
系统方法
***********************************************/
#ifndef __IGK_OS_H
#define __IGK_OS_H
#include "sys.h"
#include "cdhdata.h"
typedef unsigned short int u16;
typedef unsigned char u8;
typedef  unsigned  int u32;
#define igk_SetBit(x,y) x|=(1<<y)//指定的某一位数置1
#define igk_ClearBit(x,y) x&=~(1<<y)//指定的某一位数置0
#define igk_ReverseBit(x,y) x^=(1<<y)//指定的某一位数取反
#define igk_GetBit(x,y) ((x) >> (y)&1)//获取的某一位的值
//按键枚举,全局通用
enum ButtonEnum {KeyUp,KeyDown};
//工作模式【手动，本地自动，远程控制】
enum WorkModeEnum {Enum_LocalManual,Enum_LocalAuto,Enum_RemoteControl};
//启停状态【启动，停止】启动和停止按键或者直行路径时的启动停止标志，和障碍物的停止不一样
enum RunOrStopEnum {Enum_Run,Enum_Stop};
//方向枚举 0:前进,1:后退,2:平移
enum EnumDir{ Enum_QianJin, Enum_HouTui, Enum_PingYi};
//动作枚举 0:左分叉，1:右分叉，2:左移，3:右移，4:直行，5:左旋，6:右旋
enum EnumAction{ Enum_ZuoFenCha, Enum_YouFenCha, Enum_ZuoYi, Enum_YouYi, Enum_ZhiXing, Enum_ZuoXuan, Enum_YouXuan};

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

//通信结构体【触摸屏、调度系统、三方系统】
typedef struct {
	
	u16 *NowRfid;//当前标签
	u16 *Target;//目标标签
	u16 *NentRfid;//下一个位置
	u16 *Execute;//执行
	u16 *Cancel;//取消
}IGK_Struct_TongXin;

//系统结构体,全局通用
typedef struct {
	//系统运行时间
	IGK_Struct_DateTime OsTime;
	//摇杆控制器
	IGK_Struct_YaoGan YaoGan;
	//当前工作模式
	enum WorkModeEnum WorkMode;
	//速度
	u16 Speed;
	//启停状态
	enum RunOrStopEnum RunOrStop;
	//自动运动方向
	enum EnumDir Dir;
	//自动运动的行为
	enum EnumAction Action;
	//前磁导航
	Fencha_struct QianCiDaoHang;
	//后磁导航
	Fencha_struct HouCiDaoHang;
	//通信
	IGK_Struct_TongXin TongXin;
}IgkAgvOsStruct;






//爱极客AGV系统结构体
extern IgkAgvOsStruct IgkAgvOs;
//毫秒转时间结构体
void MillisecondToDateTime(u32 millis,IGK_Struct_DateTime *dt);
//获取系统运行时间
void GetSysRunTime(IGK_Struct_DateTime *dt,void *p_arg);

#endif












