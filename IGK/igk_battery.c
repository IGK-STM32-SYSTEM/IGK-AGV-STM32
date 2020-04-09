#include "igk_battery.h"

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
//设置电池参数
void Igk_Battery_Set(u16 code,u16 data)
{
	send4_buf[0] = 0x5A;
	send4_buf[1] = 0xA5;
	send4_buf[2] = (u8)code;
	send4_buf[3] = data>>8;
	send4_buf[4] = data;
	send4_buf[5] = 0x00;
	send4_buf[6] = 0x00;
	send4_buf[7] = IGK_CheckSum(send4_buf,7);
	Uart4_Start_DMA_Tx(8);
}
//响应电池配置
void Igk_Battery_Response()
{
	if(*IgkSystem.BatteryConfig.Set == Enum_True)
		{
			Igk_Battery_Set(*IgkSystem.BatteryConfig.Code,*IgkSystem.BatteryConfig.Data);
			*IgkSystem.BatteryConfig.Set = Enum_False;
		}
		//当前电量百分比
		if(*IgkSystem.BatteryConfig.Percent!=0)
		{
			Igk_Battery_Set(0x14,*IgkSystem.BatteryConfig.Percent<<8);
			*IgkSystem.BatteryConfig.Percent = 0;
		}
		//额定容量
		if(*IgkSystem.BatteryConfig.Total!=0)
		{
			Igk_Battery_Set(0x15,*IgkSystem.BatteryConfig.Total);
			*IgkSystem.BatteryConfig.Total = 0;
		}		
		//电池欠压值
		if(*IgkSystem.BatteryConfig.UnderVoltage!=0)
		{
			Igk_Battery_Set(0x17,*IgkSystem.BatteryConfig.UnderVoltage);
			*IgkSystem.BatteryConfig.UnderVoltage = 0;
		}		
		//满电电压值
		if(*IgkSystem.BatteryConfig.FullVoltage!=0)
		{
			Igk_Battery_Set(0x18,*IgkSystem.BatteryConfig.FullVoltage);
			*IgkSystem.BatteryConfig.FullVoltage = 0;
		}
}
