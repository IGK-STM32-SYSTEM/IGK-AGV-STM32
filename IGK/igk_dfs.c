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

//����·�����������
//����·���ڵ��б�Ŀǰ�Խڵ�������Ϊ����·����������������
u16 BestPathNodeList[NodeNum]={0};
//����·���ڵ���������������
u16 BestPathNodeCount = 0;
//��ʱ·���ڵ��б��ڲ�������
u16 TempPathNodeList[NodeNum]={0};
//��ʱ·���ڵ������ڲ�������
u16 TempPathNodeCount = 0;

//��ִ��·������������������
u16 PathTotal = 0;

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
	//1.����һ����ջ
	STACK stackTemp;

//���ھӽڵ�
u16 neighbourV(int a){
	u16 value=0;
	//2.ջ��ʼ��
	InitStack(&stackTemp);
	//��ȡ��Ӧվ��ĵ�ͼ
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
				value = P->Element;
				return value;
			}
			//�жϵ�ǰ�ڵ��Ƿ���a�ڵ���ھӽڵ�
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
//	//����һ����ʱ�ڵ�ָ�룬��ʼ��ʱָ��ջ��
//	PNODE P = MapSTACK[a].PTOP;
//	//���P����
//	while (P != NULL){
//		//�ھӽڵ��Ѿ���ջstack����
//		if( states[P->Element]==1 )
//		{
//			//��������һ���ھӽڵ� 
//			P=P->Next;
//		}
//		//����ջstack��
//		else
//		{
//			//��ҪP��Ϊ����ֵ  ���a����ջ���ڵ� 
//			if(map_next[a] == -1)
//			{
//				while(P != NULL && states[P->Element]==1)
//				{
//					P = P->Next;
//				}
//				return P;
//			}
//			//�жϵ�ǰ�ڵ��Ƿ���a�ڵ���ھӽڵ�
//			else if(P->Element == map_next[a])   
//			{
//				P=P->Next;
//				while(P != NULL && states[P->Element]==1)
//				{
//					P = P->Next;
//				}
//				return P; //��P����һ����㷵��
//			}
//			else
//			{
//				P=P->Next;
//			}
//		}
//	}
//	return NULL;
//}

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
	//��ձ���
	memset(BestPathNodeList,0,NodeNum);
	memset(TempPathNodeList,0,NodeNum);
	BestPathNodeCount = 0;
	TempPathNodeCount = 0;
  PathTotal = 0;
	memset(states,0,NodeNum);
	memset(map_next,0,NodeNum);
	/*--------------ѭ����������--------------------*/
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
			memset(TempPathNodeList,0,NodeNum);
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
					BestPathNodeList[i] = TempPathNodeList[i];
				//���ƽڵ���
				BestPathNodeCount = TempPathNodeCount;
			}
			//������ǵ�һ��
			else{
				//�����ǰ·�����ţ����������·����Ϣ
				if(TempPathNodeCount < BestPathNodeCount){
					//��·�����Ƶ�����·���б�
					for(int i=0;i < TempPathNodeCount;i++)
						BestPathNodeList[i] = TempPathNodeList[i];
					//���ƽڵ���
					BestPathNodeCount = TempPathNodeCount;
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
			u16 vv = neighbourV(cur_node);
			//�ھӲ�Ϊ��
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
				PopStack(&stack);//��ջ����㵯��
				states[cur_node]=0;//ɾ����ջ���ڵ�states��Ϊ0  ͨ���ýڵ㵽����Ŀ�ĵ� 
				map_next[cur_node] = -1;
			}
			DeleteStack(&stackTemp);
		}
	}
	//ɾ��,���ͷ��ڴ�
	DeleteStack(&stack);
	for(int i=0;i<NodeNum;i++){
		DeleteStack(&MapSTACK[i]);
	}
	PathTotal = curPath;
}

