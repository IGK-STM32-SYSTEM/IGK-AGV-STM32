#ifndef __IGK_UCOS_H
#define __IGK_UCOS_H
#include "sys.h"

//UCOS��ʼ��
extern void UCOS_Init(void);
//��������
extern void CreakTask(u8 Prio,u16 Size,OS_TCB *TaskTCB,OS_TASK_PTR TaskFun);
#endif
