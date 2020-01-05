#include "sys.h"
/**************芯片设置命令*********************/
u8 XFS_StopCom[]={0xFD,0X00,0X01,0X02};		//停止合成
u8 XFS_SuspendCom[]={0XFD,0X00,0X01,0X03};//暂停合成
u8 XFS_RecoverCom[]={0XFD,0X00,0X01,0X04};//恢复合成
u8 XFS_ChackCom[]={0XFD,0X00,0X01,0X21};	//状态查询
u8 XFS_PowerDownCom[]={0XFD,0X00,0X01,0X88};//进入POWER DOWN 状态命令
//人声选择
u8 RENSHENG[]={3,51,52,53,54,55};

/************************************************************************
功能描述： 串口发送字符串数据
入口参数： 	*DAT：字符串指针
返 回 值： none
其他说明： API 供外部使用，直观！
**************************************************************************/
void PrintCom(u8 *DAT,u16 len)
{
	u16 i;
	if(1)//控制语音的开和
	{
		/* TC TXE 标志位在复位的时候被置1 */  
		/*第一句读取SR寄存器，第二句写DR寄存器*/  
		/* 如果不这样操作，首次发送的第一个字符会丢失 */ 
//		USART_GetFlagStatus(USART1, USART_FLAG_TC);
		for(i=0;i<len;i++)
		{
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			USART_SendData(USART1, *DAT++);
			
		}	
	}
	else 
	{
		;
	}
}


/***********************************************************
* 名    称：  YS-XFS5051 文本合成函数
* 功    能：  发送合成文本到XFS5051芯片进行合成播放
* 入口参数：  *HZdata:文本指针变量 
* 出口参数：
* 说    明： 本函数只用于文本合成，具备背景音乐选择。默认波特率9600bps。					 
* 调用方法：例： SYN_FrameInfo（“飞音云电子”）；
**********************************************************/
void speek(u8 *HZdata)
{
/****************需要发送的文本**********************************/ 
		u8 Frame_Info[100]; //定义的文本长度
		u16 HZ_Length;  

		HZ_Length =strlen((const char *)HZdata); 			//需要发送文本的长度
 		 
/*****************帧固定配置信息**************************************/           
		 Frame_Info[0] = 0xFD ; 			//构造帧头FD
		 Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
		 Frame_Info[2] = HZ_Length+2; //构造数据区长度的低字节
		 Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令		 		 
		 Frame_Info[4] = 0x01;        //文本编码格式：GBK 
	
/*******************发送帧信息***************************************/		  
     memcpy(&Frame_Info[5], HZdata, HZ_Length);
		 PrintCom(Frame_Info,5+HZ_Length); //发送帧配置
}

void speekWihtLen(u8 *HZdata,u16 len)
{
/****************需要发送的文本**********************************/ 
		u8 Frame_Info[100]; //定义的文本长度
 		 
/*****************帧固定配置信息**************************************/           
		 Frame_Info[0] = 0xFD ; 			//构造帧头FD
		 Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
		 Frame_Info[2] = len+2; //构造数据区长度的低字节
		 Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令		 		 
		 Frame_Info[4] = 0x01;        //文本编码格式：GBK 
	
/*******************发送帧信息***************************************/		  
     memcpy(&Frame_Info[5], HZdata, len);
		 PrintCom(Frame_Info,5+len); //发送帧配置
}
//IGK 专用语音播报函数
void IGK_Speek(char* fmt,...)  
{  
	u8 buf[100]={0};
	u16 i;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)buf,fmt,ap);
	va_end(ap);
	i=strlen((const char*)buf);//此次发送数据的长度
	speekWihtLen(buf,i);
	osdelay_ms(10);
}


//人声选择
void rensheng(u8 num)
{
	switch(num)
	{
		case 0:IGK_Speek("[m3]小燕为您服务");break;
		case 1:IGK_Speek("[m51]许久为您服务");break;
		case 2:IGK_Speek("[m52]许多为您服务");break;
		case 3:IGK_Speek("[m53]小萍为您服务");break;
		case 4:IGK_Speek("[m54]唐老鸭为您服务");break;
		case 5:IGK_Speek("[m55]许小宝为您服务");break;
		default:break;
	}   
} 
//音量选择
void yinling(u8 num)
{
	IGK_Speek("[v%d]音量为%d",num,num);
}  
//语速选择
void yusu(u8 num)
{
	IGK_Speek("[s%d]语速为%d",num,num);
}
//语调选择
void yudiao(u8 num)
{
	IGK_Speek("[t%d]语调为%d",num,num);
}
