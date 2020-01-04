#ifndef __Modbus_slave_H
#define	__Modbus_slave_H	 
#include "sys.h"
//extern  u16   Word[64];       //定义字寻址区，32个short型 ,有符号型
//extern 	u8    Bit[32];        // 定义位寻址区，10-31内部位，0-9输出口
//extern 	u8  inputBit[32];
//extern 	u16 *pw;
//extern 	u8 *pb;
//extern 	u16 sendlength;

void AnalyzeRecieve(void);                              
void send_comm(void);
void Recirve_01(void);
void Recirve_02(void);
void Recirve_03(void);
void Recirve_04(void);
void Recirve_05(void);
void Recirve_06(void);
void Recirve_10(void);
void errorsend(u8 func,u8 type);
u16 CalcCrc(unsigned char *chData,unsigned short uNo);
//extern u8  Reciver_bit;//接收位置一
extern u8 PLC_InPut[64];//PLC输入状态寄存器
extern u8 PLC_OutPut[128];//PLC输出状态寄存器
extern u16 PLC_Data[128];//PLC数据寄存器
#endif
