#include "sys.h"
//#include "igk_ucos.h"

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
void Task1_task(void *p_arg);

double ddd;
int main(void)
{
		OS_ERR err;
	CPU_SR_ALLOC();

	delay_init(168);  	//ʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
//ddd = 0;
	LED_Init();         //LED��ʼ��
	
	
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

	while(1);
}


//��ʼ������,����������̣�ִ��һ�κ��Զ��˳�
void start_task(void *p_arg)
{
	p_arg = p_arg;
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
	OS_TaskSuspend((OS_TCB *)&StartTaskTCB, &err);		//����ʼ����
	OS_CRITICAL_EXIT();	//�����ٽ���
}

/*������1��������ר�á�*****************************
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







