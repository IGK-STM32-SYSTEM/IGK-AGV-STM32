#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#define SerialPrintSize 256   //���ڴ�ӡ������������С
/****************************************************
����:ͨ�ô��ڴ�ӡ�ַ���
˵��:
1.�ڲ��Զ��л�485�շ�����
2.���η��Ͳ��ó���SerialPrintSize��256Byte��
��˾:��������������Ƽ���˾
����:������
*****************************************************/
void IGK_Printf(USART_TypeDef * USARTx ,char* fmt,...);
/****************************************************
����:ͨ�ô��ڴ�ӡ����
˵��:
1.�ڲ��Զ��л�485�շ�����
��˾:��������������Ƽ���˾
����:������
*****************************************************/
void IGK_PrintBuf(USART_TypeDef* USARTx ,char* buf,u8 len); 
/****************************************************
����:ͳһ���Դ�ӡ�ӿ�
˵��:�����Ҫ��������,ֻ��Ҫ�滻�ú����ڵ�USART2����
��˾:��������������Ƽ���˾
����:������
*****************************************************/
void IGK_SysPrintf(char* fmt,...);
//��ϵͳʱ��ͻ���
void IGK_SysTimePrintln(char* fmt,...);
void USART1_Configuration(u32 bound);
void USART2_Configuration(u32 bound);
void USART3_Configuration(u32 bound);
void USART4_Configuration(u32 bound);
void USART5_Configuration(u32 bound);
void USART6_Configuration(u32 bound);
#endif


