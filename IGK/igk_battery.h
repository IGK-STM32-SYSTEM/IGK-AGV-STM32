#ifndef __IGK_BATTERY_H
#define __IGK_BATTERY_H	 
#include "usart.h"

//��ȡ�������
void Igk_Battery_Get(void);

//���õ�ز���
void Igk_Battery_Set(u16 code,u16 data);

//��Ӧ�������
void Igk_Battery_Response(void);


































#endif
