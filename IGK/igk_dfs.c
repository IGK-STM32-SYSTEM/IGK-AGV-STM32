#include "igk_dfs.h"
#include "igk_map.h"
/*--------------------------内部变量-------------------------------------*/
//当前结点，即为栈顶的结点【内部变量】
u16 cur_node;
//每个结点的下一个邻接点，即刚从栈顶弹出的结点【内部变量】
int map_next[NodeMaxNum]={0};
//定义邻居栈,动态加载当前点的所有邻居节点【内部变量】
STACK neighbourStack;
//定义搜索主栈【搜索过程使用的栈】【内部变量】
STACK stack;
//保存结点状态的数组
u16 states[NodeMaxNum];
//临时路径节点数【内部变量】
u16 TempPathNodeCount = 0;
//临时路径节点列表【内部变量】
u16 TempPathNodeList[NodeMaxNum]={0};
/*--------------------------公共变量-------------------------------------*/
//最优路径结构体变量【公共变量】
IgkStuct_BestPath BestPath;

//数组倒叙方法
void Reverse(u16 *p , u16 size)
{
	int i,tmp;
	for(i = 0 ; i < size/2 ; i++)
	{
		tmp = p[i] ; 
		p[i] = p[size-1-i] ; 
		p[size-1-i] = tmp ;  
	}    
}

//找邻居节点(目前采用的是直接从Flash读取当前点的地图信息)
PNODE neighbour(int a){
	//删除邻居栈
	DeleteStack(&neighbourStack);
	//初始化neighbourStack
	InitStack(&neighbourStack);
	//读取对应站点的地图
	StaionMapStruct mapStruct;
	ReadToMapStruct(a,&mapStruct);
	for(int j=0;j<StationMapType;j++)
	{
		//邻接点必须大于0，并且小于节点最大值
		if(mapStruct.Stop[j]>0&&mapStruct.Stop[j] < NodeMaxNum)
		{
			PushStack(&neighbourStack,mapStruct.Stop[j]);
		}
	}
	//建立一个临时节点指针，初始化时指向栈顶
	PNODE P = neighbourStack.PTOP;
	//结点P不空
	while (P != NULL){
		//邻居节点已经在栈stack里了
		if( states[P->Element]==1 )
		{
			//继续找下一个邻居节点 
			P=P->Next;
		}
		//不在栈stack里
		else
		{
			//就要P作为返回值  如果a是最栈顶节点 
			if(map_next[a] == -1)
			{
				while(P != NULL && states[P->Element]==1)
				{
					P = P->Next;
				}
				return P;
			}
			//判断当前节点是否是a节点的邻居节点
			else if(P->Element == map_next[a])   
			{
				P=P->Next;
				while(P != NULL && states[P->Element]==1)
				{
					P = P->Next;
				}
				return P;
			}
			else
			{
				P=P->Next;
			}
		}
	}
	return NULL;
}

//搜索路径
void FindRoute(u16 start,u16 end){
	//清空变量
	memset(BestPath.NodeList,0,sizeof(BestPath.NodeList));
	memset(TempPathNodeList,0,sizeof(TempPathNodeList));
	BestPath.NodeCount = 0;
	TempPathNodeCount = 0;
  BestPath.PathTotal = 0;
	memset(states,0,NodeMaxNum);
	memset(map_next,0,NodeMaxNum);
	/*--------------循环遍历搜索--------------------*/
	//当前路径号
	int curPath = 0;
	//初始化stack
	InitStack(&stack);
	//初始化map_next
	for(int i=0;i<NodeMaxNum;i++)
	{
		 map_next[i] = -1;
	}
	//将起点入栈
	PushStack(&stack,start);	
	//将起点标记为入栈状态
	states[start]=1;
	//栈不为空
	while(!IsEmpty(&stack)) {
		//到达终点
		if (stack.PTOP->Element == end)
		{
			//路径号加1
			curPath++;
			
			/*-----------------------提取路径到临时列表，并倒叙处理---------------------------*/

			//建立一个临时节点指针，初始化时指向栈顶
			PNODE P = stack.PTOP;
			int i = 0;
			//清空临时路径变量
			memset(TempPathNodeList,0,NodeMaxNum);
			TempPathNodeCount = 0;
			//将结果从栈中提取到临时路径列表
			while(P->Element != NULL)
			{
				//将结果放入临时路径列表
				TempPathNodeList[i++]=P->Element;
				//路径节点数加1
				TempPathNodeCount+=1;
				P=P->Next;
			}
			//将路径倒序
			Reverse(TempPathNodeList,TempPathNodeCount);
			
			/*------------------------打印路径---------------------------*/
			
			IGK_SysTimePrintln("第:[%d]条路径,节点数:[%d]",curPath,TempPathNodeCount);
			for(int i=0;i < TempPathNodeCount;i++)
				if(i == TempPathNodeCount-1)
					IGK_SysPrintf("%d\r\n",TempPathNodeList[i]);
				else
					IGK_SysPrintf("%d-",TempPathNodeList[i]);
			
			/*----------------------选择最有路径-------------------------*/
			
			//如果是第一条路径
			if(curPath == 1){
				//将路径复制到最有路径列表
				for(int i=0;i < TempPathNodeCount;i++)
					BestPath.NodeList[i] = TempPathNodeList[i];
				//复制节点数
				BestPath.NodeCount = TempPathNodeCount;
			}
			//如果不是第一条
			else{
				//如果当前路径更优，则更新最优路径信息
				if(TempPathNodeCount < BestPath.NodeCount){
					//将路径复制到最有路径列表
					for(int i=0;i < TempPathNodeCount;i++)
						BestPath.NodeList[i] = TempPathNodeList[i];
					//复制节点数
					BestPath.NodeCount = TempPathNodeCount;
				}
			}
			/*-------------------------弹出栈顶，清除终点标志，准备下一次搜索---------------------------------*/
			//将栈顶结点弹出
			PopStack(&stack);
			//清除终点的状态
			states[end]=0;
			map_next[end]=-1;
		}
		//未到达终点
		else{
			cur_node=stack.PTOP->Element;
			PNODE d =neighbour(cur_node);
			//邻居不为空
			if(d != NULL){
				map_next[cur_node] = d->Element;
				cur_node=d->Element;
				PushStack(&stack,cur_node);
				states[cur_node]=1;
			}
			else{
				PopStack(&stack);//将栈顶结点弹出
				states[cur_node]=0;//删除的栈顶节点states置为0  通过该节点到不了目的地 
				map_next[cur_node] = -1;
			}
		}
	}
	//删除,并释放内存
	DeleteStack(&stack);
	DeleteStack(&neighbourStack);
	BestPath.PathTotal = curPath;
}

