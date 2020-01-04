#ifndef  _COMMUNICATION_H_
#define  _COMMUNICATION_H_
#include "steppercontrol.h"

//解析串口指令
void AnalysisRevice(void);
u16 CRC16(unsigned char *chData, unsigned short uNo);
//CRC自动计算
//chData:目标数组
//len:包含CRC的长度
//校验成功:1,失败:0
u8 CRC16Check(unsigned char *chData, unsigned short len);














#endif
