#ifndef __IGK_SPEEK_H
#define __IGK_SPEEK_H
#include "sys.h"
#include "usart.h"	
#include "sys.h"
#include "usart.h"	

extern u8 XFS_StopCom[];//ֹͣ�ϳ�
extern u8 XFS_SuspendCom[];//��ͣ�ϳ�
extern u8 XFS_RecoverCom[];//�ָ��ϳ�
extern u8 XFS_ChackCom[];//״̬��ѯ
extern u8 XFS_PowerDownCom[];//����POWER DOWN ״̬����
extern u8 RENSHENG[];

void speek(u8 *HZdata);
void speek2(u8 *HZdata,u16 len);

void rensheng(u8 num);//����ѡ��
void yinling(u8 num);//����ѡ��
void yusu(u8 num);//����ѡ��
void yudiao(u8 num);//���ѡ��

//IGK ר��������������
void IGK_Speek(char* fmt,...);
#endif

