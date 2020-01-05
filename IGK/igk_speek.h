#ifndef __IGK_SPEEK_H
#define __IGK_SPEEK_H
#include "sys.h"
#include "usart.h"	
#include "sys.h"
#include "usart.h"	

extern u8 XFS_StopCom[];//停止合成
extern u8 XFS_SuspendCom[];//暂停合成
extern u8 XFS_RecoverCom[];//恢复合成
extern u8 XFS_ChackCom[];//状态查询
extern u8 XFS_PowerDownCom[];//进入POWER DOWN 状态命令
extern u8 RENSHENG[];

void speek(u8 *HZdata);
void speek2(u8 *HZdata,u16 len);

void rensheng(u8 num);//人声选择
void yinling(u8 num);//音量选择
void yusu(u8 num);//语速选择
void yudiao(u8 num);//语调选择

//IGK 专用语音播报函数
void IGK_Speek(char* fmt,...);
#endif

