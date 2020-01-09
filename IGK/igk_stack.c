#include "igk_stack.h"
#include "malloc.h"

//�����ʼ��ջ����
void InitStack(PSTACK Stack) {
	//�½�һ���ڵ�
	//PNODE PNew = (PNODE)malloc(sizeof(NODE)); 
	PNODE PNew = (PNODE)mymalloc(SRAMIN,sizeof(NODE)); 
	//�ж��ڴ��Ƿ����ɹ�  
	if(PNew == NULL){        
		//my_printf(USART2,"�½ڵ�ռ����ʧ�ܣ�\r\n");
		return;
	}
	//ջ��ָ��ָ���½ڵ�
	Stack->PTOP = PNew;
	//ջ��ָ��ָ���½ڵ�
	Stack->PBOTTOM = PNew;
	//�½ڵ�ָ��ָ��Ϊ��
	PNew->Next = NULL;
	//my_printf(USART2,"ջ�����ɹ���\r\n");
}

//������ջ����
//��ջ��������ֵval
void PushStack(PSTACK Stack,int val) {
	//�����µĽڵ����ڴ�ű���
//	PNODE P = (PNODE)malloc(sizeof(NODE));
	PNODE P = (PNODE)mymalloc(SRAMIN,sizeof(NODE));
	if (P == NULL) {
		//my_printf(USART2,"����ռ��ڴ�ʧ�ܣ�\r\n");
		return;
	}
	//������ֵ���ڵ��������
	P->Element = val;
	//���½��Ľڵ�ָ����һ���ڵ�    
	P->Next=Stack->PTOP;
	//���¶����ڵ㣬ʹ��ָ���½��Ľڵ�    
	Stack->PTOP = P;
	//my_printf(USART2,"%d ��ջ�ɹ���\r\n",val);
}

//�����ջ����
//��ջ��������������ַ��ֵ������val
void PopStack(PSTACK Stack) {
	//�ж�ջ�Ƿ�Ϊ��
	if (IsEmpty(Stack)) {    
		//my_printf(USART2,"��ջʧ�ܣ�ջΪ�գ�\r\n"); 
		return;
	}        
	//����һ����ʱ�ڵ�ָ��ջ���ڵ�
	PNODE P = Stack->PTOP; 
	//��ջ��ָ��ָ����һ��ջ�ڵ�
	Stack->PTOP = P->Next;
	//�ͷŽڵ��ڴ棬��ֹ�ڴ�ռ�й©
	myfree(SRAMIN,P);  
	//��ֹ����Ұָ��  
	P = NULL;   
	//my_printf(USART2,"�ѽ�%d��ջ������\r\n", *val);
}

//�����ж�ջ�Ƿ�Ϊ�պ���
u16 IsEmpty(PSTACK Stack) {
	if (Stack->PBOTTOM == Stack->PTOP)
	{
		//my_printf(USART2,"ջΪ�գ�\r\n");
		return 1;
	}
	else
	{
		return 0;
	}
}

//����ջ�ı�������
void TraverseStack(PSTACK Stack) {
	//�ж�ջ�Ƿ�Ϊ��
	if (IsEmpty(Stack)) {        
		//my_printf(USART2,"����ջʧ�ܣ�ջΪ�գ�\r\n");
		return;
	}
	//����һ����ʱ�ڵ�ָ�룬��ʼ��ʱָ��ջ��
	PNODE P = Stack->PTOP;        
	//ʹ��ʱ�ڵ�ָ���ջ����ʼ����ջ��ֱ��ջ��
	//my_printf(USART2,"����ջ�Ľ��Ϊ�� \r\n");
	while (P != Stack->PBOTTOM) {
		//my_printf(USART2,"%d ", P->Element);//��ӡ������
		IGK_SysPrintf("����:%d", P->Element);	
		P = P->Next;
	}
	//my_printf(USART2,"\r\n");
}

//����ջ����պ���
void ClearStack(PSTACK Stack) {
	//�ж�ջ�Ƿ�Ϊ��
	if (IsEmpty(Stack)) { 
		//�ͷ��ڴ�
		myfree(SRAMIN,Stack->PBOTTOM);
		return;
	}
	//���������ڵ�ָ�������ͷ�ջ
	PNODE P = Stack->PTOP;
	PNODE Q = NULL;
	//���ջ
	while (P != Stack->PBOTTOM) {
			Q = P->Next;
			myfree(SRAMIN,P);
			P = Q;
	}
	//ջ��ָ��ջ��ָ��ĳ�ʼ�սڵ�
	Stack->PTOP = Stack->PBOTTOM; 
	//�ͷ��ڴ�
	myfree(SRAMIN,Stack->PBOTTOM);	
}

//ɾ��ջ���Զ��ͷ��ڴ�
void DeleteStack(PSTACK Stack) {
	//���
	ClearStack(Stack);
	//�ͷ��ڴ�
	myfree(SRAMIN,Stack->PBOTTOM);	
}



