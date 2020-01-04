#ifndef __IGK_STACK_H
#define __IGK_STACK_H
#include "sys.h"

typedef u16 Elementtype;    //    ������������

//����ڵ�
typedef struct Node {
    Elementtype Element;
    struct Node  * Next;
}NODE, *PNODE;

//����ջ�ṹ��
typedef struct Stack {
    PNODE PTOP;        //����ջ���ڵ�
    PNODE PBOTTOM;     //����ջ�׽ڵ�
}STACK,* PSTACK;

//��������
void InitStack(PSTACK Stack);            //��ʼ��ջ
void PushStack(PSTACK Stack, int val);   //��ջ����
void PopStack(PSTACK Stack);             //��ջ����
void TraverseStack(PSTACK Stack);        //����ջ����
u16 IsEmpty(PSTACK Stack);               //�ж�ջ�Ƿ�Ϊ�պ���
void ClearStack(PSTACK Stack);           //���ջ����
void DeleteStack(PSTACK Stack);          //ɾ��ջ���Զ��ͷ��ڴ�

#endif
