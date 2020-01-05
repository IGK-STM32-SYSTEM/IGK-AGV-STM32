#include "sys.h"
#include "igk_os.h"

/****************************************************
方法:通用串口打印字符串
说明:
1.内部自动切换485收发引脚
2.单次发送不得超过SerialPrintSize【256Byte】
公司:西安爱极客网络科技公司
姓名:孙毅明
*****************************************************/
void IGK_Printf(USART_TypeDef* USARTx ,char* fmt,...)  
{  
	char buf[SerialPrintSize]={0};
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)buf,fmt,ap);
	va_end(ap);
	i=strlen((const char*)buf);//此次发送数据的长度
	//为485端口自动切换发送
	if(USARTx==USART2)
		PDout(4) = 1;
	if(USARTx==USART3)
		PAout(15) = 1;
	if(USARTx==UART4)
		PAout(2) = 1;
	
	for(j=0;j<i;j++)//循环发送数据
	{
	  while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);  //等待上次传输完成 
		USART_SendData(USARTx,(uint8_t)buf[j]); 	 //发送数据到串口 
	}
}

/****************************************************
方法:通用串口打印数组
说明:
1.内部自动切换485收发引脚
公司:西安爱极客网络科技公司
姓名:孙毅明
*****************************************************/
void IGK_PrintBuf(USART_TypeDef* USARTx ,char* buf,u8 len)  
{  
	u8 i = 0;
	//为485端口自动切换发送
	if(USARTx==USART2)
		PDout(4) = 1;
	if(USARTx==USART3)
		PAout(15) = 1;
	if(USARTx==UART4)
		PAout(2) = 1;
	
	for(i=0;i<len;i++)//循环发送数据
	{
	  while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);  //等待上次传输完成 
		USART_SendData(USARTx,(uint8_t)buf[i]); 	 //发送数据到串口 
	}
	osdelay_ms(10);
	//为485端口自动切换接收
	if(USARTx==USART2)
		PDout(4) = 0;
	if(USARTx==USART3)
		PAout(15) = 0;
	if(USARTx==UART4)
		PAout(2) = 0;
}


/****************************************************
方法:统一调试打印接口
说明:如果需要更换串口,只需要替换该函数内的USART2即可
公司:西安爱极客网络科技公司
姓名:孙毅明
*****************************************************/
void IGK_SysPrintf(char* fmt,...)
{
	char buf[SerialPrintSize]={0};
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)buf,fmt,ap);
	va_end(ap);
	IGK_Printf(USART2,buf);
}
//带系统时间和换行
void IGK_SysTimePrintln(char* fmt,...)
{
	char buf[SerialPrintSize]={0};
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)buf,fmt,ap);
	va_end(ap);
	//打印系统时间
	IGK_Struct_DateTime *dt = &IgkAgvOs.OsTime;
	IGK_SysPrintf("[%d:%d:%d:%d]",dt->Hour,dt->Minute,dt->Second,dt->Millisecond);
	//打印消息
	IGK_Printf(USART2,buf);
	//打印换行
	IGK_SysPrintf("\r\n\r\n");
}

void USART1_Configuration(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = bound  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);	
	USART_Cmd(USART1, ENABLE);  
	USART_ClearFlag(USART1, USART_FLAG_TC);
}
void USART2_Configuration(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//485方向
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
	
	USART_InitStructure.USART_BaudRate            = bound  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);	
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART2, ENABLE);  
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void USART6_Configuration(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate            = bound  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART6, &USART_InitStructure);
	USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);	
	USART_Cmd(USART6, ENABLE);  
	USART_ClearFlag(USART6, USART_FLAG_TC);
	USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);//注释掉，用自写u6_printf函数发送
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void USART4_Configuration(u32 bound)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	
	USART_InitStructure.USART_BaudRate            = bound  ;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);	
	USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);
	USART_Cmd(UART4, ENABLE);  
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);
    USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART3_Configuration(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟

	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3); //GPIOD8复用为USART3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); //GPIOD9复用为USART3

	//USART3端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOD8与GPIOD9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PD8，PD9

	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//485方向
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
    
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART3, ENABLE);  //使能串口1 
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	
	//Usart3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;                //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);        //根据指定的参数初始化VIC寄存器、       
}

void USART5_Configuration(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOD时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//使能USART3时钟

	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //GPIOD8复用为USART3
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //GPIOD9复用为USART3

	//USART3端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIOD12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PC12
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PD2

	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //收发模式
	USART_Init(UART5, &USART_InitStructure); //初始化串口5
	
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);
	USART_Cmd(UART5, ENABLE);  //使能串口5 
	USART_ClearFlag(UART5, USART_FLAG_TC);
	USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);
	
	//Usart3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//串口5中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;                //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);        //根据指定的参数初始化VIC寄存器、       
}
