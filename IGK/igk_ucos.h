#ifndef __IGK_UCOS_H
#define __IGK_UCOS_H
#include "sys.h"

//UCOS初始化
extern void UCOS_Init(void);
//创建任务
extern void CreakTask(u8 Prio,u16 Size,OS_TCB *TaskTCB,OS_TASK_PTR TaskFun);
#endif
