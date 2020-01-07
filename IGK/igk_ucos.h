#ifndef __IGK_UCOS_H
#define __IGK_UCOS_H
#include "sys.h"

//UCOS初始化
extern void UCOS_Init(void);
/*创建任务
Prio:     任务优先级
Size:     任务堆栈大小
Task_STK: 任务堆栈基地址
TaskTCB:  任务控制块
TaskFun:  任务函数
*/
extern void CreakTask(u8 Prio,u16 Size,CPU_STK *Task_STK,OS_TCB *TaskTCB,OS_TASK_PTR TaskFun);

//删除任务
//OSTaskDel((OS_TCB*)&Test2TCB,&err);


#endif


