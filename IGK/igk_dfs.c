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

//路径节点序列
//默认最多搜索10条路径,用户可以通过权值配置选择一条合适的路径
u16 PathArry[10][NodeNum];//N:A->B->C...
//每条路径的节点数
u16 PathNodeNum[10];
//记录路径总数
u16 PathTotal=0;

//数组倒叙
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

//找邻居节点
PNODE neighbour(int a){
	//建立一个临时节点指针，初始化时指向栈顶
	PNODE P = MapSTACK[a].PTOP;
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
				return P; //将P的下一个结点返回
			}
			else
			{
				P=P->Next;
			}
		}
	}
	return NULL;
}
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
	//清空结果寄存器
	for(int i=0;i<10;i++)
		memset(PathArry[i],0,NodeNum);
	memset(PathNodeNum,0,10);
	memset(states,0,NodeNum);
	memset(map_next,0,NodeNum);
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
		if (stack.PTOP->Element == end){
			//建立一个临时节点指针，初始化时指向栈顶
			PNODE P = stack.PTOP;
			int i = 0;
			while(P->Element != start){
				//将结果放入PathArry缓存
				PathArry[curPath][i++]=P->Element;
				//路径节点数加1
				PathNodeNum[curPath]+=1;
				P=P->Next;
			}
			//将结果放入PathArry缓存
			PathArry[curPath][i++]=P->Element;
			//路径节点数加1
			PathNodeNum[curPath]+=1;
			//将路径倒序
			Reverse(PathArry[curPath],PathNodeNum[curPath]);
			//打印路径
			IGK_SysPrintf("路径号:%d\r\n",curPath);
			IGK_SysPrintf("节点数:%d\r\n",PathNodeNum[curPath]);
			for(int i=0;i < PathNodeNum[curPath];i++)
				if(i==PathNodeNum[curPath]-1)
					IGK_SysPrintf("%d",PathArry[curPath][i]);
				else
					IGK_SysPrintf("%d-",PathArry[curPath][i]);
			IGK_SysPrintf("\r\n");
			//路径号加1
			curPath++;
			
			PopStack(&stack);//将栈顶结点弹出
			states[end]=0;//清除终点的状态
			map_next[end]=-1;
		}
		else{
			cur_node=stack.PTOP->Element;
			//邻居不为空
			if(neighbour(cur_node) != NULL){
				PNODE d =neighbour(cur_node);
				//当出现0时会无限循环
				if(d->Element == 0)
					break;
				map_next[cur_node] = d->Element;
				cur_node=d->Element;
				PushStack(&stack,cur_node);
				states[cur_node]=1;
			}
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
		}
	}
	//删除,并释放内存
	DeleteStack(&stack);
	for(int i=0;i<NodeNum;i++){
		DeleteStack(&MapSTACK[i]);
	}
	PathTotal = curPath;
}

