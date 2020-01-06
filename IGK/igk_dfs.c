#include "igk_dfs.h"
#include "igk_map.h"
//当前结点，即为栈顶的结点
u16 cur_node;
//每个结点的下一个邻接点，即刚从栈顶弹出的结点
int map_next[NodeNum]={0};
//定义邻接关系栈
STACK MapSTACK[NodeNum];
STACK stack;
//保存结点状态的数组
u16 states[NodeNum];

//定义路径结果缓冲区
//最优路径节点列表【目前以节点数最少为最优路径】【公共变量】
u16 BestPathNodeList[NodeNum]={0};
//最优路径节点数【公共变量】
u16 BestPathNodeCount = 0;
//临时路径节点列表【内部变量】
u16 TempPathNodeList[NodeNum]={0};
//临时路径节点数【内部变量】
u16 TempPathNodeCount = 0;

//可执行路径总数【公共变量】
u16 PathTotal = 0;

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
	//1.创建一个空栈
	STACK stackTemp;

//找邻居节点
u16 neighbourV(int a){
	u16 value=0;
	//2.栈初始化
	InitStack(&stackTemp);
	//读取对应站点的地图
	StaionMapStruct mapStruct;
	ReadToMapStruct(a,&mapStruct);
	for(int j=0;j<StationMapType;j++)
	{
		if(mapStruct.Stop[j]>0)
		{
			PushStack(&stackTemp,mapStruct.Stop[j]);
			//IGK_SysTimePrintln("%d->%d!",i,mapStruct.Stop[j]);
		}
	}
	//建立一个临时节点指针，初始化时指向栈顶
	PNODE P = stackTemp.PTOP;
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
				value = P->Element;
				return value;
			}
			//判断当前节点是否是a节点的邻居节点
			else if(P->Element == map_next[a])   
			{
				P=P->Next;
				while(P != NULL && states[P->Element]==1)
				{
					P = P->Next;
				}
				value = P->Element;
				
				return value;
			}
			else
			{
				P=P->Next;
			}
		}
	}
	return NULL;
}
//PNODE neighbour(int a){
//	//建立一个临时节点指针，初始化时指向栈顶
//	PNODE P = MapSTACK[a].PTOP;
//	//结点P不空
//	while (P != NULL){
//		//邻居节点已经在栈stack里了
//		if( states[P->Element]==1 )
//		{
//			//继续找下一个邻居节点 
//			P=P->Next;
//		}
//		//不在栈stack里
//		else
//		{
//			//就要P作为返回值  如果a是最栈顶节点 
//			if(map_next[a] == -1)
//			{
//				while(P != NULL && states[P->Element]==1)
//				{
//					P = P->Next;
//				}
//				return P;
//			}
//			//判断当前节点是否是a节点的邻居节点
//			else if(P->Element == map_next[a])   
//			{
//				P=P->Next;
//				while(P != NULL && states[P->Element]==1)
//				{
//					P = P->Next;
//				}
//				return P; //将P的下一个结点返回
//			}
//			else
//			{
//				P=P->Next;
//			}
//		}
//	}
//	return NULL;
//}

//找邻居节点值
u16 NeighbourData(int a){
	//定义临时变量,用于存储带返回结果
	u16 data = 0;
	//1.创建一个空栈
	STACK stackTemp;
	//2.栈初始化
	InitStack(&stackTemp);
	//3.读取地图到这个栈
	StaionMapStruct mapStruct;
	ReadToMapStruct(a,&mapStruct);
	for(int i=0;i<StationMapType;i++)
	{
		if(mapStruct.Stop[i]>0)
		{
			PushStack(&stackTemp,mapStruct.Stop[i]);
			//IGK_SysTimePrintln(p_arg,"%d->%d!",i,mapStruct.Stop[j]);
		}
	}
	
	//建立一个临时节点指针，初始化时指向栈顶
	PNODE P = stackTemp.PTOP;
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
				data = P->Element;
				//删除栈
				DeleteStack(&stackTemp);
				return data;
			}
			//判断当前节点是否是a节点的邻居节点
			else if(P->Element == map_next[a])   
			{
				P=P->Next;
				while(P != NULL && states[P->Element]==1)
				{
					P = P->Next;
				}
				//将P的下一个结点返回
				data = P->Element;
				//删除栈
				DeleteStack(&stackTemp);
				return data;
			}
			else
			{
				P=P->Next;
			}
		}
	}
	//4.完成后释放栈空间
	DeleteStack(&stackTemp);
	return NULL;
}



//搜索路径
void FindRoute(u16 start,u16 end){
	//清空变量
	memset(BestPathNodeList,0,NodeNum);
	memset(TempPathNodeList,0,NodeNum);
	BestPathNodeCount = 0;
	TempPathNodeCount = 0;
  PathTotal = 0;
	memset(states,0,NodeNum);
	memset(map_next,0,NodeNum);
	/*--------------循环遍历搜索--------------------*/
	//当前路径号
	int curPath = 0;
	//初始化stack
	InitStack(&stack);
	//初始化map_next
	for(int i=0;i<NodeNum;i++)
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
			memset(TempPathNodeList,0,NodeNum);
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
					BestPathNodeList[i] = TempPathNodeList[i];
				//复制节点数
				BestPathNodeCount = TempPathNodeCount;
			}
			//如果不是第一条
			else{
				//如果当前路径更优，则更新最优路径信息
				if(TempPathNodeCount < BestPathNodeCount){
					//将路径复制到最有路径列表
					for(int i=0;i < TempPathNodeCount;i++)
						BestPathNodeList[i] = TempPathNodeList[i];
					//复制节点数
					BestPathNodeCount = TempPathNodeCount;
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
			u16 vv = neighbourV(cur_node);
			//邻居不为空
			if(vv != NULL){
				map_next[cur_node] = vv;
				cur_node=vv;
				PushStack(&stack,cur_node);
				states[cur_node]=1;
			}
//			if(neighbour(cur_node) != NULL){
//				PNODE d =neighbour(cur_node);
//				map_next[cur_node] = d->Element;
//				cur_node=d->Element;
//				PushStack(&stack,cur_node);
//				states[cur_node]=1;
//			}
			//			u16 dataTemp = NeighbourData(cur_node);
//			if(dataTemp != NULL){
//				//PNODE d =neighbour(cur_node);
//				map_next[cur_node] = dataTemp;
//				cur_node=dataTemp;
//				PushStack(&stack,cur_node);
//				states[cur_node]=1;
//			}	
			else{
				PopStack(&stack);//将栈顶结点弹出
				states[cur_node]=0;//删除的栈顶节点states置为0  通过该节点到不了目的地 
				map_next[cur_node] = -1;
			}
			DeleteStack(&stackTemp);
		}
	}
	//删除,并释放内存
	DeleteStack(&stack);
	for(int i=0;i<NodeNum;i++){
		DeleteStack(&MapSTACK[i]);
	}
	PathTotal = curPath;
}

