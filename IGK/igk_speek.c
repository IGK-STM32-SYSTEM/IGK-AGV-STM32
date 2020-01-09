#include "sys.h"
/**************оƬ��������*********************/
u8 XFS_StopCom[]={0xFD,0X00,0X01,0X02};		//ֹͣ�ϳ�
u8 XFS_SuspendCom[]={0XFD,0X00,0X01,0X03};//��ͣ�ϳ�
u8 XFS_RecoverCom[]={0XFD,0X00,0X01,0X04};//�ָ��ϳ�
u8 XFS_ChackCom[]={0XFD,0X00,0X01,0X21};	//״̬��ѯ
u8 XFS_PowerDownCom[]={0XFD,0X00,0X01,0X88};//����POWER DOWN ״̬����
//����ѡ��
u8 RENSHENG[]={3,51,52,53,54,55};

/************************************************************************
���������� ���ڷ����ַ�������
��ڲ����� 	*DAT���ַ���ָ��
�� �� ֵ�� none
����˵���� API ���ⲿʹ�ã�ֱ�ۣ�
**************************************************************************/
void PrintCom(u8 *DAT,u16 len)
{
	u16 i;
	if(1)//���������Ŀ���
	{
		/* TC TXE ��־λ�ڸ�λ��ʱ����1 */  
		/*��һ���ȡSR�Ĵ������ڶ���дDR�Ĵ���*/  
		/* ����������������״η��͵ĵ�һ���ַ��ᶪʧ */ 
//		USART_GetFlagStatus(USART1, USART_FLAG_TC);
		for(i=0;i<len;i++)
		{
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			USART_SendData(USART1, *DAT++);
			
		}	
	}
	else 
	{
		;
	}
}


/***********************************************************
* ��    �ƣ�  YS-XFS5051 �ı��ϳɺ���
* ��    �ܣ�  ���ͺϳ��ı���XFS5051оƬ���кϳɲ���
* ��ڲ�����  *HZdata:�ı�ָ����� 
* ���ڲ�����
* ˵    ���� ������ֻ�����ı��ϳɣ��߱���������ѡ��Ĭ�ϲ�����9600bps��					 
* ���÷��������� SYN_FrameInfo���������Ƶ��ӡ�����
**********************************************************/
void speek(u8 *HZdata)
{
/****************��Ҫ���͵��ı�**********************************/ 
		u8 Frame_Info[100]; //������ı�����
		u16 HZ_Length;  

		HZ_Length =strlen((const char *)HZdata); 			//��Ҫ�����ı��ĳ���
 		 
/*****************֡�̶�������Ϣ**************************************/           
		 Frame_Info[0] = 0xFD ; 			//����֡ͷFD
		 Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
		 Frame_Info[2] = HZ_Length+2; //�������������ȵĵ��ֽ�
		 Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������		 		 
		 Frame_Info[4] = 0x01;        //�ı������ʽ��GBK 
	
/*******************����֡��Ϣ***************************************/		  
     memcpy(&Frame_Info[5], HZdata, HZ_Length);
		 PrintCom(Frame_Info,5+HZ_Length); //����֡����
}

void speekWihtLen(u8 *HZdata,u16 len)
{
/****************��Ҫ���͵��ı�**********************************/ 
		u8 Frame_Info[100]; //������ı�����
 		 
/*****************֡�̶�������Ϣ**************************************/           
		 Frame_Info[0] = 0xFD ; 			//����֡ͷFD
		 Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
		 Frame_Info[2] = len+2; //�������������ȵĵ��ֽ�
		 Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������		 		 
		 Frame_Info[4] = 0x01;        //�ı������ʽ��GBK 
	
/*******************����֡��Ϣ***************************************/		  
     memcpy(&Frame_Info[5], HZdata, len);
		 PrintCom(Frame_Info,5+len); //����֡����
}
//IGK ר��������������
void IGK_Speek(char* fmt,...)  
{  
	u8 buf[100]={0};
	u16 i;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)buf,fmt,ap);
	va_end(ap);
	i=strlen((const char*)buf);//�˴η������ݵĳ���
	speekWihtLen(buf,i);
	osdelay_ms(10);
}


//����ѡ��
void rensheng(u8 num)
{
	switch(num)
	{
		case 0:IGK_Speek("[m3]С��Ϊ������");break;
		case 1:IGK_Speek("[m51]���Ϊ������");break;
		case 2:IGK_Speek("[m52]���Ϊ������");break;
		case 3:IGK_Speek("[m53]СƼΪ������");break;
		case 4:IGK_Speek("[m54]����ѼΪ������");break;
		case 5:IGK_Speek("[m55]��С��Ϊ������");break;
		default:break;
	}   
} 
//����ѡ��
void yinling(u8 num)
{
	IGK_Speek("[v%d]����Ϊ%d",num,num);
}  
//����ѡ��
void yusu(u8 num)
{
	IGK_Speek("[s%d]����Ϊ%d",num,num);
}
//���ѡ��
void yudiao(u8 num)
{
	IGK_Speek("[t%d]���Ϊ%d",num,num);
}
