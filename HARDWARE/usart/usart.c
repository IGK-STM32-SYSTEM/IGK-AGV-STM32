#include "sys.h"
#include "igk_os.h"

/****************************************************
����:ͨ�ô��ڴ�ӡ�ַ���
˵��:
1.�ڲ��Զ��л�485�շ�����
2.���η��Ͳ��ó���SerialPrintSize��256Byte��
��˾:��������������Ƽ���˾
����:������
*****************************************************/
void IGK_Printf(USART_TypeDef* USARTx ,char* fmt,...)  
{  
	char buf[SerialPrintSize]={0};
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)buf,fmt,ap);
	va_end(ap);
	i=strlen((const char*)buf);//�˴η������ݵĳ���
	//Ϊ485�˿��Զ��л�����
	if(USARTx==USART2)
		PDout(4) = 1;
	if(USARTx==USART3)
		PAout(15) = 1;
	if(USARTx==UART4)
		PAout(2) = 1;
	
	for(j=0;j<i;j++)//ѭ����������
	{
	  while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);  //�ȴ��ϴδ������ 
		USART_SendData(USARTx,(uint8_t)buf[j]); 	 //�������ݵ����� 
	}
}

/****************************************************
����:ͨ�ô��ڴ�ӡ����
˵��:
1.�ڲ��Զ��л�485�շ�����
��˾:��������������Ƽ���˾
����:������
*****************************************************/
void IGK_PrintBuf(USART_TypeDef* USARTx ,char* buf,u8 len)  
{  
	u8 i = 0;
	//Ϊ485�˿��Զ��л�����
	if(USARTx==USART2)
		PDout(4) = 1;
	if(USARTx==USART3)
		PAout(15) = 1;
	if(USARTx==UART4)
		PAout(2) = 1;
	
	for(i=0;i<len;i++)//ѭ����������
	{
	  while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)==RESET);  //�ȴ��ϴδ������ 
		USART_SendData(USARTx,(uint8_t)buf[i]); 	 //�������ݵ����� 
	}
	osdelay_ms(10);
	//Ϊ485�˿��Զ��л�����
	if(USARTx==USART2)
		PDout(4) = 0;
	if(USARTx==USART3)
		PAout(15) = 0;
	if(USARTx==UART4)
		PAout(2) = 0;
}


/****************************************************
����:ͳһ���Դ�ӡ�ӿ�
˵��:�����Ҫ��������,ֻ��Ҫ�滻�ú����ڵ�USART2����
��˾:��������������Ƽ���˾
����:������
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
//��ϵͳʱ��ͻ���
void IGK_SysTimePrintln(char* fmt,...)
{
	char buf[SerialPrintSize]={0};
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)buf,fmt,ap);
	va_end(ap);
	//��ӡϵͳʱ��
	IGK_Struct_DateTime *dt = &IgkAgvOs.OsTime;
	IGK_SysPrintf("[%d:%d:%d:%d]",dt->Hour,dt->Minute,dt->Second,dt->Millisecond);
	//��ӡ��Ϣ
	IGK_Printf(USART2,buf);
	//��ӡ����
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

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//485����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��
	
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
	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);//ע�͵�������дu6_printf��������
	
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
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��

	//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3); //GPIOD8����ΪUSART3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); //GPIOD9����ΪUSART3

	//USART3�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOD8��GPIOD9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PD8��PD9

	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//485����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
    
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
	
	//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;                //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);        //����ָ���Ĳ�����ʼ��VIC�Ĵ�����       
}

void USART5_Configuration(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIODʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//ʹ��USART3ʱ��

	//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //GPIOD8����ΪUSART3
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //GPIOD9����ΪUSART3

	//USART3�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIOD12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PC12
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PD2

	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //�շ�ģʽ
	USART_Init(UART5, &USART_InitStructure); //��ʼ������5
	
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);
	USART_Cmd(UART5, ENABLE);  //ʹ�ܴ���5 
	USART_ClearFlag(UART5, USART_FLAG_TC);
	USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);
	
	//Usart3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//����5�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;                //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);        //����ָ���Ĳ�����ʼ��VIC�Ĵ�����       
}
