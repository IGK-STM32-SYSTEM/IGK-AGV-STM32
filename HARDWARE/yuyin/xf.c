#include "sys.h"

/**************芯片设置命令*********************/
u8 XFS_StopCom[]={0xFD,0X00,0X01,0X02};		//停止合成
u8 XFS_SuspendCom[]={0XFD,0X00,0X01,0X03};//暂停合成
u8 XFS_RecoverCom[]={0XFD,0X00,0X01,0X04};//恢复合成
u8 XFS_ChackCom[]={0XFD,0X00,0X01,0X21};	//状态查询
u8 XFS_PowerDownCom[]={0XFD,0X00,0X01,0X88};//进入POWER DOWN 状态命令
//人声选择
u8 XIAO_YAN[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x6D,0x33,0x5D};//语音小燕
u8 XU_JIU[]={0xFD,0x00,0x07,0x01,0x01,0x5B,0x6D,0x35,0x31,0x5D};//语音许久
u8 YU_DUO[]={0xFD,0x00,0x07,0x01,0x01,0x5B,0x6D,0x35,0x32,0x5D};//语音许多
u8 XIAO_PING[]={0xFD,0x00,0x07,0x01,0x01,0x5B,0x6D,0x35,0x33,0x5D};//语音小萍
u8 LAO_YA[]={0xFD,0x00,0x07,0x01,0x01,0x5B,0x6D,0x35,0x34,0x5D};//语音唐老鸭
u8 XIAO_BAO[]={0xFD,0x00,0x07,0x01,0x01,0x5B,0x6D,0x35,0x35,0x5D};//语音许小宝
//音量选择
u8 YIN_LIANG0[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x76,0x30,0x5D};//静音
//u8 YIN_LIANG1[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x76,0x31,0x5D};//1
//u8 YIN_LIANG2[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x76,0x32,0x5D};//2
//u8 YIN_LIANG3[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x76,0x33,0x5D};//3
//u8 YIN_LIANG4[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x76,0x34,0x5D};//4
//u8 YIN_LIANG5[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x76,0x35,0x5D};//5
//u8 YIN_LIANG6[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x76,0x36,0x5D};//6
//u8 YIN_LIANG7[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x76,0x37,0x5D};//7
//u8 YIN_LIANG8[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x76,0x38,0x5D};//8
//u8 YIN_LIANG9[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x76,0x39,0x5D};//9

//语速选择
u8 YIN_SU0[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x73,0x30,0x5D};//最慢
u8 YIN_SU1[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x73,0x31,0x5D};//1
u8 YIN_SU2[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x73,0x32,0x5D};//2
u8 YIN_SU3[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x73,0x33,0x5D};//3
u8 YIN_SU4[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x73,0x34,0x5D};//4
u8 YIN_SU5[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x73,0x35,0x5D};//5
u8 YIN_SU6[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x73,0x36,0x5D};//6
u8 YIN_SU7[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x73,0x37,0x5D};//7
u8 YIN_SU8[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x73,0x38,0x5D};//8
u8 YIN_SU9[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x73,0x39,0x5D};//9

//语调选择
u8 YIN_DIAO0[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x74,0x30,0x5D};//最底
u8 YIN_DIAO1[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x74,0x31,0x5D};//1
u8 YIN_DIAO2[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x74,0x32,0x5D};//2
u8 YIN_DIAO3[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x74,0x33,0x5D};//3
u8 YIN_DIAO4[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x74,0x34,0x5D};//4
u8 YIN_DIAO5[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x74,0x35,0x5D};//5
u8 YIN_DIAO6[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x74,0x36,0x5D};//6
u8 YIN_DIAO7[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x74,0x37,0x5D};//7
u8 YIN_DIAO8[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x74,0x38,0x5D};//8
u8 YIN_DIAO9[]={0xFD,0x00,0x06,0x01,0x01,0x5B,0x74,0x39,0x5D};//9

/***********************************************/


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

/***********************************************************
* 名    称： void  main(void)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量 
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。					 
* 调用方法：通过调用已经定义的相关数组进行配置。 
**********************************************************/
void YS_XFS_Set(u8 *Info_data)
{
	u8 Com_Len;
	Com_Len =strlen((const char *)Info_data);
	PrintCom(Info_data,Com_Len);
}
//人声选择
void rensheng(u8 num)
{
	switch(num)
	{
		case 0:PrintCom(XIAO_YAN,9); 	osdelay_ms(10);	speek("你好！我是小燕，很高兴为您服务");break;
		case 1:PrintCom(XU_JIU,10);		osdelay_ms(10);	speek("你好！我是许久，很高兴为您服务！");break;
		case 2:PrintCom(YU_DUO,10);		osdelay_ms(10);	speek("你好！我是许多，很高兴为您服务");break;
		case 3:PrintCom(XIAO_PING,10);	osdelay_ms(10);	speek("你好！我是小萍，很高兴为您服务");break;
		case 4:PrintCom(LAO_YA,10);		osdelay_ms(10);	speek("你好！我是唐老鸭，很高兴为您服务");break;
		case 5:PrintCom(XIAO_BAO,10);	osdelay_ms(10);	speek("你好！我是唐小宝，很高兴为您服务");break;
	}                                                                         
} 
//音量选择
void yinling(u8 num)
{
	YIN_LIANG0[7] = 0x30+num;
	PrintCom(YIN_LIANG0,9);
	osdelay_ms(10);
}  
////设置默认音量
//void set_yinling(u8 num)
//{
//	switch(num)
//	{
//		case 0:PrintCom(YIN_LIANG2,9);osdelay_ms(10);break;
//		case 1:PrintCom(YIN_LIANG5,9);osdelay_ms(10);break;
//		case 2:PrintCom(YIN_LIANG9,9);osdelay_ms(10);break;
//	}                                                                        
//}
//语速选择
void yusu(u8 num)
{
	switch(num)
	{
		case 0:PrintCom(YIN_SU0,9);osdelay_ms(10);speek("这是语速为0的感受");break;
		case 1:PrintCom(YIN_SU1,9);osdelay_ms(10);speek("这是语速为1的感受");break;
		case 2:PrintCom(YIN_SU2,9);osdelay_ms(10);speek("这是语速为2的感受");break;
		case 3:PrintCom(YIN_SU3,9);osdelay_ms(10);speek("这是语速为3的感受");break;
		case 4:PrintCom(YIN_SU4,9);osdelay_ms(10);speek("这是语速为4的感受");break;
		case 5:PrintCom(YIN_SU5,9);osdelay_ms(10);speek("这是语速为5的感受");break;
		case 6:PrintCom(YIN_SU6,9);osdelay_ms(10);speek("这是语速为6的感受");break;
		case 7:PrintCom(YIN_SU7,9);osdelay_ms(10);speek("这是语速为7的感受");break;
		case 8:PrintCom(YIN_SU8,9);osdelay_ms(10);speek("这是语速为8的感受");break;
		case 9:PrintCom(YIN_SU9,9);osdelay_ms(10);speek("这是语速为9的感受");break;
	}                                                                         
}
//语调选择
void yudiao(u8 num)
{
	switch(num)
	{
		case 0:PrintCom(YIN_DIAO0,9);osdelay_ms(10);speek("这是语调为0的感受");break;
		case 1:PrintCom(YIN_DIAO1,9);osdelay_ms(10);speek("这是语调为1的感受");break;
		case 2:PrintCom(YIN_DIAO2,9);osdelay_ms(10);speek("这是语调为2的感受");break;
		case 3:PrintCom(YIN_DIAO3,9);osdelay_ms(10);speek("这是语调为3的感受");break;
		case 4:PrintCom(YIN_DIAO4,9);osdelay_ms(10);speek("这是语调为4的感受");break;
		case 5:PrintCom(YIN_DIAO5,9);osdelay_ms(10);speek("这是语调为5的感受");break;
		case 6:PrintCom(YIN_DIAO6,9);osdelay_ms(10);speek("这是语调为6的感受");break;
		case 7:PrintCom(YIN_DIAO7,9);osdelay_ms(10);speek("这是语调为7的感受");break;
		case 8:PrintCom(YIN_DIAO8,9);osdelay_ms(10);speek("这是语调为8的感受");break;
		case 9:PrintCom(YIN_DIAO9,9);osdelay_ms(10);speek("这是语调为9的感受");break;
	}                                                                         
}

                                        

