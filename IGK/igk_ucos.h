#ifndef __IGK_UCOS_H
#define __IGK_UCOS_H
#include "sys.h"

//UCOS��ʼ��
extern void UCOS_Init(void);
/*��������
Prio:     �������ȼ�
Size:     �����ջ��С
Task_STK: �����ջ����ַ
TaskTCB:  ������ƿ�
TaskFun:  ������
*/
extern void CreakTask(u8 Prio,u16 Size,CPU_STK *Task_STK,OS_TCB *TaskTCB,OS_TASK_PTR TaskFun);

//ɾ������
//OSTaskDel((OS_TCB*)&Test2TCB,&err);


#endif


