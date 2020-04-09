#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
//CAN1����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.		
//CAN2����RX0�ж�ʹ��
#define CAN2_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.	

u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��
u8 CAN1_Send_Msg(u8* msg,u8 len);						//��������

//������ר��ȫ��CAN1���ͺ���
u8 Igk_CAN1_Send(u32 id,u8* msg,u8 len);
u8 CAN1_Receive_Msg(u8 *buf);							//��������

u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��
u8 CAN2_Send_Msg(u16 id ,char* msg,u8 len);						//��������
CanRxMsg CAN2_Receive_Msg(u8 *buf);							//��������

extern u8 can_Sebuf[4];
extern u8 can_Rebuf[2];
extern u8 OutPut[10]; //can�����Ӧ�ڼ̵������ʮλ�Ĵ���
extern u8 Input[12];	//can�����Ӧ�ڼ̵������ʮλ�Ĵ���

#endif

















