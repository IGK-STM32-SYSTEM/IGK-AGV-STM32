#ifndef __XF_H
#define __XF_H
#include "sys.h"
#include "usart.h"	

extern u8 XFS_StopCom[];//停止合成
extern u8 XFS_SuspendCom[];//暂停合成
extern u8 XFS_RecoverCom[];//恢复合成
extern u8 XFS_ChackCom[];//状态查询
extern u8 XFS_PowerDownCom[];//进入POWER DOWN 状态命令

extern u8 XIAO_YAN[];//语音小燕
extern u8 XU_JIU[];//语音许久
extern u8 YU_DUO[];//语音许多
extern u8 XIAO_PING[];//语音小萍
extern u8 LAO_YA[];//语音唐老鸭
extern u8 XIAO_BAO[];//语音许小宝
//音量选择
extern u8 YIN_LIANG0[];//静音
extern u8 YIN_LIANG1[];//1
extern u8 YIN_LIANG2[];//2
extern u8 YIN_LIANG3[];//3
extern u8 YIN_LIANG4[];//4
extern u8 YIN_LIANG5[];//5
extern u8 YIN_LIANG6[];//6
extern u8 YIN_LIANG7[];//7
extern u8 YIN_LIANG8[];//8
extern u8 YIN_LIANG9[];//9

//语速选择
extern u8 YIN_SU0[];//最慢
extern u8 YIN_SU1[];//1
extern u8 YIN_SU2[];//2
extern u8 YIN_SU3[];//3
extern u8 YIN_SU4[];//4
extern u8 YIN_SU5[];//5
extern u8 YIN_SU6[];//6
extern u8 YIN_SU7[];//7
extern u8 YIN_SU8[];//8
extern u8 YIN_SU9[];//9

//语调选择
extern u8 YIN_DIAO0[];//最底
extern u8 YIN_DIAO1[];//1
extern u8 YIN_DIAO2[];//2
extern u8 YIN_DIAO3[];//3
extern u8 YIN_DIAO4[];//4
extern u8 YIN_DIAO5[];//5
extern u8 YIN_DIAO6[];//6
extern u8 YIN_DIAO7[];//7
extern u8 YIN_DIAO8[];//8
extern u8 YIN_DIAO9[];//9


void speek(u8 *HZdata);
void YS_XFS_Set(u8 *Info_data);

void rensheng(u8 num);//人声选择
void yinling(u8 num);//音量选择
void yusu(u8 num);//语速选择
void yudiao(u8 num);//语调选择
void set_yinling(u8 num);
//IGK 专用语音播报函数
void IGK_Speek(char* fmt,...);
#endif

