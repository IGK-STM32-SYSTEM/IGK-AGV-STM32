#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    

//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	1		//0,不使能;1,使能.								    
#define CAN2_RX0_INT_ENABLE	1										 							 				    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
u8 CAN1_Send_Msg(u8* msg,u8 len);						//发送数据
//爱极客专用全局CAN1发送函数
u8 Igk_CAN1_Send(u32 id,u8* msg,u8 len);
u8 CAN1_Receive_Msg(u8 *buf);							//接收数据

u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
u8 CAN2_Send_Msg(u8* msg,u8 len);						//发送数据
CanRxMsg CAN2_Receive_Msg(u8 *buf);							//接收数据
void CanSend(void);

extern u8 can_Sebuf[4];
extern u8 can_Rebuf[2];
extern u8 OutPut[10]; //can输出对应于继电器板的十位寄存器
extern u8 Input[12];	//can输入对应于继电器板的十位寄存器

extern u8 yuyin_flag;
extern u16 cdh_16_qian,cdh_16_hou;

//#define P24 OutPut[0]		//手柄+24切换
//#define N24 OutPut[1]   //手柄GND切换
//#define CBS OutPut[2]   //叉臂上升
//#define CBX OutPut[3]   //叉臂下降
//#define JJC OutPut[4]   //接近开关断串入的一端
//#define JJB OutPut[5]   //接近开关断并入的一端
//#define FX1 OutPut[6]   //叉车行驶方向控制端1
//#define FX2 OutPut[7]   //叉车行驶方向控制端2
//#define ZPWM OutPut[8]  //转向PWM
//#define FPWM OutPut[9]  //方向PWM

#define HANDMODE Input[0]//继电器板控制手动
#define AUTOMODE Input[1]//继电器板控制自动
#define START 	 Input[2]//遥控的启动
#define MAIN 		 Input[3]//遥控的总开关
#define FRONT 	 Input[4]//遥控的前进
#define BACK 		 Input[5]//遥控的后退
#define LIFT 		 Input[6]//遥控的左
#define RIGHT 	 Input[7]//遥控的右
#define UP 			 Input[8]//遥控的上
#define DOWN 		 Input[9]//遥控的下


#endif

















