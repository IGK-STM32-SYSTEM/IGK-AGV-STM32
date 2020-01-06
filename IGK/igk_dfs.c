#include "igk_dfs.h"
#include "igk_map.h"
/*--------------------------�ڲ�����-------------------------------------*/
//��ǰ��㣬��Ϊջ���Ľ�㡾�ڲ�������
u16 cur_node;
//ÿ��������һ���ڽӵ㣬���մ�ջ�������Ľ�㡾�ڲ�������
int map_next[NodeMaxNum]={0};
//�����ھ�ջ,��̬���ص�ǰ��������ھӽڵ㡾�ڲ�������
STACK neighbourStack;
//����������ջ����������ʹ�õ�ջ�����ڲ�������
STACK stack;
//������״̬������
u16 states[NodeMaxNum];
//��ʱ·���ڵ������ڲ�������
u16 TempPathNodeCount = 0;
//��ʱ·���ڵ��б��ڲ�������
u16 TempPathNodeList[NodeMaxNum]={0};
/*--------------------------��������-------------------------------------*/
//����·���ṹ�����������������
IgkStuct_BestPath BestPath;

//���鵹�𷽷�
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

//���ھӽڵ�(Ŀǰ���õ���ֱ�Ӵ�Flash��ȡ��ǰ��ĵ�ͼ��Ϣ)
PNODE neighbour(int a){
	//ɾ���ھ�ջ
	DeleteStack(&neighbourStack);
	//��ʼ��neighbourStack
	InitStack(&neighbourStack);
	//��ȡ��Ӧվ��ĵ�ͼ
	StaionMapStruct mapStruct;
	ReadToMapStruct(a,&mapStruct);
	for(int j=0;j<StationMapType;j++)
	{
		//�ڽӵ�������0������С�ڽڵ����ֵ
		if(mapStruct.Stop[j]>0&&mapStruct.Stop[j] < NodeMaxNum)
		{
			PushStack(&neighbourStack,mapStruct.Stop[j]);
		}
	}
	//����һ����ʱ�ڵ�ָ�룬��ʼ��ʱָ��ջ��
	PNODE P = neighbourStack.PTOP;
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

//����·��
void FindRoute(u16 start,u16 end){
	//��ձ���
	memset(BestPath.NodeList,0,sizeof(BestPath.NodeList));
	memset(TempPathNodeList,0,sizeof(TempPathNodeList));
	BestPath.NodeCount = 0;
	TempPathNodeCount = 0;
  BestPath.PathTotal = 0;
	memset(states,0,NodeMaxNum);
	memset(map_next,0,NodeMaxNum);
	/*--------------ѭ����������--------------------*/
	//��ǰ·����
	int curPath = 0;
	//��ʼ��stack
	InitStack(&stack);
	//��ʼ��map_next
	for(int i=0;i<NodeMaxNum;i++)
	{
		 map_next[i] = -1;
	}
	//�������ջ
	PushStack(&stack,start);	
	//�������Ϊ��ջ״̬
	states[start]=1;
	//ջ��Ϊ��
	while(!IsEmpty(&stack)) {
		//�����յ�
		if (stack.PTOP->Element == end)
		{
			//·���ż�1
			curPath++;
			
			/*-----------------------��ȡ·������ʱ�б���������---------------------------*/

			//����һ����ʱ�ڵ�ָ�룬��ʼ��ʱָ��ջ��
			PNODE P = stack.PTOP;
			int i = 0;
			//�����ʱ·������
			memset(TempPathNodeList,0,NodeMaxNum);
			TempPathNodeCount = 0;
			//�������ջ����ȡ����ʱ·���б�
			while(P->Element != NULL)
			{
				//�����������ʱ·���б�
				TempPathNodeList[i++]=P->Element;
				//·���ڵ�����1
				TempPathNodeCount+=1;
				P=P->Next;
			}
			//��·������
			Reverse(TempPathNodeList,TempPathNodeCount);
			
			/*------------------------��ӡ·��---------------------------*/
			
			IGK_SysTimePrintln("��:[%d]��·��,�ڵ���:[%d]",curPath,TempPathNodeCount);
			for(int i=0;i < TempPathNodeCount;i++)
				if(i == TempPathNodeCount-1)
					IGK_SysPrintf("%d\r\n",TempPathNodeList[i]);
				else
					IGK_SysPrintf("%d-",TempPathNodeList[i]);
			
			/*----------------------ѡ������·��-------------------------*/
			
			//����ǵ�һ��·��
			if(curPath == 1){
				//��·�����Ƶ�����·���б�
				for(int i=0;i < TempPathNodeCount;i++)
					BestPath.NodeList[i] = TempPathNodeList[i];
				//���ƽڵ���
				BestPath.NodeCount = TempPathNodeCount;
			}
			//������ǵ�һ��
			else{
				//�����ǰ·�����ţ����������·����Ϣ
				if(TempPathNodeCount < BestPath.NodeCount){
					//��·�����Ƶ�����·���б�
					for(int i=0;i < TempPathNodeCount;i++)
						BestPath.NodeList[i] = TempPathNodeList[i];
					//���ƽڵ���
					BestPath.NodeCount = TempPathNodeCount;
				}
			}
			/*-------------------------����ջ��������յ��־��׼����һ������---------------------------------*/
			//��ջ����㵯��
			PopStack(&stack);
			//����յ��״̬
			states[end]=0;
			map_next[end]=-1;
		}
		//δ�����յ�
		else{
			cur_node=stack.PTOP->Element;
			PNODE d =neighbour(cur_node);
			//�ھӲ�Ϊ��
			if(d != NULL){
				map_next[cur_node] = d->Element;
				cur_node=d->Element;
				PushStack(&stack,cur_node);
				states[cur_node]=1;
			}
			else{
				PopStack(&stack);//��ջ����㵯��
				states[cur_node]=0;//ɾ����ջ���ڵ�states��Ϊ0  ͨ���ýڵ㵽����Ŀ�ĵ� 
				map_next[cur_node] = -1;
			}
		}
	}
	//ɾ��,���ͷ��ڴ�
	DeleteStack(&stack);
	DeleteStack(&neighbourStack);
	BestPath.PathTotal = curPath;
}

