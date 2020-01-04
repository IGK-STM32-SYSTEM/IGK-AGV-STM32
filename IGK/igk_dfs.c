#include "igk_dfs.h"
#include "igk_map.h"
//��ǰ��㣬��Ϊջ���Ľ��
u16 cur_node;
//ÿ��������һ���ڽӵ㣬���մ�ջ�������Ľ��
int map_next[NodeNum]={0};
//�����ڽӹ�ϵջ
STACK MapSTACK[NodeNum];
STACK stack;
//������״̬������
u16 states[NodeNum];

//·���ڵ�����
//Ĭ���������10��·��,�û�����ͨ��Ȩֵ����ѡ��һ�����ʵ�·��
u16 PathArry[10][NodeNum];//N:A->B->C...
//ÿ��·���Ľڵ���
u16 PathNodeNum[10];
//��¼·������
u16 PathTotal=0;

//���鵹��
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

//���ھӽڵ�
PNODE neighbour(int a){
	//����һ����ʱ�ڵ�ָ�룬��ʼ��ʱָ��ջ��
	PNODE P = MapSTACK[a].PTOP;
	//���P����
	while (P != NULL){
		//�ھӽڵ��Ѿ���ջstack����
		if( states[P->Element]==1 )
		{
			//��������һ���ھӽڵ� 
			P=P->Next;
		}
		//����ջstack��
		else
		{
			//��ҪP��Ϊ����ֵ  ���a����ջ���ڵ� 
			if(map_next[a] == -1)
			{
				while(P != NULL && states[P->Element]==1)
				{
					P = P->Next;
				}
				return P;
			}
			//�жϵ�ǰ�ڵ��Ƿ���a�ڵ���ھӽڵ�
			else if(P->Element == map_next[a])   
			{
				P=P->Next;
				while(P != NULL && states[P->Element]==1)
				{
					P = P->Next;
				}
				return P; //��P����һ����㷵��
			}
			else
			{
				P=P->Next;
			}
		}
	}
	return NULL;
}
//���ھӽڵ�ֵ
u16 NeighbourData(int a){
	//������ʱ����,���ڴ洢�����ؽ��
	u16 data = 0;
	//1.����һ����ջ
	STACK stackTemp;
	//2.ջ��ʼ��
	InitStack(&stackTemp);
	//3.��ȡ��ͼ�����ջ
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
	
	//����һ����ʱ�ڵ�ָ�룬��ʼ��ʱָ��ջ��
	PNODE P = stackTemp.PTOP;
	//���P����
	while (P != NULL){
		//�ھӽڵ��Ѿ���ջstack����
		if( states[P->Element]==1 )
		{
			//��������һ���ھӽڵ� 
			P=P->Next;
		}
		//����ջstack��
		else
		{
			//��ҪP��Ϊ����ֵ  ���a����ջ���ڵ� 
			if(map_next[a] == -1)
			{
				while(P != NULL && states[P->Element]==1)
				{
					P = P->Next;
				}
				data = P->Element;
				//ɾ��ջ
				DeleteStack(&stackTemp);
				return data;
			}
			//�жϵ�ǰ�ڵ��Ƿ���a�ڵ���ھӽڵ�
			else if(P->Element == map_next[a])   
			{
				P=P->Next;
				while(P != NULL && states[P->Element]==1)
				{
					P = P->Next;
				}
				//��P����һ����㷵��
				data = P->Element;
				//ɾ��ջ
				DeleteStack(&stackTemp);
				return data;
			}
			else
			{
				P=P->Next;
			}
		}
	}
	//4.��ɺ��ͷ�ջ�ռ�
	DeleteStack(&stackTemp);
	return NULL;
}


//����·��
void FindRoute(u16 start,u16 end){
	//��ս���Ĵ���
	for(int i=0;i<10;i++)
		memset(PathArry[i],0,NodeNum);
	memset(PathNodeNum,0,10);
	memset(states,0,NodeNum);
	memset(map_next,0,NodeNum);
	//��ǰ·����
	int curPath = 0;
	//��ʼ��stack
	InitStack(&stack);
	//��ʼ��map_next
	for(int i=0;i<NodeNum;i++)
	{
		 map_next[i] = -1;
	}
	//�������ջ
	PushStack(&stack,start);	
	//�������Ϊ��ջ״̬
	states[start]=1;
	//ջ��Ϊ��
	while(!IsEmpty(&stack)) {
		if (stack.PTOP->Element == end){
			//����һ����ʱ�ڵ�ָ�룬��ʼ��ʱָ��ջ��
			PNODE P = stack.PTOP;
			int i = 0;
			while(P->Element != start){
				//���������PathArry����
				PathArry[curPath][i++]=P->Element;
				//·���ڵ�����1
				PathNodeNum[curPath]+=1;
				P=P->Next;
			}
			//���������PathArry����
			PathArry[curPath][i++]=P->Element;
			//·���ڵ�����1
			PathNodeNum[curPath]+=1;
			//��·������
			Reverse(PathArry[curPath],PathNodeNum[curPath]);
			//��ӡ·��
			IGK_SysPrintf("·����:%d\r\n",curPath);
			IGK_SysPrintf("�ڵ���:%d\r\n",PathNodeNum[curPath]);
			for(int i=0;i < PathNodeNum[curPath];i++)
				if(i==PathNodeNum[curPath]-1)
					IGK_SysPrintf("%d",PathArry[curPath][i]);
				else
					IGK_SysPrintf("%d-",PathArry[curPath][i]);
			IGK_SysPrintf("\r\n");
			//·���ż�1
			curPath++;
			
			PopStack(&stack);//��ջ����㵯��
			states[end]=0;//����յ��״̬
			map_next[end]=-1;
		}
		else{
			cur_node=stack.PTOP->Element;
			//�ھӲ�Ϊ��
			if(neighbour(cur_node) != NULL){
				PNODE d =neighbour(cur_node);
				//������0ʱ������ѭ��
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
				PopStack(&stack);//��ջ����㵯��
				states[cur_node]=0;//ɾ����ջ���ڵ�states��Ϊ0  ͨ���ýڵ㵽����Ŀ�ĵ� 
				map_next[cur_node] = -1;
			}
		}
	}
	//ɾ��,���ͷ��ڴ�
	DeleteStack(&stack);
	for(int i=0;i<NodeNum;i++){
		DeleteStack(&MapSTACK[i]);
	}
	PathTotal = curPath;
}

