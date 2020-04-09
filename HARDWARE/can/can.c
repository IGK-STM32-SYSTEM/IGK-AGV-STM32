#include "sys.h"
#include "motec.h"
//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ42M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//������Ϊ:42M/((6+7+1)*6)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��; 

u8 can_Sebuf[4]={0x01,0x01,0x55,0x01};
u8 can_Rebuf[2]={0xff,0xff};
u8 CAN1_Sebuf[5]={1,0,1,0,0x55};

u8 OutPut[10]={1,1,1,1,1,1,1,1,1,1};//can�����Ӧ�ڼ̵������ʮλ�Ĵ���
u8 Input[12]={1,1,1,1,1,1,1,1,1,1,1,1}; //can�����Ӧ�ڼ̵������ʮһλ�Ĵ���

u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
  	GPIO_InitTypeDef GPIO_InitStructure; 
		CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   	NVIC_InitTypeDef  NVIC_InitStructure;
    //ʹ�����ʱ��
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PORTAʱ��	                   											 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
    //��ʼ��GPIO
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PA11,PA12
	
	  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_CAN1); //GPIOA12����ΪCAN1
	  
  	//CAN��Ԫ����
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ   
  	CAN_InitStructure.CAN_ABOM=DISABLE;	//����Զ����߹���	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
  	CAN_InitStructure.CAN_NART=ENABLE;	//��ֹ�����Զ����� 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
  	CAN_InitStructure.CAN_Mode= mode;	 //ģʽ���� 
  	CAN_InitStructure.CAN_SJW=tsjw;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // ��ʼ��CAN1 
    
		//���ù�����
		CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
		
		CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	return 0;
}   
 

u16 bit8_huan(u16 temp_bit)
{
    u8 bit_num = 8;						//8λ����ת��
    u8 temp_i=0;
    u16 temp_out=0;

    for(temp_i=0; temp_i<bit_num; temp_i++)
    {
        temp_out |= ((temp_bit>>temp_i)&0x01)<<((bit_num-1)-temp_i);
    }
    return temp_out;
}

u16 bit16_huan(u16 temp_bit)
{
    u8 bit_num = 16;						//16λ����ת��
    u8 temp_i=0;
    u16 temp_out=0;

    for(temp_i=0; temp_i<bit_num; temp_i++)
    {
        temp_out |= ((temp_bit>>temp_i)&0x01)<<((bit_num-1)-temp_i);
    }
    return temp_out;
}

//�жϷ�����		
void CAN1_RX0_IRQHandler(void)
{
CanRxMsg RxMessage;
	int i=0;
	//ת���ֲ�
  CAN_Receive(CAN1, 0, &RxMessage);
	//�̵�����չ��
	if(RxMessage.StdId==12)
	{
		//�̵�����Ĭ��״̬����
		for(i=0;i<8;i++)
		{
			Input[i] = !(((RxMessage.Data[0]^0xff)>>i)&0x01);
		}
		for(i=0;i<4;i++)
		{
			Input[i+8] = !(((RxMessage.Data[1]^0xff)>>i)&0x01);
		}
		if(RxMessage.Data[2]==0x55)//�̵����帴λʱ�Ĵ���
		{
			CAN1_Send_Msg(CAN1_Sebuf,4);
		}		
	}
	//ң����
	if(RxMessage.StdId==15)
	{
		IgkSystem.YaoGan.x = RxMessage.Data[0];
		IgkSystem.YaoGan.y = RxMessage.Data[1];
		IgkSystem.YaoGan.z = RxMessage.Data[2];
		IgkSystem.YaoGan.key = RxMessage.Data[3]==1?Enum_KeyUp:Enum_KeyDown;
	}
	else
	//ǰ�ŵ���dir:�ֲ淽��(0��ֱ�У�1����ֲ棬2���ҷֲ�)
	if(RxMessage.StdId==6)
	{	
		u8 dir=0;
		if(IgkSystem.Action == Enum_ZhiXing)
			dir = 0;
		else
		if(IgkSystem.Action == Enum_ZuoFenCha)
			dir = 2;
		if(IgkSystem.Action == Enum_YouFenCha)
			dir = 1;
		IgkSystem.QianCiDaoHang = CdhFind(16,bit16_huan(~(RxMessage.Data[1]|(RxMessage.Data[0]<<8))),dir);
	}
	else
	//��ŵ���
	if(RxMessage.StdId==12)
	{
		u8 dir=0;
		if(IgkSystem.Action == Enum_ZhiXing)
			dir = 0;
		else
		if(IgkSystem.Action == Enum_ZuoFenCha)
			dir = 1;
		if(IgkSystem.Action == Enum_YouFenCha)
			dir = 2;
		IgkSystem.HouCiDaoHang = CdhFind(16,~(RxMessage.Data[1]|(RxMessage.Data[0]<<8)),dir);
	}
	else
	//RFID
	if(RxMessage.StdId==16)
	{
		*IgkSystem.RFID = (RxMessage.Data[1]<<8)|RxMessage.Data[0];
	}
}

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x10;	 // ��׼��ʶ��Ϊ0
  TxMessage.ExtId=0x10;	 // ������չ��ʾ����29λ��
  TxMessage.IDE=0;		  // ʹ����չ��ʶ��
//  TxMessage.RTR=0;		  // ��Ϣ����Ϊ����֡��һ֡8λ
  TxMessage.DLC=len;							 // ������֡��Ϣ
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // ��һ֡��Ϣ          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
  return 0;		
}

//������ר��ȫ��CAN1���ͺ���
u8 Igk_CAN1_Send(u32 id,u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=id;	 // ��׼��ʶ��Ϊ0
  TxMessage.ExtId=0;	 // ������չ��ʾ����29λ��
  TxMessage.IDE=0;		  // ʹ����չ��ʶ��
//  TxMessage.RTR=0;		  // ��Ϣ����Ϊ����֡��һ֡8λ
  TxMessage.DLC=len;							 // ������֡��Ϣ
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // ��һ֡��Ϣ          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
  return 0;		

}
//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
    for(i=0;i<RxMessage.DLC;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}


u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN2_RX0_INT_ENABLE
    NVIC_InitTypeDef  NVIC_InitStructure;
#endif
    //ʹ�����ʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��PORTBʱ��

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//ʹ��CAN2ʱ��

    //��ʼ��GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PB5,PB6

    //���Ÿ���ӳ������
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_CAN2); //GPIOB5����ΪCAN2
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_CAN2); //GPIOB6����ΪCAN2

    //CAN��Ԫ����
    CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ
    CAN_InitStructure.CAN_ABOM=ENABLE;	//����Զ����߹���
    CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
    CAN_InitStructure.CAN_NART=DISABLE;	//�ر�--��ֹ�����Զ�����
    CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�
    CAN_InitStructure.CAN_TXFP=DISABLE;	//�ɶ���˳�����--���ȼ��ɱ��ı�ʶ������
    CAN_InitStructure.CAN_Mode= mode;	 //ģʽ����
    CAN_InitStructure.CAN_SJW=tsjw;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
    CAN_InitStructure.CAN_BS1=tbs1; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
    CAN_InitStructure.CAN_BS2=tbs2;//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
    CAN_InitStructure.CAN_Prescaler=brp;  //��Ƶϵ��(Fdiv)Ϊbrp+1
    CAN_Init(CAN2, &CAN_InitStructure);   // ��ʼ��CAN1

    //���ù�����
    CAN_FilterInitStructure.CAN_FilterNumber=14;	  //������14
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ
    CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
    CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������14
    CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��

#if CAN2_RX0_INT_ENABLE

    CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.

    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            // �����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
    return 0;
}   

//�жϷ�����		
void CAN2_RX0_IRQHandler(void)
{
//    CAN_Receive(CAN2, 0, &RxMessage);
    
    	CanRxMsg RxMessage;
	
#if SYSTEM_SUPPORT_OS 	
	OSIntEnter();    
#endif	
	
	
  CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);  
	if(RxMessage.StdId == 0x701)			//����
	{
		if(RxMessage.Data[0] == 0x05)
		{
			g_motec_init.zijian_enable = g_motec_init.init_delay_val; 
		}
        g_motec_init.zijian_shangdian = g_motec_init.init_delay_val;
	}  	
	else
	if(RxMessage.StdId == 0x702)			//����
	{
		if(RxMessage.Data[0] == 0x05)
		{
			g_motec_init.zijian_enable = g_motec_init.init_delay_val;
		}
        g_motec_init.zijian_shangdian = g_motec_init.init_delay_val;
	} 
	else
	if(RxMessage.StdId == 0x181)			//����ʵ���ٶȺͱ�����ֵ
	{
//		AgvSystem.RealSpeedLeft = -(s16)(RxMessage.Data[2]|(RxMessage.Data[3]<<8));
//		AgvSystem.LeftEncode = -(s32)(RxMessage.Data[4]|(RxMessage.Data[5]<<8)|(RxMessage.Data[6]<<16)|(RxMessage.Data[7]<<24));
	} 		
	else
	if(RxMessage.StdId == 0x182)			//����ʵ���ٶȺͱ�����ֵ
	{
//		AgvSystem.RealSpeedRight = (s16)(RxMessage.Data[2]|(RxMessage.Data[3]<<8));
//		AgvSystem.RightEncode = (s32)(RxMessage.Data[4]|(RxMessage.Data[5]<<8)|(RxMessage.Data[6]<<16)|(RxMessage.Data[7]<<24));
	} 		

#if SYSTEM_SUPPORT_OS 	
	OSIntExit();  											 
#endif  	
	
    
}

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//����,ʧ��;
u8 CAN2_Send_Msg(u16 id ,char* msg,u8 len)
{	
    u8 mbox;
    u16 i=0;
    CanTxMsg TxMessage;
    TxMessage.StdId=id;	 // ��׼��ʶ��Ϊ0
    TxMessage.ExtId=0;	 // ������չ��ʾ����29λ��
    TxMessage.IDE=0;		  // ʹ����չ��ʶ��
    TxMessage.RTR=0;		  // ��Ϣ����Ϊ����֡��һ֡8λ
    TxMessage.DLC=len;							 // ������֡��Ϣ
    for(i=0; i<len; i++)
        TxMessage.Data[i]=msg[i];				 // ��һ֡��Ϣ
    mbox= CAN_Transmit(CAN2, &TxMessage);
    i=0;
    while((CAN_TransmitStatus(CAN2, mbox)!=CAN_TxStatus_Ok)&&(i<0XFFF))i++;	//�ȴ����ͽ���
    if(i>=0XFFF)return 1;
    return 0;

}
//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
CanRxMsg CAN2_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
	memset(&RxMessage,0,sizeof(RxMessage));//���ָ����ָ��ַ������
    if( CAN_MessagePending(CAN2,CAN_FIFO0)==0)return RxMessage;		//û�н��յ�����,ֱ���˳� 
    CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);//��ȡ����	
    for(i=0;i<RxMessage.DLC;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage;	
}





