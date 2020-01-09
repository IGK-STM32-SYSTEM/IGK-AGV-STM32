#include "sys.h"
//#include "igk_ucos.h"

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
void Task1_task(void *p_arg);

double ddd;
int main(void)
{
		OS_ERR err;
	CPU_SR_ALLOC();

	delay_init(168);  	//时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
//ddd = 0;
	LED_Init();         //LED初始化
	
	
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

	while(1);
}


//开始任务函数,创建任务进程，执行一次后自动退出
void start_task(void *p_arg)
{
	p_arg = p_arg;
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
	             (OS_TASK_PTR )Task1_task,
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
	OS_TaskSuspend((OS_TCB *)&StartTaskTCB, &err);		//挂起开始任务
	OS_CRITICAL_EXIT();	//进入临界区
}

/*【任务1】【调试专用】*****************************
****************************************************/
void Task1_task(void *p_arg)
{
//	OS_ERR err;  
//	osdelay_s(1);
	while(1)
	{
		LED1 = ~LED1;
		delay_ms(2000);
		//delay(0,0,2,0);
	}
}







