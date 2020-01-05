#ifndef __IGK_DFS_H
#define __IGK_DFS_H
#include "igk_stack.h"
#include "usart.h"

//节点总数
#define NodeNum 100
//定义邻接关系栈
extern STACK MapSTACK[NodeNum];

//定义路径结果缓冲区
//最优路径节点列表【目前以节点数最少为最优路径】【公共变量】
extern u16 BestPathNodeList[NodeNum];
//最优路径节点数【公共变量】
extern u16 BestPathNodeCount;
//可执行路径总数【公共变量】
extern u16 PathTotal;

void FindRoute(u16 start,u16 end);

//使用示例
//注意：起始点从1开始，不能从0开始
//初始化栈
//for(int i=0;i<NodeNum;i++){
//	InitStack(&MapSTACK[i]);
//}
//初始化节点邻接关系
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
//搜索路径
//	FindRoute(1,4);

#endif
