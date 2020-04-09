#ifndef __IGK_BATTERY_H
#define __IGK_BATTERY_H	 
#include "usart.h"

//读取电池数据
void Igk_Battery_Get(void);

//设置电池参数
void Igk_Battery_Set(u16 code,u16 data);

//响应电池配置
void Igk_Battery_Response(void);


































#endif
