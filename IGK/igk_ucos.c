#include "igk_ucos.h"


#define START_TASK_PRIO		3//�������ȼ�
#define START_STK_SIZE 		512//�����ջ��С
OS_TCB StartTaskTCB;//������ƿ�
CPU_STK START_TASK_STK[START_STK_SIZE];//�����ջ
void start_task(void *p_arg);//������

#define TASK1_PRIO		4//�������ȼ�
#define TASK1_STK_SIZE 		128//�����ջ��С
OS_TCB Task1TCB;//������ƿ�
CPU_STK TASK1_STK[TASK1_STK_SIZE];//�����ջ
void task1(void *p_arg);

#define Auto_TASK_PRIO		5//�������ȼ�
#define Auto_STK_SIZE 		512//�����ջ��С
OS_TCB AutoTaskTCB;//������ƿ�
CPU_STK Auto_TASK_STK[Auto_STK_SIZE];//�����ջ
void Auto_task(void *p_arg);//������

#define Manual_TASK_PRIO		6//�������ȼ�
#define Manual_STK_SIZE 		512//�����ջ��С
OS_TCB ManualTaskTCB;//������ƿ�
CPU_STK Manual_TASK_STK[Manual_STK_SIZE];//�����ջ
void Manual_task(void *p_arg);//������

#define FLOAT_TASK_PRIO		7//�������ȼ�
#define FLOAT_STK_SIZE		512//�����ջ��С
OS_TCB	FloatTaskTCB;//������ƿ�
CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];//�����ջ
void float_task(void *p_arg);//������


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

#define DEMO1_TASK_PRIO		13
#define DEMO1_STK_SIZE 		512//�����ջ��С
OS_TCB DEMO1TaskTCB;//������ƿ�
CPU_STK DEMO1_TASK_STK[DEMO1_STK_SIZE];//�����ջ
void DEMO1_task(void *p_arg);//������

#define DEMO2_TASK_PRIO		12//�������ȼ�
#define DEMO2_STK_SIZE 		512//�����ջ��С
OS_TCB DEMO2TaskTCB;//������ƿ�
CPU_STK DEMO2_TASK_STK[DEMO2_STK_SIZE];//�����ջ
void DEMO2_task(void *p_arg);//������


#define PID_TASK_PRIO		15
#define PID_STK_SIZE 		512//�����ջ��С
OS_TCB PIDTaskTCB;//������ƿ�
CPU_STK PID_TASK_STK[PID_STK_SIZE];//�����ջ
void PID_task(void *p_arg);//������



////��ʼ������
//void start_task(void *p_arg)
//{
//	OS_ERR err;
//	CPU_SR_ALLOC();
//	p_arg = p_arg;

//	CPU_Init();
//#if OS_CFG_STAT_TASK_EN > 0u
//	OSStatTaskCPUUsageInit(&err);  	//ͳ������
//#endif

//#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
//	CPU_IntDisMeasMaxCurReset();
//#endif

//#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
//	//ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
//	OSSchedRoundRobinCfg(DEF_ENABLED, 1, &err);
//#endif

//	OS_CRITICAL_ENTER();	//�����ٽ���
//	//����Task1
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
////	IGK_SysTimePrintln("Task1�������!");
//	//�����Զ�����
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

////	IGK_SysTimePrintln("AutoTask�������!");
//	//���������������
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

////	IGK_SysTimePrintln("FloatTask�������!");
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
////	IGK_SysTimePrintln("PIDTask�������!");
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
////	IGK_SysTimePrintln("DEMOTask�������!");
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
////	IGK_SysTimePrintln("DEMO1Task�������!");
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

////	IGK_SysTimePrintln("DEMO2Task�������!");
//	//�����ֶ�����
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

//	OS_TaskSuspend((OS_TCB *)&StartTaskTCB, &err);		//����ʼ����
//	OS_CRITICAL_EXIT();	//�˳��ٽ���
//}


//UCOS��ʼ��
void UCOS_Init(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();

	OSInit(&err);		//��ʼ��UCOSIII
	
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
}

/*
Prio���������ȼ�
TaskFun��������
Size: �����ջ��С
*/	
void CreakTask(u8 Prio,u16 Size,OS_TCB *TaskTCB,OS_TASK_PTR TaskFun)
{
	OS_ERR err;
	CPU_SR_ALLOC();	
	OS_CRITICAL_ENTER();//�����ٽ���
	//�����ջ
	CPU_STK *Task_STK = (CPU_STK*)mymalloc(SRAMIN,sizeof(CPU_STK)*Size);
	
	//������ʼ����
	OSTaskCreate((OS_TCB * )TaskTCB,		//������ƿ�
	             (CPU_CHAR * )"Test task", 		//��������
	             (OS_TASK_PTR )TaskFun, 			//������
	             (void * )0,					//���ݸ��������Ĳ���
	             (OS_PRIO	  )Prio,     //�������ȼ�
	             (CPU_STK * )Task_STK,	//�����ջ����ַ
	             (CPU_STK_SIZE)Size / 10,	//�����ջ�����λ
	             (CPU_STK_SIZE)Size,		//�����ջ��С
	             (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
	             (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
	             (void * )0,					//�û�����Ĵ洢��
	             (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, //����ѡ��
	             (OS_ERR * )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���
}



















