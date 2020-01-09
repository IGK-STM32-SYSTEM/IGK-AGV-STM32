#ifndef __Modbus_slave_H
#define	__Modbus_slave_H	 
#include "sys.h"
//保持寄存器数量
#define HoldingRegMax 4096

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
extern u8 PLC_InPut[128];//PLC输入状态寄存器
extern u8 PLC_OutPut[128];//PLC输出状态寄存器
extern u16 PLC_Data[HoldingRegMax];//PLC数据寄存器
#endif
