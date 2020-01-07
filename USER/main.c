#include "sys.h"
#include "igk_dfs.h"
#include "igk_map.h"
#include "igk_ucos.h"

int main(void)
{
//	OS_ERR err;
//	CPU_SR_ALLOC();

	delay_init(168);  	//ʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	LED_Init();         //LED��ʼ��
	DMA_adc1_Init();
	DMA_Uart4_Init();
	DMA_Uart2_Init();
	DMA_Uart6_Init();
	
	DMA_Uart3_Init();
	DMA_Uart5_Init();
	ADC1_Configuration();
	USART1_Configuration(9600);
	USART4_Configuration(115200);
	USART6_Configuration(115200);
	USART2_Configuration(115200);//485-2�ӿ������
	USART3_Configuration(115200);//485-3�ӵ�����Ϣ��ӡ
	USART5_Configuration(115200);
//	TIM4_PWM_Init(1000 - 1, 8 - 1);	//84M/8=10Mhz?????,????1000,??PWM??? 10M/50000=200hz. //500-10   20hz-1000hz
	CAN1_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN1��ʼ����ͨģʽ,������500Kbps
	CAN2_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN2��ʼ����ͨģʽ,������500Kbps
	PID_Init();
	TIM2_Int_Init(5 - 1, 84 - 1);			//Tout=((arr+1)*(psc+1))/Ft us.  5us
//	IWDG_Init(4, 500);
	SPI1_Init();
  TLC5620_Init();
	
	//ָ�����ͱ���ָ��Ĭ�ϵ�ַ
	IgkAgvOs.TongXin.NowRfid = &PLC_Data[41];
	IgkAgvOs.Speed = &PLC_Data[61];

	
	//�趨Ĭ�ϲ���
	IgkAgvOs.OsTime.Hour = 0;
	IgkAgvOs.OsTime.Minute = 0;
	IgkAgvOs.OsTime.Second = 0;
	IgkAgvOs.OsTime.Millisecond = 0;
	
	IgkAgvOs.WorkMode = Enum_LocalAuto;//�Զ�ģʽ
	IgkAgvOs.Dir = Enum_QianJin;//ǰ������
	*IgkAgvOs.TongXin.NowRfid = 1;
	IgkAgvOs.RunOrStop = Enum_Stop;//ֹͣ״̬
	*IgkAgvOs.Speed = 50;//�ٶ�50%;	
	
	IGK_SysTimePrintln("���ڳ�ʼ��...");
	//��ʼ��Flash
	W25QXX_Init();			//W25QXX��ʼ��
	while(W25QXX_ReadID()!=W25Q128)//��ⲻ��W25Q128��LED��˸
	{
		delay(0,0,0,100);
	}

	//ϵͳ��������������ʾ
	for(u8 i=0;i<4;i++)
	{
		BEEP =~BEEP;
		osdelay_ms(60);
	}
	IGK_SysTimePrintln("Flash Ok!");	
	//��ʼ���ڴ��[����ԭ���ڴ����]
	my_mem_init(SRAMIN);
	IGK_SysTimePrintln("������̬�ڴ棺%dKByte!",MEM1_MAX_SIZE/1024);	
	//��ʼ��ucos
	UCOS_Init();
	while(1);
}

//ģʽ
void task1(void *p_arg)
{
	p_arg = p_arg;
	osdelay_ms(10);
	while(1)
	{
		//���ҡ�˰���,�л�ģʽ
		if(IgkAgvOs.YaoGan.key==KeyDown)
		{
			delay(0, 0, 1, 0);
			if(IgkAgvOs.YaoGan.key==KeyDown)
			{
				if(IgkAgvOs.WorkMode == Enum_LocalManual)
				{
					IgkAgvOs.WorkMode = Enum_LocalAuto;
					IGK_Speek("�Զ�ģʽ");
				}
				else
				{
					IgkAgvOs.WorkMode = Enum_LocalManual;
					IGK_Speek("�ֶ�ģʽ");
				}
			}
		}
		//�ֶ�����ģʽ
		if(IgkAgvOs.WorkMode == Enum_LocalManual)
		{
			//ֱ�ӽ��ٶȸ�ֵ��������
			//�жϵ�ǰ����
			if(abs(IgkAgvOs.YaoGan.y)>0)//ǰ������
			{
				*IgkAgvOs.Speed = abs(IgkAgvOs.YaoGan.y)*1;
				//ת���ٶ�
				s8 speed = IgkAgvOs.YaoGan.z*0.5;
				if(IgkAgvOs.YaoGan.y>0)
				{
					if(speed >= 0)
						DriverQinJinSpeed(*IgkAgvOs.Speed-speed,*IgkAgvOs.Speed);
					else
						DriverQinJinSpeed(*IgkAgvOs.Speed,*IgkAgvOs.Speed+speed);
				}
				else
				if(IgkAgvOs.YaoGan.y<0)
				{
					if(speed >= 0)
						DriverHouTuiSpeed(*IgkAgvOs.Speed,*IgkAgvOs.Speed-speed);
					else
						DriverHouTuiSpeed(*IgkAgvOs.Speed+speed,*IgkAgvOs.Speed);
				}
			}
			else
			if(abs(IgkAgvOs.YaoGan.z)>0)//������ת
			{
				*IgkAgvOs.Speed = abs(IgkAgvOs.YaoGan.z)*0.8;
				if(IgkAgvOs.YaoGan.z<0)
					DriverZuoXuan(*IgkAgvOs.Speed);
				else
				if(IgkAgvOs.YaoGan.z>0)
					DriverYouXuan(*IgkAgvOs.Speed);
			}
			else // ֹͣ
				DriverTingZhi();
		}
		delay(0, 0, 0, 5);
	}
}
//�Զ�����ģʽ
void Auto_task(void *p_arg)
{
	p_arg = p_arg;
	//��ʼ��PID
	PID_Init();
	while(1)
	{
		//�Զ�ģʽ
		if(IgkAgvOs.WorkMode == Enum_LocalAuto)
		{
			//ǰ��
			if(IgkAgvOs.Dir == Enum_QianJin)
			{ 
				if(IgkAgvOs.RunOrStop == Enum_Stop)
					DriverTingZhi();
				else
				{
					switch(IgkAgvOs.Action)
					{
						case Enum_ZuoXuan://����
							DriverZuoXuan(50);
							break;
						case Enum_YouXuan://����
							DriverYouXuan(50);
							break;
						case Enum_ZuoFenCha://��ֲ�
						case Enum_YouFenCha://�ҷֲ�
						case Enum_ZhiXing:  //ֱ��
							if(IgkAgvOs.QianCiDaoHang.Error == 0)
							{
								//PID��̬����ѭ��
								float Inc = PosPIDCalc(IgkAgvOs.QianCiDaoHang.Distance);
								//�����ٶ�
								s16 s1 = *IgkAgvOs.Speed+Inc;
								s16 s2 = *IgkAgvOs.Speed-Inc;
								//�ж��߽�
								s1 = s1>100?100:s1;
								s1 = s1<0?0:s1;
								s2 = s2>100?100:s2;
								s2 = s2<0?0:s2;
								DriverQinJinSpeed(s1,s2);
							}
							//����ֹͣ
							else
							{
								DriverTingZhi();
							}
							break;
						default:
							break;
					}
				}
			}
			else
			//����
			if(IgkAgvOs.Dir == Enum_HouTui)
			{
				if(IgkAgvOs.RunOrStop == Enum_Stop)
					DriverTingZhi();
				else
				{
					switch(IgkAgvOs.Action)
					{
						case Enum_ZuoXuan://����
							DriverZuoXuan(50);
							break;
						case Enum_YouXuan://����
							DriverYouXuan(50);
							break;
						case Enum_ZuoFenCha://��ֲ�
						case Enum_YouFenCha://�ҷֲ�
						case Enum_ZhiXing:  //ֱ��
							if(IgkAgvOs.HouCiDaoHang.Error == 0)
							{
								//PID��̬����ѭ��
								float Inc = PosPIDCalc(-IgkAgvOs.HouCiDaoHang.Distance);
								//�����ٶ�
								s16 s1 = *IgkAgvOs.Speed + Inc;
								s16 s2 = *IgkAgvOs.Speed - Inc;
								//�ж��߽�
								s1 = s1>100?100:s1;
								s1 = s1<0?0:s1;
								s2 = s2>100?100:s2;
								s2 = s2<0?0:s2;
								DriverHouTuiSpeed(s1,s2);
							}
							//����ֹͣ
							else
							{
								DriverTingZhi();
							}
							break;
						default:
							break;
					}
				}
			}
		}
		delay(0, 0, 0, 10);
	}
}

//����Ŀ��λ��
u16 target = 1;
void Manual_task(void *p_arg)
{
	p_arg = p_arg;
	osdelay_s(1);
	yinling(1);
	IGK_Speek("ϵͳ�Լ����");
	while(1)
	{
		/*1.-----------------�ȴ�ִ���ź�--------------------------*/
		IGK_SysTimePrintln("�ȴ�����!");
		while(PLC_Data[43]!=1)
		{
			osdelay_ms(10);
		}
		//���ִ���ź�
		PLC_Data[43] = 0;
		/*2.-----------------�ж�Ŀ���ǩ�Ϸ���--------------------------*/
		if(PLC_Data[40]==*IgkAgvOs.TongXin.NowRfid)
		{
			IGK_Speek("Ŀ���ǩ�͵�ǰ��ǩ��ͬ,������ɣ�");
			IGK_SysTimePrintln("Ŀ���ǩ�͵�ǰ��ǩ��ͬ,������ɣ�");
			//����
			continue;
		}
		else
		if(PLC_Data[40]==0)
		{
			IGK_Speek("Ŀ���ǩΪ0,�����Ƿ���");
			IGK_SysTimePrintln("Ŀ���ǩΪ0,�����Ƿ���");
			//����
			continue;
		}
		else
		if(PLC_Data[40] > NodeMaxNum)
		{
			IGK_Speek("Ŀ���ǩ���ܴ���%d,�����Ƿ���",NodeMaxNum);
			IGK_SysTimePrintln("Ŀ���ǩ���ܴ���%d,�����Ƿ���",NodeMaxNum);
			//����
			continue;
		}	
		//����Ŀ��λ�õ���ʱ����
		target = PLC_Data[40];
		//����·��
		FindRoute(*IgkAgvOs.TongXin.NowRfid,target);
		IGK_SysTimePrintln("·���������!");

		//����Ŀ��λ��
		IGK_Speek("׼��ǰ��%d�ű�ǩ",target);
		osdelay_s(3);
		if(BestPath.PathTotal==0)
		{
			IGK_SysPrintf("δ�ҵ���Ч·����\r\n\r\n");
			IGK_Speek("δ�ҵ���Ч·��,����������");
		}
		else
		{
			IGK_SysPrintf("���ҵ�%d��·����\r\n\r\n",BestPath.PathTotal);
			IGK_Speek("���ҵ�%d��·����",BestPath.PathTotal);
			osdelay_s(2);
		/*3.-----------------ִ��·��--------------------------*/
			//����
			IgkAgvOs.RunOrStop = Enum_Run;
			for(int i=0;i < BestPath.NodeCount;i++)
			{
				//����Ĭ���ٶ�
				*IgkAgvOs.Speed = 50;
				u16 nodeId = BestPath.NodeList[i];
				//��ȡ��Ӧվ��ĵ�ͼ
				StaionMapStruct mapStruct;
				ReadToMapStruct(nodeId,&mapStruct);
				IGK_SysPrintf("��ǩ��:[%d]",nodeId);
				//��һ��λ�ø��µ�����
				PLC_Data[42] = nodeId;
				//�ȴ���ǩ,��㲻�õȴ�
				if(i!=0)
				{
						IGK_Speek("�ȴ�%d�ű�ǩ!",nodeId);
						//osdelay_s(2);
						while(*IgkAgvOs.TongXin.NowRfid != nodeId )
						{
							osdelay_ms(5);
						}
						if(i == BestPath.NodeCount-1)
						{
							//�����յ�
							break;
						}
				}
				//ֱ��վ�㷽�򣬶���
				for(int j=0;j<StationMapType;j++)
				{
					if(mapStruct.Stop[j] == BestPath.NodeList[i+1])
					{
						//����
						if(mapStruct.Dir[j] == Enum_QianJin)
						{
							if(IgkAgvOs.Dir != Enum_QianJin)
							{
								//ֹͣ
								IgkAgvOs.RunOrStop = Enum_Stop;
								DriverTingZhi();
								//�л�����
								IgkAgvOs.Dir = Enum_QianJin;
								//�ȴ�
								osdelay_ms(500);
							}
							//��ӡ����
							IGK_SysPrintf("ǰ��-");
							//��������
							IGK_Speek("ǰ��");					
						}
						else
						if(mapStruct.Dir[j] == Enum_HouTui)
						{
							if(IgkAgvOs.Dir != Enum_HouTui)
							{
								//ֹͣ
								IgkAgvOs.RunOrStop = Enum_Stop;
								DriverTingZhi();
								//�л�����
								IgkAgvOs.Dir = Enum_HouTui;
								//�ȴ�
								osdelay_ms(500);
							}
							//��ӡ����
							IGK_SysPrintf("����-");
							//��������
							IGK_Speek("����");
						}
						else
						if(mapStruct.Dir[j] == Enum_PingYi)
						{
							if(IgkAgvOs.Dir != Enum_ZuoFenCha)
							{
								osdelay_ms(300);
								IGK_SysPrintf("ƽ��-");
								IGK_Speek("ƽ��");
							}
						}
						
						//����
						if(mapStruct.Action[j] == Enum_ZuoFenCha)
						{
							IgkAgvOs.Action = Enum_ZuoFenCha;
							IGK_SysPrintf("��ֲ�-");
							IGK_Speek("��ֲ�");
						}
						else
						if(mapStruct.Action[j] == Enum_YouFenCha)
						{
							IgkAgvOs.Action = Enum_YouFenCha;
							IGK_SysPrintf("�ҷֲ�-");
							IGK_Speek("�ҷֲ�");
						}
						else
						if(mapStruct.Action[j] == Enum_ZhiXing)
						{
							IgkAgvOs.Action = Enum_ZhiXing;
							IGK_SysPrintf("ֱ��-");
							IGK_Speek("ֱ��");
						}
						else
						if(mapStruct.Action[j] == Enum_ZuoXuan)
						{
							//ͣ��
							IgkAgvOs.RunOrStop = Enum_Stop;
							DriverTingZhi();
							IGK_Speek("����%d��",mapStruct.Angle[j]);
							//ԭ����ת
							osdelay_ms(200);
							//����
							IgkAgvOs.Action = Enum_ZuoXuan;
							//����
							IgkAgvOs.RunOrStop = Enum_Run;
							//���ݷ���ѡ��ŵ���������
							Fencha_struct * PCiDaoHang;
							if(IgkAgvOs.Dir == Enum_QianJin)
								PCiDaoHang = &IgkAgvOs.QianCiDaoHang;
							else
								PCiDaoHang = &IgkAgvOs.HouCiDaoHang;
							//�ȴ��뿪����
							while(PCiDaoHang->Error==0)
								osdelay_ms(10);
							//�ȴ���⵽����[���������㲢�Ҿ���]
							while(PCiDaoHang->Num <3)
								osdelay_ms(10);
							if(mapStruct.Angle[j]==180)
							{
								//�ȴ��뿪����
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//�ȴ���⵽����[���������㲢�Ҿ���]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
							}
							else
							if(mapStruct.Angle[j]==270)
							{
								//�ȴ��뿪����
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//�ȴ���⵽����[���������㲢�Ҿ���]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
								//�ȴ��뿪����
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//�ȴ���⵽����[���������㲢�Ҿ���]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
							}
							//ֹͣ
							IgkAgvOs.RunOrStop = Enum_Stop;
							DriverTingZhi();
							osdelay_ms(200);
							//�л�ֱ��
							IgkAgvOs.Action = Enum_ZhiXing;
							//�л�Ϊ�˶�״̬
							osdelay_ms(200);
							IgkAgvOs.RunOrStop = Enum_Run;
							IGK_SysPrintf("����[%d]��-",mapStruct.Angle[j]);
						}
						else
						if(mapStruct.Action[j] == Enum_YouXuan)
						{
							//ͣ��
							IgkAgvOs.RunOrStop = Enum_Stop;
							DriverTingZhi();
							IGK_Speek("����%d��",mapStruct.Angle[j]);
							//ԭ����ת
							osdelay_ms(200);
							//����
							IgkAgvOs.Action = Enum_YouXuan;
							//����
							IgkAgvOs.RunOrStop = Enum_Run;
							//���ݷ���ѡ��ŵ���������
							Fencha_struct * PCiDaoHang;
							if(IgkAgvOs.Dir == Enum_QianJin)
								PCiDaoHang = &IgkAgvOs.QianCiDaoHang;
							else
								PCiDaoHang = &IgkAgvOs.HouCiDaoHang;
							//�ȴ��뿪����
							while(PCiDaoHang->Error==0)
								osdelay_ms(10);
							//�ȴ���⵽����[���������㲢�Ҿ���]
							while(PCiDaoHang->Num <3)
								osdelay_ms(10);
							if(mapStruct.Angle[j]==180)
							{
								//�ȴ��뿪����
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//�ȴ���⵽����[���������㲢�Ҿ���]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
							}
							else
							if(mapStruct.Angle[j]==270)
							{
								//�ȴ��뿪����
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//�ȴ���⵽����[���������㲢�Ҿ���]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
								//�ȴ��뿪����
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//�ȴ���⵽����[���������㲢�Ҿ���]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
							}
							//ֹͣ
							IgkAgvOs.RunOrStop = Enum_Stop;
							DriverTingZhi();
							osdelay_ms(200);
							//�л�ֱ��
							IgkAgvOs.Action = Enum_ZhiXing;
							//�л�Ϊ�˶�״̬
							osdelay_ms(200);
							//ԭ����ת
							IGK_SysPrintf("����[%d]��-",mapStruct.Angle[j]);
						}
						IgkAgvOs.RunOrStop = Enum_Run;
						//����
						IGK_SysPrintf("[%d]\r\n\r\n",mapStruct.Stop[j]);
						//�ҵ���Ӧվ�㣬����ѭ��
						break;
					}
				}
			}
			//����Ŀ��λ��,ͣ��
			IgkAgvOs.RunOrStop = Enum_Stop;
			IGK_Speek("�����յ㣬�������");
			IGK_SysTimePrintln("�����յ㣬�������");
		}
		delay(0, 0, 0, 10); //��ʱ10ms
	}
}
//Modbusͬ������
void PID_task(void *p_arg)
{
	while(1)
	{
		ReadWriteMap(p_arg);
		delay(0, 0, 0, 100);
	}
}
//��������
void float_task(void *p_arg)
{
	while(1)
	{
		if(IN6 == KeyDown)
		{
			//��ʱ����
			delay(0, 0, 0, 30);
			if(IN6 == KeyDown)
			{
				IgkAgvOs.RunOrStop = Enum_Run;
				IGK_Speek("����");
				//�ȴ�����̧��
				while(IN6 == KeyDown){delay(0, 0, 0, 5);}
			}
		}
		if(IN7 == KeyDown)
		{
			//��ʱ����
			delay(0, 0, 0, 30);
			if(IN7 == KeyDown)
			{
				IgkAgvOs.RunOrStop = Enum_Stop;
				IGK_Speek("ֹͣ");
				//�ȴ�����̧��
				while(IN7 == KeyDown){delay(0, 0, 0, 5);}
			}
		}
		delay(0, 0, 0, 5); //��ʱ5ms
	}
}

void DEMO_task(void *p_arg)//LED,DA��FLASH����������
{
    
	while(1)
	{
		LED1 = ~LED1;
		delay(0, 0, 0, 300);
	}
}

//����
void DEMO1_task(void *p_arg)//���ڲ���
{
//	OS_ERR err;
//	CPU_STK_SIZE  n_free;
//  CPU_STK_SIZE  n_used;  
	while(1)
	{
		delay(0, 0, 1, 10);
//		OSTaskStkChk(&Task1TCB,
//                 &n_free,
//                 &n_used,
//                 &err);
//		IGK_SysTimePrintln("Task1:%d��ռ�ã�%d��ʹ���ʣ�%f",TASK1_STK_SIZE,n_used,n_used/(float)TASK1_STK_SIZE);
//		OSTaskStkChk(&ManualTaskTCB,
//                 &n_free,
//                 &n_used,
//                 &err);
//		IGK_SysTimePrintln("Manual:%d��ռ�ã�%d��ʹ���ʣ�%f",Manual_STK_SIZE,n_used,n_used/(float)Manual_STK_SIZE);
	}
}
//ι��,����ϵͳ����ʱ��
void DEMO2_task(void *p_arg)
{
	u16 num = 0;
	while(1)
	{
		//��ʱι��
		num++;
		if(num == 20)
		{
			num = 0;
			IWDG_Feed();//ι��
		}
		//����ϵͳ����ʱ��
		GetSysRunTime(&IgkAgvOs.OsTime,p_arg);
		delay(0, 0, 0, 5);
	}
}


