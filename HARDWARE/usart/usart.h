#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#define SerialPrintSize 256   //串口打印函数缓冲区大小
/****************************************************
方法:通用串口打印字符串
说明:
1.内部自动切换485收发引脚
2.单次发送不得超过SerialPrintSize【256Byte】
公司:西安爱极客网络科技公司
姓名:孙毅明
*****************************************************/
void IGK_Printf(USART_TypeDef * USARTx ,char* fmt,...);
/****************************************************
方法:通用串口打印数组
说明:
1.内部自动切换485收发引脚
公司:西安爱极客网络科技公司
姓名:孙毅明
*****************************************************/
void IGK_PrintBuf(USART_TypeDef* USARTx ,char* buf,u8 len); 
/****************************************************
方法:统一调试打印接口
说明:如果需要更换串口,只需要替换该函数内的USART2即可
公司:西安爱极客网络科技公司
姓名:孙毅明
*****************************************************/
void IGK_SysPrintf(char* fmt,...);
//带系统时间和换行
void IGK_SysTimePrintln(char* fmt,...);
void USART1_Configuration(u32 bound);
void USART2_Configuration(u32 bound);
void USART3_Configuration(u32 bound);
void USART4_Configuration(u32 bound);
void USART5_Configuration(u32 bound);
void USART6_Configuration(u32 bound);
#endif


