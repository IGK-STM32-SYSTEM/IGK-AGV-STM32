#include "sys.h"
#include "igk_dfs.h"
#include "igk_map.h"
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//任务优先级
#define TASK1_PRIO		4
//任务堆栈大小
#define TASK1_STK_SIZE 		512
//任务控制块
OS_TCB Task1TCB;
//任务堆栈
CPU_STK TASK1_STK[TASK1_STK_SIZE];
void task1(void *p_arg);


//任务优先级
#define Auto_TASK_PRIO		5
//任务堆栈大小
#define Auto_STK_SIZE 		512
//任务控制块
OS_TCB AutoTaskTCB;
//任务堆栈
CPU_STK Auto_TASK_STK[Auto_STK_SIZE];
//任务函数
void Auto_task(void *p_arg);

//任务优先级
#define Manual_TASK_PRIO		6
//任务堆栈大小
#define Manual_STK_SIZE 		512
//任务控制块
OS_TCB ManualTaskTCB;
//任务堆栈
CPU_STK Manual_TASK_STK[Manual_STK_SIZE];
//任务函数
void Manual_task(void *p_arg);

//任务优先级
#define FLOAT_TASK_PRIO		7
//任务堆栈大小
#define FLOAT_STK_SIZE		512
//任务控制块
OS_TCB	FloatTaskTCB;
//任务堆栈
CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];
//任务函数
void float_task(void *p_arg);

//任务优先级
#define DEMO_TASK_PRIO		8
//任务堆栈大小
#define DEMO_STK_SIZE 		512
//任务控制块
OS_TCB DEMOTaskTCB;
//任务堆栈
CPU_STK DEMO_TASK_STK[DEMO_STK_SIZE];
//任务函数
void DEMO_task(void *p_arg);

//任务优先级
#define Task5_TASK_PRIO		9
//任务堆栈大小
#define Task5_STK_SIZE 		512
//任务控制块
OS_TCB Task5_TaskTCB;
//任务堆栈
CPU_STK Task5_TASK_STK[Task5_STK_SIZE];
//任务函数
void Task5_task(void *p_arg);


//任务优先级
#define Control_TASK_PRIO		10
//任务堆栈大小
#define Control_STK_SIZE 		512
//任务控制块
OS_TCB ControlTaskTCB;
//任务堆栈
CPU_STK Control_TASK_STK[Control_STK_SIZE];
//任务函数
void Control_task(void *p_arg);


//任务优先级
#define WIFI_TASK_PRIO		11
//任务堆栈大小
#define WIFI_STK_SIZE 		512
//任务控制块
OS_TCB WIFITaskTCB;
//任务堆栈
CPU_STK WIFI_TASK_STK[WIFI_STK_SIZE];
//任务函数
void WIFI_task(void *p_arg);


//任务优先级
#define DEMO1_TASK_PRIO		13
//任务堆栈大小
#define DEMO1_STK_SIZE 		512
//任务控制块
OS_TCB DEMO1TaskTCB;
//任务堆栈
CPU_STK DEMO1_TASK_STK[DEMO1_STK_SIZE];
//任务函数
void DEMO1_task(void *p_arg);

//任务优先级
#define DEMO2_TASK_PRIO		12
//任务堆栈大小
#define DEMO2_STK_SIZE 		512
//任务控制块
OS_TCB DEMO2TaskTCB;
//任务堆栈
CPU_STK DEMO2_TASK_STK[DEMO2_STK_SIZE];
//任务函数
void DEMO2_task(void *p_arg);


//任务优先级
#define Screen_TASK_PRIO		14
//任务堆栈大小
#define Screen_STK_SIZE 		512
//任务控制块
OS_TCB ScreenTaskTCB;
//任务堆栈
CPU_STK Screen_TASK_STK[Screen_STK_SIZE];
//任务函数
void Screen_task(void *p_arg);


//任务优先级
#define PID_TASK_PRIO		15
//任务堆栈大小
#define PID_STK_SIZE 		512
//任务控制块
OS_TCB PIDTaskTCB;
//任务堆栈
CPU_STK PID_TASK_STK[PID_STK_SIZE];
//任务函数
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

	delay_init(168);  	//时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	LED_Init();         //LED初始化
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
	USART2_Configuration(115200);//485-2接控制软件
	USART3_Configuration(115200);//485-3接调试信息打印
	USART5_Configuration(115200);
//	TIM4_PWM_Init(1000 - 1, 8 - 1);	//84M/8=10Mhz?????,????1000,??PWM??? 10M/50000=200hz. //500-10   20hz-1000hz
	CAN1_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN1初始化普通模式,波特率500Kbps
	CAN2_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN2初始化普通模式,波特率500Kbps
	PID_Init();
	TIM2_Int_Init(5 - 1, 84 - 1);			//Tout=((arr+1)*(psc+1))/Ft us.  5us
//	IWDG_Init(4, 500);
	SPI1_Init();
  TLC5620_Init();
	
	
	//设定默认参数
	IgkAgvOs.OsTime.Hour = 0;
	IgkAgvOs.OsTime.Minute = 0;
	IgkAgvOs.OsTime.Second = 0;
	IgkAgvOs.OsTime.Millisecond = 0;
	
	IgkAgvOs.WorkMode = Enum_LocalAuto;//自动模式
	IgkAgvOs.Dir = Enum_QianJin;//前进方向
	*IgkAgvOs.TongXin.NowRfid = 1;
	IgkAgvOs.RunOrStop = Enum_Stop;//停止状态
	IgkAgvOs.Speed = 50;//速度50%;	
	
	IGK_SysTimePrintln("正在初始化...");
	//初始化Flash
	W25QXX_Init();			//W25QXX初始化
	while(W25QXX_ReadID()!=W25Q128)//检测不到W25Q128，LED闪烁
	{
		delay(0,0,0,100);
	}

	//系统启动，蜂鸣器提示
	for(u8 i=0;i<4;i++)
	{
		BEEP =~BEEP;
		osdelay_ms(60);
	}
	IGK_SysTimePrintln("Flash Ok!");	
	//初始化内存池[采用原子内存管理]
	my_mem_init(SRAMIN);
	IGK_SysTimePrintln("开启动态内存：%dKByte!",MEM1_MAX_SIZE/1024);	
	OSInit(&err);		//初始化UCOSIII
	IGK_SysTimePrintln("UCOSIII初始化完成,系统进入时间轮片!");
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB * )&StartTaskTCB,		//任务控制块
	             (CPU_CHAR * )"start task", 		//任务名字
	             (OS_TASK_PTR )start_task, 			//任务函数
	             (void * )0,					//传递给任务函数的参数
	             (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
	             (CPU_STK * )&START_TASK_STK[0],	//任务堆栈基地址
	             (CPU_STK_SIZE)START_STK_SIZE / 10,	//任务堆栈深度限位
	             (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
	             (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
	             (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
	             (void * )0,					//用户补充的存储区
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, //任务选项
	             (OS_ERR * )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区
	OSStart(&err);  //开启UCOSIII
	while(1);
}
//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
	OSStatTaskCPUUsageInit(&err);  	//统计任务
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
	CPU_IntDisMeasMaxCurReset();
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	//使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
#endif

	OS_CRITICAL_ENTER();	//进入临界区
	//创建Task1
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
//	IGK_SysTimePrintln("Task1创建完成!");
	//创建自动任务
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

//	IGK_SysTimePrintln("AutoTask创建完成!");
	//创建浮点测试任务
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

//	IGK_SysTimePrintln("FloatTask创建完成!");
	//创建触摸屏任务
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

//	IGK_SysTimePrintln("ScreenTask创建完成!");
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
//	IGK_SysTimePrintln("PIDTask创建完成!");
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
//	IGK_SysTimePrintln("ControlTask创建完成!");
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
//	IGK_SysTimePrintln("DEMOTask创建完成!");
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
//	IGK_SysTimePrintln("DEMO1Task创建完成!");
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

//	IGK_SysTimePrintln("DEMO2Task创建完成!");
	//创建手动任务
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

//	IGK_SysTimePrintln("ManualTask创建完成!");
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

//	IGK_SysTimePrintln("WIFITask创建完成!");
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

//	IGK_SysTimePrintln("Task5创建完成!");
	OS_TaskSuspend((OS_TCB *)&StartTaskTCB, &err);		//挂起开始任务
	OS_CRITICAL_EXIT();	//进入临界区
	IGK_SysTimePrintln("任务创建完成!");
}

//模式
void task1(void *p_arg)
{
	p_arg = p_arg;
	osdelay_ms(10);
	while(1)
	{
		//监控摇杆按键,切换模式
		if(IgkAgvOs.YaoGan.key==KeyDown)
		{
			delay(0, 0, 1, 0);
			if(IgkAgvOs.YaoGan.key==KeyDown)
			{
				if(IgkAgvOs.WorkMode == Enum_LocalManual)
				{
					IgkAgvOs.WorkMode = Enum_LocalAuto;
					IGK_Speek("自动模式");
				}
				else
				{
					IgkAgvOs.WorkMode = Enum_LocalManual;
					IGK_Speek("手动模式");
				}
			}
		}
		//手动控制模式
		if(IgkAgvOs.WorkMode == Enum_LocalManual)
		{
			//直接将速度赋值到驱动器
			//判断当前动作
			if(abs(IgkAgvOs.YaoGan.y)>0)//前进后退
			{
				IgkAgvOs.Speed = abs(IgkAgvOs.YaoGan.y)*1;
				//转向速度
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
			if(abs(IgkAgvOs.YaoGan.z)>0)//左右旋转
			{
				IgkAgvOs.Speed = abs(IgkAgvOs.YaoGan.z)*0.8;
				if(IgkAgvOs.YaoGan.z<0)
					DriverZuoXuan(IgkAgvOs.Speed);
				else
				if(IgkAgvOs.YaoGan.z>0)
					DriverYouXuan(IgkAgvOs.Speed);
			}
			else // 停止
				DriverTingZhi();
		}
		delay(0, 0, 0, 5);
	}
}
//自动运行模式
void Auto_task(void *p_arg)
{
	p_arg = p_arg;
	//初始化PID
	PID_Init();
	while(1)
	{
		//自动模式
		if(IgkAgvOs.WorkMode == Enum_LocalAuto)
		{
			//前进
			if(IgkAgvOs.Dir == Enum_QianJin)
			{ 
				if(IgkAgvOs.RunOrStop == Enum_Stop)
					DriverTingZhi();
				else
				{
					switch(IgkAgvOs.Action)
					{
						case Enum_ZuoXuan://左旋
							DriverZuoXuan(50);
							break;
						case Enum_YouXuan://右旋
							DriverYouXuan(50);
							break;
						case Enum_ZuoFenCha://左分叉
						case Enum_YouFenCha://右分叉
						case Enum_ZhiXing:  //直行
							if(IgkAgvOs.QianCiDaoHang.Error == 0)
							{
								//PID动态调节循迹
								float Inc = PosPIDCalc(IgkAgvOs.QianCiDaoHang.Distance);
								//更新速度
								s16 s1 = IgkAgvOs.Speed+Inc;
								s16 s2 = IgkAgvOs.Speed-Inc;
								//判定边界
								s1 = s1>100?100:s1;
								s1 = s1<0?0:s1;
								s2 = s2>100?100:s2;
								s2 = s2<0?0:s2;
								DriverQinJinSpeed(s1,s2);
							}
							//出轨停止
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
			//后退
			if(IgkAgvOs.Dir == Enum_HouTui)
			{
				if(IgkAgvOs.RunOrStop == Enum_Stop)
					DriverTingZhi();
				else
				{
					switch(IgkAgvOs.Action)
					{
						case Enum_ZuoXuan://左旋
							DriverZuoXuan(50);
							break;
						case Enum_YouXuan://右旋
							DriverYouXuan(50);
							break;
						case Enum_ZuoFenCha://左分叉
						case Enum_YouFenCha://右分叉
						case Enum_ZhiXing:  //直行
							if(IgkAgvOs.HouCiDaoHang.Error == 0)
							{
								//PID动态调节循迹
								float Inc = PosPIDCalc(-IgkAgvOs.HouCiDaoHang.Distance);
								//更新速度
								s16 s1 = IgkAgvOs.Speed + Inc;
								s16 s2 = IgkAgvOs.Speed - Inc;
								//判定边界
								s1 = s1>100?100:s1;
								s1 = s1<0?0:s1;
								s2 = s2>100?100:s2;
								s2 = s2<0?0:s2;
								DriverHouTuiSpeed(s1,s2);
							}
							//出轨停止
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

//计算目标位置
u16 target = 1;
void Manual_task(void *p_arg)
{
	p_arg = p_arg;
	osdelay_s(1);
	yinling(1);
	IGK_Speek("系统自检完成");
	while(1)
	{
		/*1.-----------------等待执行信号--------------------------*/
		IGK_SysTimePrintln("等待任务!");
		while(PLC_Data[43]!=1)
		{
			osdelay_ms(10);
		}
		//清除执行信号
		PLC_Data[43] = 0;
		/*2.-----------------判断目标标签合法性--------------------------*/
		if(PLC_Data[40]==*IgkAgvOs.TongXin.NowRfid)
		{
			IGK_Speek("目标标签和当前标签相同,任务完成！");
			IGK_SysTimePrintln("目标标签和当前标签相同,任务完成！");
			//跳过
			continue;
		}
		else
		if(PLC_Data[40]==0)
		{
			IGK_Speek("目标标签为0,操作非法！");
			IGK_SysTimePrintln("目标标签为0,操作非法！");
			//跳过
			continue;
		}
		else
		if(PLC_Data[40] > NodeMaxNum)
		{
			IGK_Speek("目标标签不能大于%d,操作非法！",NodeMaxNum);
			IGK_SysTimePrintln("目标标签不能大于%d,操作非法！",NodeMaxNum);
			//跳过
			continue;
		}	
		//更新目标位置到临时变量
		target = PLC_Data[40];
		//搜索路径
		FindRoute(*IgkAgvOs.TongXin.NowRfid,target);
		IGK_SysTimePrintln("路径搜索完成!");

		//播报目标位置
		IGK_Speek("准备前往%d号标签",target);
		osdelay_s(3);
		if(BestPath.PathTotal==0)
		{
			IGK_SysPrintf("未找到有效路径！\r\n\r\n");
			IGK_Speek("未找到有效路径,请重新设置");
		}
		else
		{
			IGK_SysPrintf("共找到%d条路径！\r\n\r\n",BestPath.PathTotal);
			IGK_Speek("共找到%d条路径！",BestPath.PathTotal);
			osdelay_s(2);
		/*3.-----------------执行路径--------------------------*/
			//启动
			IgkAgvOs.RunOrStop = Enum_Run;
			for(int i=0;i < BestPath.NodeCount;i++)
			{
				//给定默认速度
				IgkAgvOs.Speed = 50;
				u16 nodeId = BestPath.NodeList[i];
				//读取对应站点的地图
				StaionMapStruct mapStruct;
				ReadToMapStruct(nodeId,&mapStruct);
				IGK_SysPrintf("标签号:[%d]",nodeId);
				//下一个位置更新到界面
				PLC_Data[42] = nodeId;
				//等待标签,起点不用等待
				if(i!=0)
				{
						IGK_Speek("等待%d号标签!",nodeId);
						//osdelay_s(2);
						while(*IgkAgvOs.TongXin.NowRfid != nodeId )
						{
							osdelay_ms(5);
						}
						if(i == BestPath.NodeCount-1)
						{
							//到达终点
							break;
						}
				}
				//直行站点方向，动作
				for(int j=0;j<StationMapType;j++)
				{
					if(mapStruct.Stop[j] == BestPath.NodeList[i+1])
					{
						//方向
						if(mapStruct.Dir[j] == Enum_QianJin)
						{
							if(IgkAgvOs.Dir != Enum_QianJin)
							{
								//停止
								IgkAgvOs.RunOrStop = Enum_Stop;
								DriverTingZhi();
								//切换方向
								IgkAgvOs.Dir = Enum_QianJin;
								//等待
								osdelay_ms(500);
							}
							//打印数据
							IGK_SysPrintf("前进-");
							//语音播报
							IGK_Speek("前进");					
						}
						else
						if(mapStruct.Dir[j] == Enum_HouTui)
						{
							if(IgkAgvOs.Dir != Enum_HouTui)
							{
								//停止
								IgkAgvOs.RunOrStop = Enum_Stop;
								DriverTingZhi();
								//切换方向
								IgkAgvOs.Dir = Enum_HouTui;
								//等待
								osdelay_ms(500);
							}
							//打印数据
							IGK_SysPrintf("后退-");
							//语音播报
							IGK_Speek("后退");
						}
						else
						if(mapStruct.Dir[j] == Enum_PingYi)
						{
							if(IgkAgvOs.Dir != Enum_ZuoFenCha)
							{
								osdelay_ms(300);
								IGK_SysPrintf("平移-");
								IGK_Speek("平移");
							}
						}
						
						//动作
						if(mapStruct.Action[j] == Enum_ZuoFenCha)
						{
							IgkAgvOs.Action = Enum_ZuoFenCha;
							IGK_SysPrintf("左分叉-");
							IGK_Speek("左分叉");
						}
						else
						if(mapStruct.Action[j] == Enum_YouFenCha)
						{
							IgkAgvOs.Action = Enum_YouFenCha;
							IGK_SysPrintf("右分叉-");
							IGK_Speek("右分叉");
						}
						else
						if(mapStruct.Action[j] == Enum_ZhiXing)
						{
							IgkAgvOs.Action = Enum_ZhiXing;
							IGK_SysPrintf("直行-");
							IGK_Speek("直行");
						}
						else
						if(mapStruct.Action[j] == Enum_ZuoXuan)
						{
							//停车
							IgkAgvOs.RunOrStop = Enum_Stop;
							DriverTingZhi();
							IGK_Speek("左旋%d度",mapStruct.Angle[j]);
							//原地旋转
							osdelay_ms(200);
							//左旋
							IgkAgvOs.Action = Enum_ZuoXuan;
							//启动
							IgkAgvOs.RunOrStop = Enum_Run;
							//根据方向选择磁导航传感器
							Fencha_struct * PCiDaoHang;
							if(IgkAgvOs.Dir == Enum_QianJin)
								PCiDaoHang = &IgkAgvOs.QianCiDaoHang;
							else
								PCiDaoHang = &IgkAgvOs.HouCiDaoHang;
							//等待离开磁条
							while(PCiDaoHang->Error==0)
								osdelay_ms(10);
							//等待检测到磁条[至少两个点并且居中]
							while(PCiDaoHang->Num <3)
								osdelay_ms(10);
							if(mapStruct.Angle[j]==180)
							{
								//等待离开磁条
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//等待检测到磁条[至少两个点并且居中]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
							}
							else
							if(mapStruct.Angle[j]==270)
							{
								//等待离开磁条
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//等待检测到磁条[至少两个点并且居中]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
								//等待离开磁条
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//等待检测到磁条[至少两个点并且居中]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
							}
							//停止
							IgkAgvOs.RunOrStop = Enum_Stop;
							DriverTingZhi();
							osdelay_ms(200);
							//切换直行
							IgkAgvOs.Action = Enum_ZhiXing;
							//切换为运动状态
							osdelay_ms(200);
							IgkAgvOs.RunOrStop = Enum_Run;
							IGK_SysPrintf("左旋[%d]度-",mapStruct.Angle[j]);
						}
						else
						if(mapStruct.Action[j] == Enum_YouXuan)
						{
							//停车
							IgkAgvOs.RunOrStop = Enum_Stop;
							DriverTingZhi();
							IGK_Speek("右旋%d度",mapStruct.Angle[j]);
							//原地旋转
							osdelay_ms(200);
							//右旋
							IgkAgvOs.Action = Enum_YouXuan;
							//启动
							IgkAgvOs.RunOrStop = Enum_Run;
							//根据方向选择磁导航传感器
							Fencha_struct * PCiDaoHang;
							if(IgkAgvOs.Dir == Enum_QianJin)
								PCiDaoHang = &IgkAgvOs.QianCiDaoHang;
							else
								PCiDaoHang = &IgkAgvOs.HouCiDaoHang;
							//等待离开磁条
							while(PCiDaoHang->Error==0)
								osdelay_ms(10);
							//等待检测到磁条[至少两个点并且居中]
							while(PCiDaoHang->Num <3)
								osdelay_ms(10);
							if(mapStruct.Angle[j]==180)
							{
								//等待离开磁条
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//等待检测到磁条[至少两个点并且居中]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
							}
							else
							if(mapStruct.Angle[j]==270)
							{
								//等待离开磁条
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//等待检测到磁条[至少两个点并且居中]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
								//等待离开磁条
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//等待检测到磁条[至少两个点并且居中]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
							}
							//停止
							IgkAgvOs.RunOrStop = Enum_Stop;
							DriverTingZhi();
							osdelay_ms(200);
							//切换直行
							IgkAgvOs.Action = Enum_ZhiXing;
							//切换为运动状态
							osdelay_ms(200);
							//原地旋转
							IGK_SysPrintf("右旋[%d]度-",mapStruct.Angle[j]);
						}
						IgkAgvOs.RunOrStop = Enum_Run;
						//到达
						IGK_SysPrintf("[%d]\r\n\r\n",mapStruct.Stop[j]);
						//找到对应站点，跳出循环
						break;
					}
				}
			}
			//到达目标位置,停车
			IgkAgvOs.RunOrStop = Enum_Stop;
			IGK_Speek("到达终点，任务完成");
			IGK_SysTimePrintln("到达终点，任务完成");
		}
		delay(0, 0, 0, 10); //延时10ms
	}
}
//Modbus同步进程
void PID_task(void *p_arg)
{
	while(1)
	{
		ReadWriteMap(p_arg);
		//当前位置
		PLC_Data[41] = *IgkAgvOs.TongXin.NowRfid;
		//当前速度
		PLC_Data[61] = IgkAgvOs.Speed;
		delay(0, 0, 0, 100);
	}
}
//监听按键
void float_task(void *p_arg)
{
	while(1)
	{
		if(IN6 == KeyDown)
		{
			//延时消抖
			delay(0, 0, 0, 30);
			if(IN6 == KeyDown)
			{
				IgkAgvOs.RunOrStop = Enum_Run;
				IGK_Speek("启动");
				//等待按键抬起
				while(IN6 == KeyDown){delay(0, 0, 0, 5);}
			}
		}
		if(IN7 == KeyDown)
		{
			//延时消抖
			delay(0, 0, 0, 30);
			if(IN7 == KeyDown)
			{
				IgkAgvOs.RunOrStop = Enum_Stop;
				IGK_Speek("停止");
				//等待按键抬起
				while(IN7 == KeyDown){delay(0, 0, 0, 5);}
			}
		}
		delay(0, 0, 0, 5); //延时5ms
	}
}
//触摸屏上路径流程参数设置
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
void WIFI_task(void *p_arg)//串口，CAN测试
{
	while(1)
	{
		delay(0, 0,0, 300);
	}

}
void DEMO_task(void *p_arg)//LED,DA，FLASH，语音测试
{
    
	while(1)
	{
		LED1 = ~LED1;
		delay(0, 0, 0, 300);
	}
}

//启动
void DEMO1_task(void *p_arg)//网口测试
{
	while(1)
	{
		delay(0, 0, 0, 10);
	}
}
//喂狗,更新系统运行时间
void DEMO2_task(void *p_arg)
{
	u16 num = 0;
	while(1)
	{
		//定时喂狗
		num++;
		if(num == 20)
		{
			num = 0;
			IWDG_Feed();//喂狗
		}
		//更新系统运行时间
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
