#ifndef __IGK_STACK_H
#define __IGK_STACK_H
#include "sys.h"

typedef u16 Elementtype;    //    定义数据类型

//定义节点
typedef struct Node {
    Elementtype Element;
    struct Node  * Next;
}NODE, *PNODE;

//定义栈结构体
typedef struct Stack {
    PNODE PTOP;        //定义栈顶节点
    PNODE PBOTTOM;     //定义栈底节点
}STACK,* PSTACK;

//函数声明
void InitStack(PSTACK Stack);            //初始化栈
void PushStack(PSTACK Stack, int val);   //入栈函数
void PopStack(PSTACK Stack);             //出栈函数
void TraverseStack(PSTACK Stack);        //遍历栈函数
u16 IsEmpty(PSTACK Stack);               //判断栈是否为空函数
void ClearStack(PSTACK Stack);           //清空栈函数
void DeleteStack(PSTACK Stack);          //删除栈，自动释放内存

#endif
