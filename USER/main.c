#include "sys.h"
#include "igk_dfs.h"
#include "igk_map.h"
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С
#define START_STK_SIZE 		512
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//�������ȼ�
#define TASK1_PRIO		4
//�����ջ��С
#define TASK1_STK_SIZE 		512
//������ƿ�
OS_TCB Task1TCB;
//�����ջ
CPU_STK TASK1_STK[TASK1_STK_SIZE];
void task1(void *p_arg);


//�������ȼ�
#define Auto_TASK_PRIO		5
//�����ջ��С
#define Auto_STK_SIZE 		512
//������ƿ�
OS_TCB AutoTaskTCB;
//�����ջ
CPU_STK Auto_TASK_STK[Auto_STK_SIZE];
//������
void Auto_task(void *p_arg);

//�������ȼ�
#define Manual_TASK_PRIO		6
//�����ջ��С
#define Manual_STK_SIZE 		512
//������ƿ�
OS_TCB ManualTaskTCB;
//�����ջ
CPU_STK Manual_TASK_STK[Manual_STK_SIZE];
//������
void Manual_task(void *p_arg);

//�������ȼ�
#define FLOAT_TASK_PRIO		7
//�����ջ��С
#define FLOAT_STK_SIZE		512
//������ƿ�
OS_TCB	FloatTaskTCB;
//�����ջ
CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];
//������
void float_task(void *p_arg);

//�������ȼ�
#define DEMO_TASK_PRIO		8
//�����ջ��С
#define DEMO_STK_SIZE 		512
//������ƿ�
OS_TCB DEMOTaskTCB;
//�����ջ
CPU_STK DEMO_TASK_STK[DEMO_STK_SIZE];
//������
void DEMO_task(void *p_arg);

//�������ȼ�
#define Task5_TASK_PRIO		9
//�����ջ��С
#define Task5_STK_SIZE 		512
//������ƿ�
OS_TCB Task5_TaskTCB;
//�����ջ
CPU_STK Task5_TASK_STK[Task5_STK_SIZE];
//������
void Task5_task(void *p_arg);


//�������ȼ�
#define Control_TASK_PRIO		10
//�����ջ��С
#define Control_STK_SIZE 		512
//������ƿ�
OS_TCB ControlTaskTCB;
//�����ջ
CPU_STK Control_TASK_STK[Control_STK_SIZE];
//������
void Control_task(void *p_arg);


//�������ȼ�
#define WIFI_TASK_PRIO		11
//�����ջ��С
#define WIFI_STK_SIZE 		512
//������ƿ�
OS_TCB WIFITaskTCB;
//�����ջ
CPU_STK WIFI_TASK_STK[WIFI_STK_SIZE];
//������
void WIFI_task(void *p_arg);


//�������ȼ�
#define DEMO1_TASK_PRIO		13
//�����ջ��С
#define DEMO1_STK_SIZE 		512
//������ƿ�
OS_TCB DEMO1TaskTCB;
//�����ջ
CPU_STK DEMO1_TASK_STK[DEMO1_STK_SIZE];
//������
void DEMO1_task(void *p_arg);

//�������ȼ�
#define DEMO2_TASK_PRIO		12
//�����ջ��С
#define DEMO2_STK_SIZE 		512
//������ƿ�
OS_TCB DEMO2TaskTCB;
//�����ջ
CPU_STK DEMO2_TASK_STK[DEMO2_STK_SIZE];
//������
void DEMO2_task(void *p_arg);


//�������ȼ�
#define Screen_TASK_PRIO		14
//�����ջ��С
#define Screen_STK_SIZE 		512
//������ƿ�
OS_TCB ScreenTaskTCB;
//�����ջ
CPU_STK Screen_TASK_STK[Screen_STK_SIZE];
//������
void Screen_task(void *p_arg);


//�������ȼ�
#define PID_TASK_PRIO		15
//�����ջ��С
#define PID_STK_SIZE 		512
//������ƿ�
OS_TCB PIDTaskTCB;
//�����ջ
CPU_STK PID_TASK_STK[PID_STK_SIZE];
//������
void PID_task(void *p_arg);

#define SOCK_TCPS        0
#define SOCK_UDPS        1
#define DATA_BUF_SIZE   2048
/* Private macro -------------------------------------------------------------*/
uint8_t gDATABUF[DATA_BUF_SIZE];
uint32_t CPU_ID[3];
// Default Network Configuration
wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0x00, 0xab, 0xcd},
                            .ip = {192, 168, 10, 123},
                            .sn = {255, 255, 255, 0},
                            .gw = {192, 168, 10, 1},
                            .dns = {0, 0, 0, 0},
                            .dhcp = NETINFO_STATIC
                          };
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void platform_init(void);								// initialize the dependent host peripheral
void network_init(void);								// Initialize Network information and display it
int32_t loopback_tcps(uint8_t, uint8_t *, uint16_t);		// Loopback TCP server
int32_t loopback_udps(uint8_t, uint8_t *, uint16_t);		// Loopback UDP server
u8 aa;
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();

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
	
	
	//�趨Ĭ�ϲ���
	IgkAgvOs.OsTime.Hour = 0;
	IgkAgvOs.OsTime.Minute = 0;
	IgkAgvOs.OsTime.Second = 0;
	IgkAgvOs.OsTime.Millisecond = 0;
	
	IgkAgvOs.WorkMode = Enum_LocalAuto;//�Զ�ģʽ
	IgkAgvOs.Dir = Enum_QianJin;//ǰ������
	*IgkAgvOs.TongXin.NowRfid = 1;
	IgkAgvOs.RunOrStop = Enum_Stop;//ֹͣ״̬
	IgkAgvOs.Speed = 50;//�ٶ�50%;	
	
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
	OSInit(&err);		//��ʼ��UCOSIII
	IGK_SysTimePrintln("UCOSIII��ʼ�����,ϵͳ����ʱ����Ƭ!");
	OS_CRITICAL_ENTER();//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB * )&StartTaskTCB,		//������ƿ�
	             (CPU_CHAR * )"start task", 		//��������
	             (OS_TASK_PTR )start_task, 			//������
	             (void * )0,					//���ݸ��������Ĳ���
	             (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
	             (CPU_STK * )&START_TASK_STK[0],	//�����ջ����ַ
	             (CPU_STK_SIZE)START_STK_SIZE / 10,	//�����ջ�����λ
	             (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
	             (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
	             (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
	             (void * )0,					//�û�����Ĵ洢��
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, //����ѡ��
	             (OS_ERR * )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	OSStart(&err);  //����UCOSIII
	while(1);
}
//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&err);  	//ͳ������
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
	CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	//ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
#endif

	OS_CRITICAL_ENTER();	//�����ٽ���
	//����Task1
	OSTaskCreate((OS_TCB * )&Task1TCB,
	             (CPU_CHAR * )"Transducer task",
	             (OS_TASK_PTR )task1,
	             (void * )0,
	             (OS_PRIO	  )TASK1_PRIO,
	             (CPU_STK * )&TASK1_STK[0],
	             (CPU_STK_SIZE)TASK1_STK_SIZE / 10,
	             (CPU_STK_SIZE)TASK1_STK_SIZE,
	             (OS_MSG_QTY  )0,
	             (OS_TICK	  )0,
	             (void * )0,
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR * )&err);
//	IGK_SysTimePrintln("Task1�������!");
	//�����Զ�����
	OSTaskCreate((OS_TCB * )&AutoTaskTCB,
	             (CPU_CHAR * )"Auto task",
	             (OS_TASK_PTR )Auto_task,
	             (void * )0,
	             (OS_PRIO	  )Auto_TASK_PRIO,
	             (CPU_STK * )&Auto_TASK_STK[0],
	             (CPU_STK_SIZE)Auto_STK_SIZE / 10,
	             (CPU_STK_SIZE)Auto_STK_SIZE,
	             (OS_MSG_QTY  )0,
	             (OS_TICK	  )0,
	             (void * )0,
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR * )&err);

//	IGK_SysTimePrintln("AutoTask�������!");
	//���������������
	OSTaskCreate((OS_TCB * )&FloatTaskTCB,
	             (CPU_CHAR * )"float test task",
	             (OS_TASK_PTR )float_task,
	             (void * )0,
	             (OS_PRIO	  )FLOAT_TASK_PRIO,
	             (CPU_STK * )&FLOAT_TASK_STK[0],
	             (CPU_STK_SIZE)FLOAT_STK_SIZE / 10,
	             (CPU_STK_SIZE)FLOAT_STK_SIZE,
	             (OS_MSG_QTY  )0,
	             (OS_TICK	  )0,
	             (void * )0,
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR * )&err);

//	IGK_SysTimePrintln("FloatTask�������!");
	//��������������
	OSTaskCreate((OS_TCB * )&ScreenTaskTCB,
	             (CPU_CHAR * )"Screen task",
	             (OS_TASK_PTR )Screen_task,
	             (void * )0,
	             (OS_PRIO	  )Screen_TASK_PRIO,
	             (CPU_STK * )&Screen_TASK_STK[0],
	             (CPU_STK_SIZE)Screen_STK_SIZE / 10,
	             (CPU_STK_SIZE)Screen_STK_SIZE,
	             (OS_MSG_QTY  )0,
	             (OS_TICK	  )0,
	             (void * )0,
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR * )&err);

//	IGK_SysTimePrintln("ScreenTask�������!");
	//PID
	OSTaskCreate((OS_TCB * )&PIDTaskTCB,
	             (CPU_CHAR * )"PID task",
	             (OS_TASK_PTR )PID_task,
	             (void * )0,
	             (OS_PRIO	  )PID_TASK_PRIO,
	             (CPU_STK * )&PID_TASK_STK[0],
	             (CPU_STK_SIZE)PID_STK_SIZE / 10,
	             (CPU_STK_SIZE)PID_STK_SIZE,
	             (OS_MSG_QTY  )0,
	             (OS_TICK	  )0,
	             (void * )0,
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR * )&err);
//	IGK_SysTimePrintln("PIDTask�������!");
	//Control
	OSTaskCreate((OS_TCB * )&ControlTaskTCB,
	             (CPU_CHAR * )"Control task",
	             (OS_TASK_PTR )Control_task,
	             (void * )0,
	             (OS_PRIO	  )Control_TASK_PRIO,
	             (CPU_STK * )&Control_TASK_STK[0],
	             (CPU_STK_SIZE)Control_STK_SIZE / 10,
	             (CPU_STK_SIZE)Control_STK_SIZE,
	             (OS_MSG_QTY  )0,
	             (OS_TICK	  )0,
	             (void * )0,
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR * )&err);
//	IGK_SysTimePrintln("ControlTask�������!");
	//DEMO
	OSTaskCreate((OS_TCB * )&DEMOTaskTCB,
	             (CPU_CHAR * )"DEMO task",
	             (OS_TASK_PTR )DEMO_task,
	             (void * )0,
	             (OS_PRIO	  )DEMO_TASK_PRIO,
	             (CPU_STK * )&DEMO_TASK_STK[0],
	             (CPU_STK_SIZE)DEMO_STK_SIZE / 10,
	             (CPU_STK_SIZE)DEMO_STK_SIZE,
	             (OS_MSG_QTY  )0,
	             (OS_TICK	  )0,
	             (void * )0,
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR * )&err);
//	IGK_SysTimePrintln("DEMOTask�������!");
	//DEMO1
	OSTaskCreate((OS_TCB * )&DEMO1TaskTCB,
	             (CPU_CHAR * )"DEMO1 task",
	             (OS_TASK_PTR )DEMO1_task,
	             (void * )0,
	             (OS_PRIO	  )DEMO1_TASK_PRIO,
	             (CPU_STK * )&DEMO1_TASK_STK[0],
	             (CPU_STK_SIZE)DEMO1_STK_SIZE / 10,
	             (CPU_STK_SIZE)DEMO1_STK_SIZE,
	             (OS_MSG_QTY  )0,
	             (OS_TICK	  )0,
	             (void * )0,
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR * )&err);
//	IGK_SysTimePrintln("DEMO1Task�������!");
	//DEMO2
	OSTaskCreate((OS_TCB * )&DEMO2TaskTCB,
	             (CPU_CHAR * )"DEMO2 task",
	             (OS_TASK_PTR )DEMO2_task,
	             (void * )0,
	             (OS_PRIO	  )DEMO2_TASK_PRIO,
	             (CPU_STK * )&DEMO2_TASK_STK[0],
	             (CPU_STK_SIZE)DEMO2_STK_SIZE / 10,
	             (CPU_STK_SIZE)DEMO2_STK_SIZE,
	             (OS_MSG_QTY  )0,
	             (OS_TICK	  )0,
	             (void * )0,
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR * )&err);

//	IGK_SysTimePrintln("DEMO2Task�������!");
	//�����ֶ�����
	OSTaskCreate((OS_TCB * )&ManualTaskTCB,
	             (CPU_CHAR * )"Manual task",
	             (OS_TASK_PTR )Manual_task,
	             (void * )0,
	             (OS_PRIO	  )Manual_TASK_PRIO,
	             (CPU_STK * )&Manual_TASK_STK[0],
	             (CPU_STK_SIZE)Manual_STK_SIZE / 10,
	             (CPU_STK_SIZE)Manual_STK_SIZE,
	             (OS_MSG_QTY  )0,
	             (OS_TICK	  )0,
	             (void * )0,
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR * )&err);

//	IGK_SysTimePrintln("ManualTask�������!");
	OSTaskCreate((OS_TCB * )&WIFITaskTCB,
	             (CPU_CHAR * )"WIFI task",
	             (OS_TASK_PTR )WIFI_task,
	             (void * )0,
	             (OS_PRIO	  )WIFI_TASK_PRIO,
	             (CPU_STK * )&WIFI_TASK_STK[0],
	             (CPU_STK_SIZE)WIFI_STK_SIZE / 10,
	             (CPU_STK_SIZE)WIFI_STK_SIZE,
	             (OS_MSG_QTY  )0,
	             (OS_TICK	  )0,
	             (void * )0,
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR * )&err);

//	IGK_SysTimePrintln("WIFITask�������!");
	OSTaskCreate((OS_TCB * )&Task5_TaskTCB,
	             (CPU_CHAR * )"Task5 task",
	             (OS_TASK_PTR )Task5_task,
	             (void * )0,
	             (OS_PRIO	  )Task5_TASK_PRIO,
	             (CPU_STK * )&Task5_TASK_STK[0],
	             (CPU_STK_SIZE)Task5_STK_SIZE / 10,
	             (CPU_STK_SIZE)Task5_STK_SIZE,
	             (OS_MSG_QTY  )0,
	             (OS_TICK	  )0,
	             (void * )0,
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
	             (OS_ERR * )&err);

//	IGK_SysTimePrintln("Task5�������!");
	OS_TaskSuspend((OS_TCB *)&StartTaskTCB, &err);		//����ʼ����
	OS_CRITICAL_EXIT();	//�����ٽ���
	IGK_SysTimePrintln("���񴴽����!");
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
				IgkAgvOs.Speed = abs(IgkAgvOs.YaoGan.y)*1;
				//ת���ٶ�
				s8 speed = IgkAgvOs.YaoGan.z*0.5;
				if(IgkAgvOs.YaoGan.y>0)
				{
					if(speed >= 0)
						DriverQinJinSpeed(IgkAgvOs.Speed-speed,IgkAgvOs.Speed);
					else
						DriverQinJinSpeed(IgkAgvOs.Speed,IgkAgvOs.Speed+speed);
				}
				else
				if(IgkAgvOs.YaoGan.y<0)
				{
					if(speed >= 0)
						DriverHouTuiSpeed(IgkAgvOs.Speed,IgkAgvOs.Speed-speed);
					else
						DriverHouTuiSpeed(IgkAgvOs.Speed+speed,IgkAgvOs.Speed);
				}
			}
			else
			if(abs(IgkAgvOs.YaoGan.z)>0)//������ת
			{
				IgkAgvOs.Speed = abs(IgkAgvOs.YaoGan.z)*0.8;
				if(IgkAgvOs.YaoGan.z<0)
					DriverZuoXuan(IgkAgvOs.Speed);
				else
				if(IgkAgvOs.YaoGan.z>0)
					DriverYouXuan(IgkAgvOs.Speed);
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
								s16 s1 = IgkAgvOs.Speed+Inc;
								s16 s2 = IgkAgvOs.Speed-Inc;
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
								s16 s1 = IgkAgvOs.Speed + Inc;
								s16 s2 = IgkAgvOs.Speed - Inc;
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
				IgkAgvOs.Speed = 50;
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
		//��ǰλ��
		PLC_Data[41] = *IgkAgvOs.TongXin.NowRfid;
		//��ǰ�ٶ�
		PLC_Data[61] = IgkAgvOs.Speed;
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
//��������·�����̲�������
void Screen_task(void *p_arg)
{
	while(1)
	{
		delay(0, 0, 0, 10);
	}
}
//
void Task5_task(void *p_arg)
{
	p_arg = p_arg;
	while(1)
	{
		delay(0, 0, 0, 5);
	}
}

//
void Control_task(void *p_arg)
{
	while(1)
	{
		delay(0, 0, 0, 5);
	}
}
void WIFI_task(void *p_arg)//���ڣ�CAN����
{
	while(1)
	{
		delay(0, 0,0, 300);
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
	while(1)
	{
		delay(0, 0, 0, 10);
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

/**
  * @brief  Intialize the network information to be used in WIZCHIP
  * @retval None
  */
void network_init(void)
{
	uint8_t tmpstr[6];
	CPU_ID[0] = *(vu32 *)(0x1FFF7A10);
	CPU_ID[1] = *(vu32 *)(0x1FFF7A14);
	CPU_ID[2] = *(vu32 *)(0x1FFF7A18);
	gWIZNETINFO.mac[0] = (CPU_ID[0] >> 16) & 0XFF;
	gWIZNETINFO.mac[1] = (CPU_ID[0] >> 8) & 0XFFF;
	gWIZNETINFO.mac[2] = CPU_ID[0] & 0XFF;
	gWIZNETINFO.mac[3] = (CPU_ID[1] >> 16) & 0XFF;
	gWIZNETINFO.mac[4] = (CPU_ID[1] >> 8) & 0XFFF;
	gWIZNETINFO.mac[5] = CPU_ID[1] & 0XFF;
	
	ctlnetwork(CN_SET_NETINFO, (void *)&gWIZNETINFO);
	ctlnetwork(CN_GET_NETINFO, (void *)&gWIZNETINFO);

	// Display Network Information
	ctlwizchip(CW_GET_ID, (void *)tmpstr);
	printf("\r\n=== %s NET CONF ===\r\n", (char *)tmpstr);
	printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", gWIZNETINFO.mac[0], gWIZNETINFO.mac[1], gWIZNETINFO.mac[2],
	       gWIZNETINFO.mac[3], gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
	printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1], gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
	printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1], gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
	printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1], gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
	printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1], gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
	printf("======================\r\n");
}

/**
  * @brief  Loopback Test Example Code using ioLibrary_BSD
  * @retval None
  */
int32_t loopback_tcps(uint8_t sn, uint8_t *buf, uint16_t port)
{
	int32_t ret;
	uint16_t size = 0, sentsize = 0;
	switch(getSn_SR(sn))
	{
		case SOCK_ESTABLISHED :
			if(getSn_IR(sn) & Sn_IR_CON)
			{
				printf("%d:Connected\r\n", sn);
				setSn_IR(sn, Sn_IR_CON);
			}
			if((size = getSn_RX_RSR(sn)) > 0)
			{
				if(size > DATA_BUF_SIZE) { size = DATA_BUF_SIZE; }
				ret = recv(sn, buf, size);
				if(ret <= 0) { return ret; }
				sentsize = 0;
				while(size != sentsize)
				{
					ret = send(sn, buf + sentsize, size - sentsize);
					if(ret < 0)
					{
						close(sn);
						return ret;
					}
					sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
				}
			}
			break;
		case SOCK_CLOSE_WAIT :
			printf("%d:CloseWait\r\n", sn);
			if((ret = disconnect(sn)) != SOCK_OK) { return ret; }
			printf("%d:Closed\r\n", sn);
			break;
		case SOCK_INIT :
			printf("%d:Listen, port [%d]\r\n", sn, port);
			if( (ret = listen(sn)) != SOCK_OK) { return ret; }
			break;
		case SOCK_CLOSED:
			printf("%d:LBTStart\r\n", sn);
			if((ret = socket(sn, Sn_MR_TCP, port, 0x00)) != sn)
			{ return ret; }
			printf("%d:Opened\r\n", sn);
			break;
		default:
			break;
	}
	return 1;
}

/**
  * @brief  Loopback Test Example Code using ioLibrary_BSD
  * @retval None
  */
int32_t loopback_udps(uint8_t sn, uint8_t *buf, uint16_t port)
{
	int32_t  ret;
	uint16_t size, sentsize;
	uint8_t  destip[4];
	uint16_t destport;
	//uint8_t  packinfo = 0;
	switch(getSn_SR(sn))
	{
		case SOCK_UDP :
			if((size = getSn_RX_RSR(sn)) > 0)
			{
				if(size > DATA_BUF_SIZE) { size = DATA_BUF_SIZE; }
				ret = recvfrom(sn, buf, size, destip, (uint16_t *)&destport);
				if(ret <= 0)
				{
					printf("%d: recvfrom error. %ld\r\n", sn, ret);
					return ret;
				}
				size = (uint16_t) ret;
				sentsize = 0;
				while(sentsize != size)
				{
					ret = sendto(sn, buf + sentsize, size - sentsize, destip, destport);
					if(ret < 0)
					{
						printf("%d: sendto error. %ld\r\n", sn, ret);
						return ret;
					}
					sentsize += ret; // Don't care SOCKERR_BUSY, because it is zero.
				}
			}
			break;
		case SOCK_CLOSED:
			printf("%d:LBUStart\r\n", sn);
			if((ret = socket(sn, Sn_MR_UDP, port, 0x00)) != sn)
			{ return ret; }
			printf("%d:Opened, port [%d]\r\n", sn, port);
			break;
		default :
			break;
	}
	return 1;
}
