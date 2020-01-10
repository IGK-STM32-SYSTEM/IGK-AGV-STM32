#include "igk_ucos.h"
#define START_TASK_PRIO		3//�������ȼ�
#define START_STK_SIZE 		128//�����ջ��С
OS_TCB StartTaskTCB;//������ƿ�
CPU_STK START_TASK_STK[START_STK_SIZE];//�����ջ
void start_task(void *p_arg);//������

//UCOS��ʼ��
void IGK_UCOS_Init(void)
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


/*��������**********************************************
Prio:     �������ȼ�
Size:     �����ջ��С
Task_STK: �����ջ����ַ
TaskTCB:  ������ƿ�
TaskFun:  ������
******************************************************/	
void IGK_UCOS_Create(u8 Prio,u16 Size,CPU_STK *Task_STK,OS_TCB *TaskTCB,OS_TASK_PTR TaskFun)
{
	OS_ERR err;
	CPU_SR_ALLOC();	
	OS_CRITICAL_ENTER();//�����ٽ���
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



















