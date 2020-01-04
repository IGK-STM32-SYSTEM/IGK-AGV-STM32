#include "communication.h"
#include "calculate.h"

//32位联合体
typedef union
{
	float fv;
	long int lv;
	u8 buf[4]; 
} union32;

//解析串口指令
void AnalysisRevice() {
	union32 u1;
	u8 i=0;
	if(rece2_index>0)
	{
		//进行桢头，CRC校验，数据长度校验
		if(CRC16Check(rece2_buf,rece2_index)&&rece2_buf[0]==0xAA && rece2_index == (rece2_buf[2] + 5))
		{
			switch (rece2_buf[1]) {
				case 0:
					//打印消息				
					my_printf(USART3,"指令0:回原点!\r\n");
					Axis1HomeFlag = 0;
					Axis2HomeFlag = 0;
					Axis3HomeFlag = 0;
					AxisNowSteps1 = 0;
				  AxisNowSteps2 = 0;
				  AxisNowSteps3 = 0;
					GoHome();
					break;
				case 1:
					//打印消息				
				my_printf(USART3,"指令1:沿某个方向走固定步长,相对运动!\r\n");
				RobotRunSteps(rece2_buf[3], rece2_buf[4], rece2_buf[5] << 8 | rece2_buf[6]);
					break;
				case 2:
					//打印消息				
					my_printf(USART3,"指令2:控制单个轴沿某个方向连续运动!\r\n");
					RobotRun(rece2_buf[3],rece2_buf[4]);
					break;
				case 3:
					//打印消息				
					my_printf(USART3,"指令3:控制单个轴停止!\r\n");
					RobotStopOne(rece2_buf[3]);
					break;
				case 4:
					//打印消息				
					my_printf(USART3,"指令4:控制所有轴停止!\r\n");
					RobotStopAll();
					break;
				case 5:
					//打印消息				
					my_printf(USART3,"指令5:设置全局速度!\r\n");
					//更新全局速度
					AxisNowSpeed = (rece2_buf[3]<<8)|rece2_buf[4];
					my_printf(USART3,"当前速度:%5.3f\r\n",AxisNowSpeed);
					break;
				case 6:
					//打印消息				
					for(i=0;i<4;i++)
						u1.buf[i] = rece2_buf[4+i];
					my_printf(USART3,"指令6:单轴运动到指定角度![%5.2f]\r\n",u1.fv);
					RobotToAngle(rece2_buf[3], u1.fv);
					break;
				case 7:
					//打印消息				
					my_printf(USART3,"指令7:X+5\r\n");
				  AxisPoint.x +=5;
					CalculateReverse(AxisPoint);
					break;
				case 8:
					//打印消息	
					my_printf(USART3,"指令8:X-5\r\n");				
				  AxisPoint.x -=5;
					CalculateReverse(AxisPoint);
					break;
				case 9:
					//打印消息				
					my_printf(USART3,"指令9:Y+5\r\n");				
				  AxisPoint.y +=5;
					CalculateReverse(AxisPoint);
					break;
				case 10:
					//打印消息				
					my_printf(USART3,"指令10:Y-5\r\n");				
				  AxisPoint.y -=5;
					CalculateReverse(AxisPoint);
					break;
				case 11:
					//打印消息				
					my_printf(USART3,"指令11:Z+5\r\n");				
				  AxisPoint.z +=5;
					CalculateReverse(AxisPoint);
					break;
				case 12:
					//打印消息				
					my_printf(USART3,"指令12:Z-5\r\n");				
				  AxisPoint.z -=5;
					CalculateReverse(AxisPoint);
					break;
				default:
					my_printf(USART3,"无效指令！\r\n");
					break;
			}
		}
		else
		{
			my_printf(USART3,"错误指令！\r\n");
		}
		//清空接收缓冲区
		memset(rece2_buf,0,UART_RX_LEN);
		rece2_index = 0;
	}
}

/*************************************************
crc16校验码计算函数,计算算法：
1、设置crc校验寄存器的初值为0xffff;
2、将报文的第一个字节与crc寄存器的低字节异或，结果存入crc寄存器
3、判断crc的第一位是否为1，如果是1,crc右移1位后和0xa001异或，如果为0，则再移1位；
4、重复步骤3，直到完成8个位；
5、重复步骤2、3、4直到完成所有字节
6、返回计算结果
***********************************************/
u16 CRC16(unsigned char *chData, unsigned short uNo)
{
	uint16_t crc = 0xffff;
	uint16_t i, j;
	for(i = 0; i < uNo; i++)
	{
		crc ^= chData[i];
		for(j = 0; j < 8; j++)
		{
			if(crc & 1)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{ crc >>= 1; }
		}
	}
	return (crc);
}

//CRC自动计算
//chData:目标数组
//len:包含CRC的长度
//校验成功:1,失败:0
u8 CRC16Check(unsigned char *chData, unsigned short len)
{
	u16 crc = 0;
	crc = CRC16(chData,len-2);
	if(crc==(chData[len-2]<<8|chData[len-1]))
		return 1;
	else
		return 0;
}










