#ifndef __ROUTE_H
#define __ROUTE_H
#include "sys.h"
#include "Modbus_slave.h"
#include "w25qxx.h"
//////////////////flash内存管理//////////////////////////////////////////////////////////////////

//请根据实际需求配置各个存储块参数
//系统参数
#define SystemParaNum 101  //系统信息总数，其中0用于判断flash是否被初始化，或者状态是否正常
extern u16 SystemParameter[SystemParaNum];//系统参数

//路径: 共500个路径，每个路径100个站点，每个站点16个信息，每个信息2个字节
#define RouteNum 500  //路径总数
#define StationNum 100 //每条路径包含站点数
#define StaiionInfor 20 //每个站点包含信息数

#define RouteStationNumAdress(n) SystemParaNum+n-1  //每条路径包含站点数信息地址
#define NowRouteInforAdress(n)       RouteStationNumAdress(RouteNum)+1+StationNum*StaiionInfor*(n-1)  //当前路径信息地址
#define NowStationInforAdress(m,n)    NowRouteInforAdress(m)+StaiionInfor*(n)//当前路径，当前站点信息地址
extern u16 RouteStationNum[RouteNum]; 		//每条路径包含站点数
extern u16 NowRouteInfor[StationNum][StaiionInfor];   //当前路径信息
//extern u16 StationInfor[StaiionInfor];          //单个站点信息       

//流程: 共500个流程，每个流程100步，每步5个信息，每个信息2个字节
#define ProcessNum 500  //流程总数
#define StepNum 100 //每条流程包含步数
#define StepInfor 3 //每步包含信息数

#define ProcessStepNumAdress(n)   NowRouteInforAdress(RouteNum+1)+n  //每个流程包含步数信息地址
#define NowProcessInforAdress(n)  ProcessStepNumAdress(ProcessNum)+1+StepNum*StepInfor*(n-1)  //当前流程信息地址
#define NowStepInforAdress(m,n)  NowProcessInforAdress(m)+StepInfor*(n)  //当前流程、当前步骤信息地址
extern u16 ProcessStepNum[ProcessNum]; 		   //每个流程包含步数
extern u16 NowProcessInfor[StepNum][StepInfor];  //当前流程信息
//extern u16 NowStepInfor[StepInfor];              //单个步骤信息

extern u16 HmiProcessNumFlag; //上一个流程号，判断流程变化用
extern u16 HmiRouteNumFlag; //上一个路径号，判断路径变化用
extern u16 HmiRouteCopiedNumTemp;//被复制路径号临时变量
extern u16 HmiProcessCopiedNumTemp;//被复制流程号临时变量


//////////////////////////触摸屏地址映射表/////////////////////////////////////////////////////////////////////

#define HmiScreenSetGet                 PLC_Data[15]   //设置获取触摸屏界面号
#define ZiDongMoShiJieMian   			3   //自动模式界面
#define FuZhiLuJingTanChuJieMian        11   //路径复制弹出界面  
#define ZhiXingLuJingJieMian            18  //执行路径界面
#define LuJingGuiHuaJieMian             5  //路径规划界面
#define LuJingSheZhiJieMian           	16  //路径设置界面
#define QueDingChaRuLuJingJieMian     	6   //确定插入路径界面
#define QueDingShanChuLuJingJieMian   	7   //确定删除路径界面

#define LiuChengGuiHuaJieMian           10   //流程规划界面
#define LiuChengSheZhiJieMian           15   //流程设置界面
#define ZhiXingLiuChengJieMian          19   //执行流程界面


#define FuZhiLiuChengTanChuJieMian      12   //复制流程弹出界面
#define QueDingChaRuLiuChengJieMian     14   //确定插入流程界面
#define QueDingShanChuLiuChengJieMian   13   //确定删除流程界面
#define QuXiaoRenWuJieMian              20   //取消任务界面


#define HmiScreenControlMode PLC_OutPut[19]   //操作模式手动、自动
#define HmiShouZiDongYinCang 		PLC_OutPut[20]   //手自动隐藏
#define HmiQiDong 			PLC_OutPut[21]   //启动
#define HmiTinZhi 			PLC_OutPut[22]   //停止

#define HmiScreenRunState    PLC_Data[96]   //运行状态  启动，停止

#define HmiRfidNum           HmiRoadData6 //地标号

#define HmiRoadData1         PLC_Data[13] //远程红外 默认为0 0：保持1：开2：关
#define HmiRoadData2         PLC_Data[10] //岔路口左分叉 0：保持1：左分叉 2：右分叉	
#define HmiRoadData3         PLC_Data[75] //突变次数
#define HmiRoadData4         PLC_Data[76] //旋转次数	
#define HmiRoadData5         PLC_Data[21] //档位默认选择慢速 0：保持 1：慢速 2：中速 3：快速   	       
#define HmiRoadData6         PLC_Data[1] //地标	
#define HmiRoadData7         PLC_Data[4] //动作	0：停止	1：前进	2:后左	3：后右  4：通过
#define HmiRoadData8         PLC_Data[5] //0：自动 1：手动
#define HmiRoadData9         PLC_Data[6] //时间

#define LuJingZongShu        PLC_Data[17] //路径总数
#define LiuChengZongShu        PLC_Data[74] //路径总数

#define HmiRouteNum          PLC_Data[27] //路径号
#define HmiStationNum        PLC_Data[19] //站点数
#define HmiStationSerialNum  PLC_Data[18] //站点序号
#define HmiRouteEnter        PLC_Data[12] //确定1，取消2
#define HmiStationLast       PLC_Data[71] //上一个1
#define HmiStationNext       PLC_Data[71] //下一个2
#define HmiRoutemode         PLC_Data[11] //模式，查看0，复制1，添加2，插入3，更改4，删除5
#define HmiRouteCopiedNum    PLC_Data[52] //被复制路径号
#define HmiRouteCopiedCount  PLC_Data[53] //被复制站点数
#define HmiNextRfidNum       PLC_Data[30] //下一个地标
#define ZhiXingLuJingQueDing PLC_Data[65] //执行路径确定按钮



//////流程规划
#define HmiProcessNum          PLC_Data[54] //流程号
#define HmiStepNum             PLC_Data[55] //流程步数
#define HmiProcessSerialNum    PLC_Data[56] //流程序号
#define HmiProcessStartNum     PLC_Data[57] //开始路径号
#define HmiProcessAction       PLC_Data[58] //流程动作
#define HmiProcessStopNum      PLC_Data[59] //循环和结束数据

#define HmiProcessEnter        PLC_Data[61] //确定1，取消2

#define HmiProcessLast         PLC_Data[60] //上一个
#define HmiProcessNext         PLC_Data[60] //下一个

#define HmiProcessMode         PLC_Data[62] //模式，查看0，复制1，添加2，插入3，更改4，删除5

#define HmiProcessCopiedNum    PLC_Data[63] //被复制流程号
#define HmiProcessCopiedCount  PLC_Data[64] //被复制流程步数

#define HmiProcessMessage      PLC_Data[69] //流程规划操作提示信息

#define HmiTask                PLC_Data[66] //执行任务 无任务0 执行路径1 执行流程2
#define HmiTaskState           PLC_Data[70] //任务状态、运行状态
#define ZhiXingLiuChengQueDing PLC_Data[67] //执行流程确定按钮
#define QuXiaoRenWuQueDing     PLC_Data[68] //取消任务确定按钮

#define CheHao     			   PLC_Data[72] //车号
//---------------系统------------//
//从系统获取一个参数 num:参数位置
extern u16 GetOneParameterFromSystem(u16 num);

//获取所有系统参数，并存入默认数组SystemParameter
extern void GetAllParameterFromSystem(void);

//设置一个参数到系统 value:参数值 num:参数序号
extern void SetOneParameterToSystem(u16 value,u16 num);

//设置所有参数到系统 将SystemParameter数组的数据存入Flash
extern void SetAllParameterToSystem(void);

//---------------路径------------//
//获取路径包含站点数 num:路径号
extern u16 GetRouteStationNum(u16 num);

//保存所有路径包含的站点数
extern void SetAllStationNum(void);

//获取路径信息,num:路径号
extern void GetRouteData(u16 num);

//保存路径信息,num:路径号
extern void SetRouteData(u16 num);

//保存站点信息,num:路径号，stationnum
void SetStationData(u16 num,u16 stationnum);

//更新路径站点数据到界面
extern void UpdataStationToHmi(void);

//更新路径站点数据到当前(当前数组)
void UpdataStationToNow(void);

//更新路径站点数据到界面
extern void ClearStationToHmi(void);

//---------------流程------------//

//保存及更新流程数据（当前操作的流程信息）
extern void	SetProcessData(void);

//获取流程包含步数
extern u16	GetProcessStepNum(u16 num);

//保存所有流程包含步数
extern void SetAllStepNum(void);

//获取及更新流程数据（当前操作的流程信息）
extern void	GetProcessData(void);

//获取及更新指定流程数据（指定的流程信息）
extern void	GetProcessDataFrom(u16 num);
	
//更新流程数据到界面
extern void UpdataProcessToHmi(void);

//更新流程数据到界面
extern void ClearStepToHmi(void);

//插入及更新流程数据（当前操作的流程信息）
extern void	InsertProcessData(void);

//删除及更新流程数据（当前操作的流程信息）
extern void	DeleteProcessData(void);

//自动界面显示信息重载
extern void HmiAutoReload(void);






#endif
