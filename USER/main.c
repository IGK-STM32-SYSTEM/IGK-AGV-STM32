#include "sys.h"
#include "igk_dfs.h"
#include "igk_map.h"
#include "igk_ucos.h"
#include "malloc.h"

OS_TCB Task1TCB;//������ƿ�
CPU_STK Task1_STK[256];//�����ջ
void Task1_task(void *p_arg);//������

OS_TCB Task2TCB;//������ƿ�
CPU_STK Task2_STK[256];//�����ջ
void Task2_task(void *p_arg);//������

OS_TCB Task3TCB;//������ƿ�
CPU_STK Task3_STK[256];//�����ջ
void Task3_task(void *p_arg);//������

OS_TCB Task4TCB;//������ƿ�
CPU_STK Task4_STK[256];//�����ջ
void Task4_task(void *p_arg);//������

OS_TCB Task5TCB;//������ƿ�
CPU_STK Task5_STK[256];//�����ջ
void Task5_task(void *p_arg);//������

OS_TCB Task6TCB;//������ƿ�
CPU_STK Task6_STK[256];//�����ջ
void Task6_task(void *p_arg);//������

OS_TCB Task7TCB;//������ƿ�
CPU_STK Task7_STK[256];//�����ջ
void Task7_task(void *p_arg);//������

OS_TCB Task8TCB;//������ƿ�
CPU_STK Task8_STK[128];//�����ջ
void Task8_task(void *p_arg);//������

int main(void)
{
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
	TIM2_Int_Init(5 - 1, 84 - 1);			//Tout=((arr+1)*(psc+1))/Ft us.  5us
	IWDG_Init(4, 500);//���1066ms
	SPI1_Init();
  TLC5620_Init();
	/*-----------ϵͳָ�����ͱ���ָ��Ĭ�ϵ�ַ---------------*/
	//
	IgkSystem.RFID = &PLC_Data[41];//ʵʱRFIDֵ,�Ͷ�����������ֵͬ��
	IgkSystem.AutoSpeed = &PLC_Data[110];//�Զ�ģʽ�ٶ�
	IgkSystem.ManuaSpeed = &PLC_Data[111];//�ֶ�ģʽ�ٶ�
	//����Task
	IgkSystem.Task.Target = &PLC_Data[40];     //Ŀ���ǩ
	IgkSystem.Task.Next = &PLC_Data[42];   //��һ��λ��
	IgkSystem.Task.Execute = &PLC_Data[43];    //ִ��
	IgkSystem.Task.Cancel = &PLC_Data[44];     //ȡ��
	IgkSystem.Task.SerialNum = &PLC_Data[45];  //�����š�ϵͳ�Զ����ӣ���ͨ���ӿڸ��¡�
	
	//���
	IgkSystem.Battery.Percent = &PLC_Data[100];  //ʣ������ٷֱ�
	IgkSystem.Battery.Voltage = &PLC_Data[101];   //��ѹ
	IgkSystem.Battery.Current = &PLC_Data[102];	//����
	IgkSystem.Battery.Now = &PLC_Data[103];			//ʣ�����
	IgkSystem.Battery.Total = &PLC_Data[104];		//�������
	
	//PID
	IgkSystem.PID.SetTarget = (s16*)&PLC_Data[200]; 
	IgkSystem.PID.SumError = (s16*)&PLC_Data[201]; 
	IgkSystem.PID.Error = (s16*)&PLC_Data[202]; 
	IgkSystem.PID.LastError = (s16*)&PLC_Data[203]; 
	IgkSystem.PID.Kp = &PLC_Data[205]; 
	IgkSystem.PID.Ki = &PLC_Data[206]; 
	IgkSystem.PID.Kd = &PLC_Data[207]; 
	
	/*------------�趨Ĭ�ϲ���-------------------------------*/
	//��ʼ��ʱ��
	IgkSystem.OsTime.Hour = 0;
	IgkSystem.OsTime.Minute = 0;
	IgkSystem.OsTime.Second = 0;
	IgkSystem.OsTime.Millisecond = 0;
	
	IgkSystem.WorkMode = Enum_LocalAuto;//�Զ�ģʽ
	IgkSystem.Dir = Enum_QianJin;//ǰ������
	*IgkSystem.RFID = 1;//RFIDֵ
	IgkSystem.RunOrStop = Enum_Stop;//ֹͣ״̬
	*IgkSystem.AutoSpeed = 50;//�Զ��ٶ�;	
	*IgkSystem.ManuaSpeed = 80;//�ֶ��ٶ�;
		
	/*------------------------------------------------------*/

	PID_Init();
	
	IGK_SysTimePrintln("���ڳ�ʼ��...");
	//��ʼ��Flash
	W25QXX_Init();			//W25QXX��ʼ��
	while(W25QXX_ReadID()!=W25Q128)//��ⲻ��W25Q128��LED��˸
	{
		delay_ms(100);
	}
	//ϵͳ��������������ʾ
	for(u8 i=0;i<4;i++)
	{
		BEEP =~BEEP;
		delay_ms(60);
	}

	IGK_SysTimePrintln("Flash Ok!");	
	//��ʼ���ڴ��[����ԭ���ڴ����]
	my_mem_init(SRAMIN);
	IGK_SysTimePrintln("������̬�ڴ棺%dKByte!",MEM1_MAX_SIZE/1024);	
//	SEGGER_RTT_printf(0,"��ʼ��ucos���");
	//��ʼ��ucos
	IGK_UCOS_Init();	
	while(1);
}


//��ʼ������,����������̣�ִ��һ�κ��Զ��˳�
void start_task(void *p_arg)
{
	p_arg = p_arg;
	//��������
	IGK_UCOS_Create(4,sizeof(Task1_STK)/4,Task1_STK,&Task1TCB,Task1_task);
	IGK_UCOS_Create(5,sizeof(Task2_STK)/4,Task2_STK,&Task2TCB,Task2_task);
	IGK_UCOS_Create(6,sizeof(Task3_STK)/4,Task3_STK,&Task3TCB,Task3_task);
	IGK_UCOS_Create(7,sizeof(Task4_STK)/4,Task4_STK,&Task4TCB,Task4_task);
	IGK_UCOS_Create(8,sizeof(Task5_STK)/4,Task5_STK,&Task5TCB,Task5_task);
	IGK_UCOS_Create(9,sizeof(Task6_STK)/4,Task6_STK,&Task6TCB,Task6_task);
	IGK_UCOS_Create(10,sizeof(Task7_STK)/4,Task7_STK,&Task7TCB,Task7_task);
	IGK_UCOS_Create(11,sizeof(Task8_STK)/4,Task8_STK,&Task8TCB,Task8_task);
	

	IGK_SysTimePrintln("UCOSIII��ʼ�����,ϵͳ����ʱ����Ƭ!");
}

/*������1��������ר�á�*****************************
****************************************************/
void Task1_task(void *p_arg)
{
	OS_ERR err;  
//	u16 num = 0;
	osdelay_s(1);
	yinling(1);
	IGK_Speek("ϵͳ�Լ����");
			CPU_STK_SIZE free,used;

	while(1)
	{
		//��������
//		num++;
//		IGK_SysTimePrintln("������%d",num);
//		LED1 = ~LED1;
//		delay(0, 0,0 , 20);
		//��ӡ�����ջʹ����
		OSTaskStkChk (&Task1TCB,&free,&used,&err);
		IGK_SysTimePrintln("Task1 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
		OSTaskStkChk (&Task2TCB,&free,&used,&err);
		IGK_SysTimePrintln("Task2 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
		OSTaskStkChk (&Task3TCB,&free,&used,&err); 	
		IGK_SysTimePrintln("Task3 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
		OSTaskStkChk (&Task4TCB,&free,&used,&err);
		IGK_SysTimePrintln("Task4 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
		OSTaskStkChk (&Task5TCB,&free,&used,&err);
		IGK_SysTimePrintln("Task5 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
		OSTaskStkChk (&Task6TCB,&free,&used,&err);
		IGK_SysTimePrintln("Task6 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
		OSTaskStkChk (&Task7TCB,&free,&used,&err);
		IGK_SysTimePrintln("Task7 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
		OSTaskStkChk (&Task8TCB,&free,&used,&err);	
		IGK_SysTimePrintln("Task8 used/free:%d/%d  usage:%d",used,free,(used*100)/(used+free));  		
		osdelay_s(1);
	}
}
/*������2�����Զ�ģʽ��**************************************************
*                    
****************************************************/
void Task2_task(void *p_arg)
{
	//��ʼ��PID
	PID_Init();
	while(1)
	{
		//�Զ�ģʽ
		if(IgkSystem.WorkMode == Enum_LocalAuto)
		{
			//ǰ��
			if(IgkSystem.Dir == Enum_QianJin)
			{ 
				if(IgkSystem.RunOrStop == Enum_Stop)
					DriverTingZhi();
				else
				{
					switch(IgkSystem.Action)
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
							if(IgkSystem.QianCiDaoHang.Error == 0)
							{
								//PID��̬����ѭ��
								float Inc = PosPIDCalc(IgkSystem.QianCiDaoHang.Distance);
								//�����ٶ�
								s16 s1 = *IgkSystem.AutoSpeed+Inc;
								s16 s2 = *IgkSystem.AutoSpeed-Inc;
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
			if(IgkSystem.Dir == Enum_HouTui)
			{
				if(IgkSystem.RunOrStop == Enum_Stop)
					DriverTingZhi();
				else
				{
					switch(IgkSystem.Action)
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
							if(IgkSystem.HouCiDaoHang.Error == 0)
							{
								//PID��̬����ѭ��
								float Inc = PosPIDCalc(-IgkSystem.HouCiDaoHang.Distance);
								//�����ٶ�
								s16 s1 = *IgkSystem.AutoSpeed + Inc;
								s16 s2 = *IgkSystem.AutoSpeed - Inc;
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
/*������3����ִ���Զ�����**************************************************
1.����ִ��
2.���Ƿ�
3.��������·��
4.ִ������
****************************************************/
void Task3_task(void *p_arg)
{
	p_arg = p_arg;
	while(1)
	{
		GotoStart:
		/*1.-----------------�ȴ�ִ���ź�--------------------------*/
		IGK_SysTimePrintln("�ȴ�����!");
		while(*IgkSystem.Task.Execute != DEF_TRUE)
		{
			osdelay_ms(10);
		}
		//���ִ���ź�
		*IgkSystem.Task.Execute = DEF_FALSE;
		/*2.-----------------�ж�Ŀ���ǩ�Ϸ���--------------------------*/
		if(*IgkSystem.Task.Target == *IgkSystem.RFID)
		{
			IGK_Speek("Ŀ���ǩ�͵�ǰ��ǩ��ͬ,������ɣ�");
			IGK_SysTimePrintln("Ŀ���ǩ�͵�ǰ��ǩ��ͬ,������ɣ�");
			//����
			continue;
		}
		else
		if(*IgkSystem.Task.Target == 0)
		{
			IGK_Speek("Ŀ���ǩΪ0,�����Ƿ���");
			IGK_SysTimePrintln("Ŀ���ǩΪ0,�����Ƿ���");
			//����
			continue;
		}
		else
		if(*IgkSystem.Task.Target > NodeMaxNum)
		{
			IGK_Speek("Ŀ���ǩ���ܴ���%d,�����Ƿ���",NodeMaxNum);
			IGK_SysTimePrintln("Ŀ���ǩ���ܴ���%d,�����Ƿ���",NodeMaxNum);
			//����
			continue;
		}	
		//����·��
		FindRoute(*IgkSystem.RFID,*IgkSystem.Task.Target);
		IGK_SysTimePrintln("·���������!");

		//����Ŀ��λ��
		IGK_Speek("׼��ǰ��%d�ű�ǩ",*IgkSystem.Task.Target);
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
			IgkSystem.RunOrStop = Enum_Run;
			for(int i=0;i < BestPath.NodeCount;i++)
			{
				u16 nodeId = BestPath.NodeList[i];
				//��ȡ��Ӧվ��ĵ�ͼ
				StaionMapStruct mapStruct;
				ReadToMapStruct(nodeId,&mapStruct);
				IGK_SysPrintf("��ǩ��:[%d]",nodeId);
				//��һ��λ�ø��µ�����
				*IgkSystem.Task.Next = nodeId;
				//�ȴ���ǩ,��㲻�õȴ�
				if(i!=0)
				{
						IGK_Speek("�ȴ�%d�ű�ǩ!",nodeId);
						//osdelay_s(2);
						while(*IgkSystem.RFID != nodeId )
						{
							//���ȡ������
							if(*IgkSystem.Task.Cancel == DEF_TRUE)
							{
								//ͣ��
								IgkSystem.RunOrStop = Enum_Stop;
								//���ȡ�����
								*IgkSystem.Task.Cancel = DEF_FALSE;
								//����Ŀ�����һվ��λ��ǰ
								*IgkSystem.Task.Next  = *IgkSystem.RFID;
								*IgkSystem.Task.Target  = *IgkSystem.RFID;
								IGK_Speek("����ȡ���ɹ���");
								IGK_SysTimePrintln("������ȡ����");
								//�����˻ص��ȴ�״̬
								goto GotoStart;
							}
							osdelay_ms(5);
						}
						if(i == BestPath.NodeCount-1)
						{
							//�����յ�
							break;
						}
				}
				
				//TODO:
				//����дվ�㶯��,��ͣ��ʱ�䣬����������
				

				//ֱ��վ�㷽�򣬶���
				for(int j=0;j<StationMapType;j++)
				{
					if(mapStruct.Stop[j] == BestPath.NodeList[i+1])
					{
						//����
						if(mapStruct.Dir[j] == Enum_QianJin)
						{
							if(IgkSystem.Dir != Enum_QianJin)
							{
								//ֹͣ
								IgkSystem.RunOrStop = Enum_Stop;
								DriverTingZhi();
								//�л�����
								IgkSystem.Dir = Enum_QianJin;
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
							if(IgkSystem.Dir != Enum_HouTui)
							{
								//ֹͣ
								IgkSystem.RunOrStop = Enum_Stop;
								DriverTingZhi();
								//�л�����
								IgkSystem.Dir = Enum_HouTui;
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
							if(IgkSystem.Dir != Enum_ZuoFenCha)
							{
								osdelay_ms(300);
								IGK_SysPrintf("ƽ��-");
								IGK_Speek("ƽ��");
							}
						}
						
						//����
						if(mapStruct.Action[j] == Enum_ZuoFenCha)
						{
							IgkSystem.Action = Enum_ZuoFenCha;
							IGK_SysPrintf("��ֲ�-");
							IGK_Speek("��ֲ�");
						}
						else
						if(mapStruct.Action[j] == Enum_YouFenCha)
						{
							IgkSystem.Action = Enum_YouFenCha;
							IGK_SysPrintf("�ҷֲ�-");
							IGK_Speek("�ҷֲ�");
						}
						else
						if(mapStruct.Action[j] == Enum_ZhiXing)
						{
							IgkSystem.Action = Enum_ZhiXing;
							IGK_SysPrintf("ֱ��-");
							IGK_Speek("ֱ��");
						}
						else
						if(mapStruct.Action[j] == Enum_ZuoXuan)
						{
							//ͣ��
							IgkSystem.RunOrStop = Enum_Stop;
							DriverTingZhi();
							IGK_Speek("����%d��",mapStruct.Angle[j]);
							//ԭ����ת
							osdelay_ms(200);
							//����
							IgkSystem.Action = Enum_ZuoXuan;
							//����
							IgkSystem.RunOrStop = Enum_Run;
							//���ݷ���ѡ��ŵ���������
							Fencha_struct * PCiDaoHang;
							if(IgkSystem.Dir == Enum_QianJin)
								PCiDaoHang = &IgkSystem.QianCiDaoHang;
							else
								PCiDaoHang = &IgkSystem.HouCiDaoHang;
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
							IgkSystem.RunOrStop = Enum_Stop;
							DriverTingZhi();
							osdelay_ms(200);
							//�л�ֱ��
							IgkSystem.Action = Enum_ZhiXing;
							//�л�Ϊ�˶�״̬
							osdelay_ms(200);
							IgkSystem.RunOrStop = Enum_Run;
							IGK_SysPrintf("����[%d]��-",mapStruct.Angle[j]);
						}
						else
						if(mapStruct.Action[j] == Enum_YouXuan)
						{
							//ͣ��
							IgkSystem.RunOrStop = Enum_Stop;
							DriverTingZhi();
							IGK_Speek("����%d��",mapStruct.Angle[j]);
							//ԭ����ת
							osdelay_ms(200);
							//����
							IgkSystem.Action = Enum_YouXuan;
							//����
							IgkSystem.RunOrStop = Enum_Run;
							//���ݷ���ѡ��ŵ���������
							Fencha_struct * PCiDaoHang;
							if(IgkSystem.Dir == Enum_QianJin)
								PCiDaoHang = &IgkSystem.QianCiDaoHang;
							else
								PCiDaoHang = &IgkSystem.HouCiDaoHang;
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
							IgkSystem.RunOrStop = Enum_Stop;
							DriverTingZhi();
							osdelay_ms(200);
							//�л�ֱ��
							IgkSystem.Action = Enum_ZhiXing;
							//�л�Ϊ�˶�״̬
							osdelay_ms(200);
							//ԭ����ת
							IGK_SysPrintf("����[%d]��-",mapStruct.Angle[j]);
						}
						IgkSystem.RunOrStop = Enum_Run;
						//����
						IGK_SysPrintf("[%d]\r\n\r\n",mapStruct.Stop[j]);
						//�ҵ���Ӧվ�㣬����ѭ��
						break;
					}
				}
			}
			//����Ŀ��λ��,ͣ��
			IgkSystem.RunOrStop = Enum_Stop;
			IGK_Speek("�����յ㣬�������");
			IGK_SysTimePrintln("�����յ㣬�������");
		}
		delay(0, 0, 0, 100); //��ʱ10ms
	}
}
/*������4�����ա�**************************************************
*                   
****************************************************/
void Task4_task(void *p_arg)
{
	p_arg = p_arg;
	while(1)
	{
		delay(0,0,0,100);
	}
}
/*������5�����ֶ����Զ��л���**************************************************
*                   
****************************************************/
void Task5_task(void *p_arg)
{
	while(1)
	{
		//���ҡ�˰���,�л�ģʽ
		if(IgkSystem.YaoGan.key==Enum_KeyDown)
		{
			delay(0, 0, 1, 0);
			if(IgkSystem.YaoGan.key==Enum_KeyDown)
			{
				if(IgkSystem.WorkMode == Enum_LocalManual)
				{
					IgkSystem.WorkMode = Enum_LocalAuto;
					IGK_Speek("�Զ�ģʽ");
				}
				else
				{
					IgkSystem.WorkMode = Enum_LocalManual;
					IGK_Speek("�ֶ�ģʽ");
				}
			}
		}
		//�ֶ�����ģʽ
		if(IgkSystem.WorkMode == Enum_LocalManual)
		{
			//ֱ�ӽ��ٶȸ�ֵ��������
			//�жϵ�ǰ����
			if(abs(IgkSystem.YaoGan.y)>0)//ǰ������
			{
				*IgkSystem.ManuaSpeed = abs(IgkSystem.YaoGan.y)*1;
				//ת���ٶ�
				s8 speed = IgkSystem.YaoGan.z*0.5;
				if(IgkSystem.YaoGan.y>0)
				{
					if(speed >= 0)
						DriverQinJinSpeed(*IgkSystem.ManuaSpeed-speed,*IgkSystem.ManuaSpeed);
					else
						DriverQinJinSpeed(*IgkSystem.ManuaSpeed,*IgkSystem.ManuaSpeed+speed);
				}
				else
				if(IgkSystem.YaoGan.y<0)
				{
					if(speed >= 0)
						DriverHouTuiSpeed(*IgkSystem.ManuaSpeed,*IgkSystem.ManuaSpeed-speed);
					else
						DriverHouTuiSpeed(*IgkSystem.ManuaSpeed+speed,*IgkSystem.ManuaSpeed);
				}
			}
			else
			if(abs(IgkSystem.YaoGan.z)>0)//������ת
			{
				*IgkSystem.ManuaSpeed = abs(IgkSystem.YaoGan.z)*0.8;
				if(IgkSystem.YaoGan.z<0)
					DriverZuoXuan(*IgkSystem.ManuaSpeed);
				else
				if(IgkSystem.YaoGan.z>0)
					DriverYouXuan(*IgkSystem.ManuaSpeed);
			}
			else // ֹͣ
				DriverTingZhi();
		}
		delay(0, 0, 0, 10);
	}
}
/*������6����IO��ء�**************************************************
*                   
****************************************************/
void Task6_task(void *p_arg)
{
	while(1)
	{
		if(IN6 == Enum_KeyDown)
		{
			//��ʱ����
			delay(0, 0, 0, 30);
			if(IN6 == Enum_KeyDown)
			{
				IgkSystem.RunOrStop = Enum_Run;
				IGK_Speek("����");
				//�ȴ�����̧��
				while(IN6 == Enum_KeyDown){delay(0, 0, 0, 5);}
			}
		}
		if(IN7 == Enum_KeyDown)
		{
			//��ʱ����
			delay(0, 0, 0, 30);
			if(IN7 == Enum_KeyDown)
			{
				IgkSystem.RunOrStop = Enum_Stop;
				IGK_Speek("ֹͣ");
				//�ȴ�����̧��
				while(IN7 == Enum_KeyDown){delay(0, 0, 0, 5);}
			}
		}
		delay(0, 0, 0, 10); //��ʱ5ms
	}
}
/*������7����Modbus��**************************************************
*                 
****************************************************/
void Task7_task(void *p_arg)
{
	p_arg = p_arg;
	while(1)
	{
		//��ӦModbus��д��ͼ��Ϣ
		ReadWriteMap(p_arg);
		delay(0,0,0,100);
	}
}
/*������8����ι��+ϵͳʱ����¡�**************************************************   
1.
2.����ϵͳ����ʱ��
****************************************************/
void Task8_task(void *p_arg)
{
	while(1)
	{
		IWDG_Feed();//ι��
		//����ϵͳ����ʱ��
		GetSysRunTime(&IgkSystem.OsTime,p_arg);
		delay(0, 0, 0, 200);
	}
}




















