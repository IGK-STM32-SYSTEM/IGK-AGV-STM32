#include "igk_ucos.h"


#define START_TASK_PRIO		3//任务优先级
#define START_STK_SIZE 		512//任务堆栈大小
OS_TCB StartTaskTCB;//任务控制块
CPU_STK START_TASK_STK[START_STK_SIZE];//任务堆栈
void start_task(void *p_arg);//任务函数

#define TASK1_PRIO		4//任务优先级
#define TASK1_STK_SIZE 		128//任务堆栈大小
OS_TCB Task1TCB;//任务控制块
CPU_STK TASK1_STK[TASK1_STK_SIZE];//任务堆栈
void task1(void *p_arg);

#define Auto_TASK_PRIO		5//任务优先级
#define Auto_STK_SIZE 		512//任务堆栈大小
OS_TCB AutoTaskTCB;//任务控制块
CPU_STK Auto_TASK_STK[Auto_STK_SIZE];//任务堆栈
void Auto_task(void *p_arg);//任务函数

#define Manual_TASK_PRIO		6//任务优先级
#define Manual_STK_SIZE 		512//任务堆栈大小
OS_TCB ManualTaskTCB;//任务控制块
CPU_STK Manual_TASK_STK[Manual_STK_SIZE];//任务堆栈
void Manual_task(void *p_arg);//任务函数

#define FLOAT_TASK_PRIO		7//任务优先级
#define FLOAT_STK_SIZE		512//任务堆栈大小
OS_TCB	FloatTaskTCB;//任务控制块
CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];//任务堆栈
void float_task(void *p_arg);//任务函数


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

#define DEMO1_TASK_PRIO		13
#define DEMO1_STK_SIZE 		512//任务堆栈大小
OS_TCB DEMO1TaskTCB;//任务控制块
CPU_STK DEMO1_TASK_STK[DEMO1_STK_SIZE];//任务堆栈
void DEMO1_task(void *p_arg);//任务函数

#define DEMO2_TASK_PRIO		12//任务优先级
#define DEMO2_STK_SIZE 		512//任务堆栈大小
OS_TCB DEMO2TaskTCB;//任务控制块
CPU_STK DEMO2_TASK_STK[DEMO2_STK_SIZE];//任务堆栈
void DEMO2_task(void *p_arg);//任务函数


#define PID_TASK_PRIO		15
#define PID_STK_SIZE 		512//任务堆栈大小
OS_TCB PIDTaskTCB;//任务控制块
CPU_STK PID_TASK_STK[PID_STK_SIZE];//任务堆栈
void PID_task(void *p_arg);//任务函数



////开始任务函数
//void start_task(void *p_arg)
//{
//	OS_ERR err;
//	CPU_SR_ALLOC();
//	p_arg = p_arg;

//	CPU_Init();
//#if OS_CFG_STAT_TASK_EN > 0u
//	OSStatTaskCPUUsageInit(&err);  	//统计任务
//#endif

//#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
//	CPU_IntDisMeasMaxCurReset();
//#endif

//#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
//	//使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
//	OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
//#endif

//	OS_CRITICAL_ENTER();	//进入临界区
//	//创建Task1
//	OSTaskCreate((OS_TCB * )&Task1TCB,
//	             (CPU_CHAR * )"Transducer task",
//	             (OS_TASK_PTR )task1,
//	             (void * )0,
//	             (OS_PRIO	  )TASK1_PRIO,
//	             (CPU_STK * )&TASK1_STK[0],
//	             (CPU_STK_SIZE)TASK1_STK_SIZE / 10,
//	             (CPU_STK_SIZE)TASK1_STK_SIZE,
//	             (OS_MSG_QTY  )0,
//	             (OS_TICK	  )0,
//	             (void * )0,
//	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
//	             (OS_ERR * )&err);
////	IGK_SysTimePrintln("Task1创建完成!");
//	//创建自动任务
//	OSTaskCreate((OS_TCB * )&AutoTaskTCB,
//	             (CPU_CHAR * )"Auto task",
//	             (OS_TASK_PTR )Auto_task,
//	             (void * )0,
//	             (OS_PRIO	  )Auto_TASK_PRIO,
//	             (CPU_STK * )&Auto_TASK_STK[0],
//	             (CPU_STK_SIZE)Auto_STK_SIZE / 10,
//	             (CPU_STK_SIZE)Auto_STK_SIZE,
//	             (OS_MSG_QTY  )0,
//	             (OS_TICK	  )0,
//	             (void * )0,
//	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
//	             (OS_ERR * )&err);

////	IGK_SysTimePrintln("AutoTask创建完成!");
//	//创建浮点测试任务
//	OSTaskCreate((OS_TCB * )&FloatTaskTCB,
//	             (CPU_CHAR * )"float test task",
//	             (OS_TASK_PTR )float_task,
//	             (void * )0,
//	             (OS_PRIO	  )FLOAT_TASK_PRIO,
//	             (CPU_STK * )&FLOAT_TASK_STK[0],
//	             (CPU_STK_SIZE)FLOAT_STK_SIZE / 10,
//	             (CPU_STK_SIZE)FLOAT_STK_SIZE,
//	             (OS_MSG_QTY  )0,
//	             (OS_TICK	  )0,
//	             (void * )0,
//	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
//	             (OS_ERR * )&err);

////	IGK_SysTimePrintln("FloatTask创建完成!");
//	//PID
//	OSTaskCreate((OS_TCB * )&PIDTaskTCB,
//	             (CPU_CHAR * )"PID task",
//	             (OS_TASK_PTR )PID_task,
//	             (void * )0,
//	             (OS_PRIO	  )PID_TASK_PRIO,
//	             (CPU_STK * )&PID_TASK_STK[0],
//	             (CPU_STK_SIZE)PID_STK_SIZE / 10,
//	             (CPU_STK_SIZE)PID_STK_SIZE,
//	             (OS_MSG_QTY  )0,
//	             (OS_TICK	  )0,
//	             (void * )0,
//	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
//	             (OS_ERR * )&err);
////	IGK_SysTimePrintln("PIDTask创建完成!");
//	//DEMO
//	OSTaskCreate((OS_TCB * )&DEMOTaskTCB,
//	             (CPU_CHAR * )"DEMO task",
//	             (OS_TASK_PTR )DEMO_task,
//	             (void * )0,
//	             (OS_PRIO	  )DEMO_TASK_PRIO,
//	             (CPU_STK * )&DEMO_TASK_STK[0],
//	             (CPU_STK_SIZE)DEMO_STK_SIZE / 10,
//	             (CPU_STK_SIZE)DEMO_STK_SIZE,
//	             (OS_MSG_QTY  )0,
//	             (OS_TICK	  )0,
//	             (void * )0,
//	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
//	             (OS_ERR * )&err);
////	IGK_SysTimePrintln("DEMOTask创建完成!");
//	//DEMO1
//	OSTaskCreate((OS_TCB * )&DEMO1TaskTCB,
//	             (CPU_CHAR * )"DEMO1 task",
//	             (OS_TASK_PTR )DEMO1_task,
//	             (void * )0,
//	             (OS_PRIO	  )DEMO1_TASK_PRIO,
//	             (CPU_STK * )&DEMO1_TASK_STK[0],
//	             (CPU_STK_SIZE)DEMO1_STK_SIZE / 10,
//	             (CPU_STK_SIZE)DEMO1_STK_SIZE,
//	             (OS_MSG_QTY  )0,
//	             (OS_TICK	  )0,
//	             (void * )0,
//	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
//	             (OS_ERR * )&err);
////	IGK_SysTimePrintln("DEMO1Task创建完成!");
//	//DEMO2
//	OSTaskCreate((OS_TCB * )&DEMO2TaskTCB,
//	             (CPU_CHAR * )"DEMO2 task",
//	             (OS_TASK_PTR )DEMO2_task,
//	             (void * )0,
//	             (OS_PRIO	  )DEMO2_TASK_PRIO,
//	             (CPU_STK * )&DEMO2_TASK_STK[0],
//	             (CPU_STK_SIZE)DEMO2_STK_SIZE / 10,
//	             (CPU_STK_SIZE)DEMO2_STK_SIZE,
//	             (OS_MSG_QTY  )0,
//	             (OS_TICK	  )0,
//	             (void * )0,
//	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
//	             (OS_ERR * )&err);

////	IGK_SysTimePrintln("DEMO2Task创建完成!");
//	//创建手动任务
//	OSTaskCreate((OS_TCB * )&ManualTaskTCB,
//	             (CPU_CHAR * )"Manual task",
//	             (OS_TASK_PTR )Manual_task,
//	             (void * )0,
//	             (OS_PRIO	  )Manual_TASK_PRIO,
//	             (CPU_STK * )&Manual_TASK_STK[0],
//	             (CPU_STK_SIZE)Manual_STK_SIZE / 10,
//	             (CPU_STK_SIZE)Manual_STK_SIZE,
//	             (OS_MSG_QTY  )0,
//	             (OS_TICK	  )0,
//	             (void * )0,
//	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
//	             (OS_ERR * )&err);

//	OS_TaskSuspend((OS_TCB *)&StartTaskTCB, &err);		//挂起开始任务
//	OS_CRITICAL_EXIT();	//退出临界区
//}


//UCOS初始化
void UCOS_Init(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();

	OSInit(&err);		//初始化UCOSIII
	
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
}

/*
Prio：任务优先级
TaskFun：任务函数
Size: 任务堆栈大小
*/	
void CreakTask(u8 Prio,u16 Size,OS_TCB *TaskTCB,OS_TASK_PTR TaskFun)
{
	OS_ERR err;
	CPU_SR_ALLOC();	
	OS_CRITICAL_ENTER();//进入临界区
	//任务堆栈
	CPU_STK *Task_STK = (CPU_STK*)mymalloc(SRAMIN,sizeof(CPU_STK)*Size);
	
	//创建开始任务
	OSTaskCreate((OS_TCB * )TaskTCB,		//任务控制块
	             (CPU_CHAR * )"Test task", 		//任务名字
	             (OS_TASK_PTR )TaskFun, 			//任务函数
	             (void * )0,					//传递给任务函数的参数
	             (OS_PRIO	  )Prio,     //任务优先级
	             (CPU_STK * )Task_STK,	//任务堆栈基地址
	             (CPU_STK_SIZE)Size / 10,	//任务堆栈深度限位
	             (CPU_STK_SIZE)Size,		//任务堆栈大小
	             (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
	             (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
	             (void * )0,					//用户补充的存储区
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, //任务选项
	             (OS_ERR * )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区
}



















