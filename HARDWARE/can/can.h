#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    

u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
u8 CAN1_Send_Msg(u8* msg,u8 len);						//发送数据

//爱极客专用全局CAN1发送函数
u8 Igk_CAN1_Send(u32 id,u8* msg,u8 len);
u8 CAN1_Receive_Msg(u8 *buf);							//接收数据

u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
u8 CAN2_Send_Msg(u8* msg,u8 len);						//发送数据
CanRxMsg CAN2_Receive_Msg(u8 *buf);							//接收数据

extern u8 can_Sebuf[4];
extern u8 can_Rebuf[2];
extern u8 OutPut[10]; //can输出对应于继电器板的十位寄存器
extern u8 Input[12];	//can输入对应于继电器板的十位寄存器

#endif

















