#ifndef __IGK_DFS_H
#define __IGK_DFS_H
#include "igk_stack.h"
#include "usart.h"

//节点总数
#define NodeMaxNum 100
//最优路径结构体
typedef struct {
	u16 NodeList[NodeMaxNum];//最优路径节点列表【目前以节点数最少为最优路径】
	u16 NodeCount;//最优路径节点数
	u16 PathTotal;//可执行路径总数
}IgkStuct_BestPath;

/*--------------------------公共变量-------------------------------------*/
//最优路径结构体变量【公共变量】
extern IgkStuct_BestPath BestPath;

//搜索函数，搜索结果存在公共变量中
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
