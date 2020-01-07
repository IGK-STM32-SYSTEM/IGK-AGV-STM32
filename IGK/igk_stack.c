#include "igk_stack.h"
#include "malloc.h"

//定义初始化栈函数
void InitStack(PSTACK Stack) {
	//新建一个节点
	//PNODE PNew = (PNODE)malloc(sizeof(NODE)); 
	PNODE PNew = (PNODE)mymalloc(SRAMIN,sizeof(NODE)); 
	//判断内存是否分配成功  
	if(PNew == NULL){        
		//my_printf(USART2,"新节点空间分配失败！\r\n");
		return;
	}
	//栈顶指针指向新节点
	Stack->PTOP = PNew;
	//栈底指针指向新节点
	Stack->PBOTTOM = PNew;
	//新节点指针指向为空
	PNew->Next = NULL;
	//my_printf(USART2,"栈创建成功！\r\n");
}

//定义入栈函数
//从栈顶插入数值val
void PushStack(PSTACK Stack,int val) {
	//创建新的节点用于存放变量
//	PNODE P = (PNODE)malloc(sizeof(NODE));
	PNODE P = (PNODE)mymalloc(SRAMIN,sizeof(NODE));
	if (P == NULL) {
		//my_printf(USART2,"分配空间内存失败！\r\n");
		return;
	}
	//变量赋值给节点的数据域
	P->Element = val;
	//让新建的节点指向上一个节点    
	P->Next=Stack->PTOP;
	//更新顶部节点，使其指向新建的节点    
	Stack->PTOP = P;
	//my_printf(USART2,"%d 入栈成功！\r\n",val);
}

//定义出栈函数
//从栈顶弹出，并将地址赋值给变量val
void PopStack(PSTACK Stack) {
	//判断栈是否为空
	if (IsEmpty(Stack)) {    
		//my_printf(USART2,"出栈失败，栈为空！\r\n"); 
		return;
	}        
	//建立一个临时节点指向栈顶节点
	PNODE P = Stack->PTOP; 
	//把栈顶指针指向上一个栈节点
	Stack->PTOP = P->Next;
	//释放节点内存，防止内存空间泄漏
	myfree(SRAMIN,P);  
	//防止产生野指针  
	P = NULL;   
	//my_printf(USART2,"已将%d从栈弹出！\r\n", *val);
}

//定义判断栈是否为空函数
u16 IsEmpty(PSTACK Stack) {
	if (Stack->PBOTTOM == Stack->PTOP)
	{
		//my_printf(USART2,"栈为空！\r\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

//定义栈的遍历函数
void TraverseStack(PSTACK Stack) {
	//判断栈是否为空
	if (IsEmpty(Stack)) {        
		//my_printf(USART2,"遍历栈失败，栈为空！\r\n");
		return;
	}
	//建立一个临时节点指针，初始化时指向栈顶
	PNODE P = Stack->PTOP;        
	//使临时节点指针从栈顶开始遍历栈，直到栈底
	//my_printf(USART2,"遍历栈的结果为： \r\n");
	while (P != Stack->PBOTTOM) {
		//my_printf(USART2,"%d ", P->Element);//打印出数据
		IGK_SysPrintf("遍历:%d", P->Element);	
		P = P->Next;
	}
	//my_printf(USART2,"\r\n");
}

//定义栈的清空函数
void ClearStack(PSTACK Stack) {
	//判断栈是否为空
	if (IsEmpty(Stack)) { 
		//释放内存
		myfree(SRAMIN,Stack->PBOTTOM);
		return;
	}
	//创建两个节点指针用来释放栈
	PNODE P = Stack->PTOP;
	PNODE Q = NULL;
	//清空栈
	while (P != Stack->PBOTTOM) {
			Q = P->Next;
			myfree(SRAMIN,P);
			P = Q;
	}
	//栈顶指向栈底指向的初始空节点
	Stack->PTOP = Stack->PBOTTOM; 
	//释放内存
	myfree(SRAMIN,Stack->PBOTTOM);	
}

//删除栈，自动释放内存
void DeleteStack(PSTACK Stack) {
	//清空
	ClearStack(Stack);
	//释放内存
	myfree(SRAMIN,Stack->PBOTTOM);	
}



