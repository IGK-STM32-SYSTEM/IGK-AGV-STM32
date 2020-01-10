//#include "igk_battery.h"
#include "usart.h"

//电池采用DMA发送和接收

//读取电池数据
void Igk_Battery_Get(void)
{
	send4_buf[0] = 0x5A;
	send4_buf[1] = 0xA5;
	send4_buf[2] = 0x10;
	send4_buf[3] = 0x00;
	send4_buf[4] = 0x00;
	send4_buf[5] = 0x00;
	send4_buf[6] = 0x00;
	send4_buf[7] = IGK_CheckSum(send4_buf,7);
	Uart4_Start_DMA_Tx(8);
}
//保存设置
void Igk_Battery_Save(void)
{
	send4_buf[0] = 0x5A;
	send4_buf[1] = 0xA5;
	send4_buf[2] = (u8)*IgkSystem.BatteryConfig.Code;
	send4_buf[3] = (u8)*IgkSystem.BatteryConfig.Data >> 8;
	send4_buf[4] = (u8)*IgkSystem.BatteryConfig.Data;
	send4_buf[5] = 0x00;
	send4_buf[6] = 0x00;
	send4_buf[7] = IGK_CheckSum(send4_buf,7);
	Uart4_Start_DMA_Tx(8);
}

