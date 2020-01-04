#ifndef __IGK_DFS_H
#define __IGK_DFS_H
#include "igk_stack.h"
#include "usart.h"

//�ڵ�����
#define NodeNum 100
//�����ڽӹ�ϵջ
extern STACK MapSTACK[NodeNum];

//·���ڵ�����
//Ĭ���������10��·��,�û�����ͨ��Ȩֵ����ѡ��һ�����ʵ�·��
extern u16 PathArry[10][NodeNum];//N:A->B->C...
//ÿ��·���Ľڵ���
extern u16 PathNodeNum[10];
//��¼·������
extern u16 PathTotal;

void FindRoute(u16 start,u16 end);

//ʹ��ʾ��
//ע�⣺��ʼ���1��ʼ�����ܴ�0��ʼ
//��ʼ��ջ
//for(int i=0;i<NodeNum;i++){
//	InitStack(&MapSTACK[i]);
//}
//��ʼ���ڵ��ڽӹ�ϵ
//  0-->1  0-->2
//	PushStack(&MapSTACK[0],1);
//	PushStack(&MapSTACK[0],2);
//	//1-->2  1-->3  1-->4
//	PushStack(&MapSTACK[1],2);
//	PushStack(&MapSTACK[1],3);
//	PushStack(&MapSTACK[1],4);
//	//2-->4
//	PushStack(&MapSTACK[2],4);		
//	//3-->4  3-->5
//	PushStack(&MapSTACK[3],4);	
//	PushStack(&MapSTACK[3],5);		
//	//4-->5
//	PushStack(&MapSTACK[4],5);	
//����·��
//	FindRoute(1,4);

#endif
