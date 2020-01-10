#include "cdhdata.h"

u8 NormalNum=4;//�����������Ч��ĸ���


//�������ĵ���������
Fencha_struct front_cdh16,back_cdh16,backl_cdh8,backr_cdh8;//����һ��Fencha_struct���͵�ȫ�ֱ���
Fencha_struct findtemp={0,0,0,0,0,0};//�����������еĽṹ�����鶨��



//16λ�ŵ������ݴ�����

//dst:�ŵ�����ԭʼ16���Ƶ�16λ��
//num:����ʱ�����������������1~15��
//dir:�ֲ淽��0��ֱ�У�1����ֲ棬2���ҷֲ�
// ���;��λ���Ҳࣺ��λ
//����ֵ��Fencha_struct�ṹ��
Fencha_struct FindSpace(u16 dst,u8 num,u8 dir)
{
	u16 i=0,dirr=dir;
	u16 p=~dst,numr=0,numl=0;
	Fencha_struct find;
	AA:	
	find.zeroNum = 0;
	find.zeroContinue = 0;
	find.Num = 0;
	find.Distance = 0;
	find.Error = 0;
	
	numr=0;
	numl=0;
	
	//������Ч��ĸ���
	for(i=0;i<16;i++)
	{
		if((p>>i)&0x0001)
		{
			find.Num++;
		}
	}
	//�ж��ұ�0�ĸ���
	for(i=0;i<16;i++)
	{
		if(((p>>i)&0x0001)==0)
		{
			numr++;
		}
		else
		{
			i=16;//ֱ���˳�
		}
	}
	//�ж����0�ĸ���
	for(i=0;i<16;i++)
	{
		if(((p<<i)&0x8000)==0)
		{
			numl++;
		}
		else
		{
			i=16;//ֱ���˳�
		}
	}		
	//����0�ĸ�����������0���ֵ�λ��
	for(i=numr;i<(16-numl);i++)
	{
		if(((p>>i)&0x0001)==0)
		{
			find.zeroLoctaion[find.zeroNum]=i;
			find.zeroNum++;
		}
	}
	//�ж϶ϵ��Ƿ�����
	if(find.zeroNum>0)
	{
		find.zeroContinue = 1;
		if(find.zeroNum>1)
		{
			for(i = 0;i<(find.zeroNum-1);i++)
			{
				if(find.zeroLoctaion[i+1]-find.zeroLoctaion[i]==1)
				{
					continue;
				}
				else
				{
					find.zeroContinue++;
				}
			}
		}
	}
	//������Ч���������λ�õľ���
	if(find.Num==0)//û�ҵ���Ч�㣬���뷵��0
	{
		find.Distance = 0;
		find.Error = 1;		
	}
	else
	{
		if(find.zeroNum>0)//�жϵ㣬����0
		{
			if(find.zeroContinue==1)//һ���ֲ�
			{
				if(dirr==2)//�ҷֲ棬Ĩ����ߣ���λ
				{
					for(i=numl;i<16;i++)
					{
						if((p<<i)&0x8000)
						{
							p = p&(~(0x8000>>i));
						}
						else
						{
							i = 16;
						}
					}
					goto AA;
				}
				else if(dirr==1)//��ֲ�,Ĩ���ұߣ���λ
				{
					for(i=numr;i<16;i++)
					{
						if((p>>i)&0x0001)
						{
							p = p&(~(0x0001<<i));
						}
						else
						{
							i = 16;
						}
					}
					goto AA;
				}
				else if(dirr == 0)//ֱ��
				{
					//Ĩ���ֲ沿��
					if(numl > numr)//�ұ߿յ��٣�Ĩ���ұ�
					{
						for(i=numr;i<16;i++)
						{
							if((p>>i)&0x0001)
							{
								p = p&(~(0x0001<<i));
							}
							else
							{
								i = 16;
							}
						}
					}
					else if(numl < numr)//��߿յ��٣�Ĩ�����
					{
						for(i=numl;i<16;i++)
						{
							if((p<<i)&0x8000)
							{
								p = p&(~(0x8000>>i));
							}
							else
							{
								i = 16;
							}
						}				
						goto AA;
					}
					else if(numl == numr)//����һ���ֱ࣬�Ӿ��д���
					{
						//���ϵ紦ȫ�����
						for(i=numl;i<16-numr;i++)
						{
							//���Ϊ0
							if(((p<<i)&0x8000)==0)
							{
								//����λ��1
								p = p|(0x8000>>i);
							}
						}		
					}
				}
				else
				{
					find.Error = 1;
				}
			}
		}
		else//�޶ϵ㣬�������
		{
			//�ж���Ч���Ƿ񳬹�����ֵ
			if(find.Num>NormalNum)
			{
				//ȫ��ʱ�����������ǹ�ʮ��·�ڵ�ʱ��
				if(find.Num==16)
				{
					find.Distance = 0;
				}
				else
				if(dirr==2)//�ҷֲ�
				{
					//Ĩ����λ�����
					for(i=numl;i<find.Num-NormalNum+numl;i++)
					{
						p = p&(~(0x8000>>i));
					}					
					goto AA;
				}
				else if(dirr==1)//��ֲ�
				{
					//Ĩ����λ�����
					for(i=numr;i<find.Num-NormalNum+numr;i++)
					{
						p = p&(~(0x0001<<i));
					}
					goto AA;
				}
				else if(dirr == 0)//ֱ��
				{
					//Ĩ������ؽϽ��Ķ����
					if(numl > numr)//�ұ߿յ��٣�Ĩ���ұ߶����
					{
						//Ĩ����λ�����
						for(i=numr;i<find.Num-NormalNum+numr;i++)
						{
							p = p&(~(0x0001<<i));
						}
						goto AA;						
					}
					else if(numl < numr)//��߿յ��٣�Ĩ����߶����
					{
						//Ĩ����λ�����
						for(i=numl;i<find.Num-NormalNum+numl;i++)
						{
							p = p&(~(0x8000>>i));
						}
						goto AA;
					}
				}
			}
			else
			if(find.Num<=16)
			{				
				find.Distance = numl - numr;
			}
			else
			{
				find.Error = 1;
			}
		}
	}
	findtemp = find;
	return find;
}
//8λ�ŵ������ݴ�����
//dst:�ŵ�����ԭʼ16���Ƶ�8λ��
//num:����ʱ�����������������1~7��
//dir:�ֲ淽��1����ֲ棬2���ҷֲ�
//����ֵ��Fencha_struct�ṹ��
Fencha_struct FindSpace1(u8 dst,u8 num,u8 dir)
{
	u8 i=0,dirr=dir;
	u8 p=~dst,numr=0,numl=0;
	Fencha_struct find;
	AA:	
	find.zeroNum = 0;
	find.zeroContinue = 0;
	find.Num = 0;
	find.Distance = 0;
	find.Error = 0;
	
	numr=0;
	numl=0;
	
	//������Ч��ĸ���
	for(i=0;i<8;i++)
	{
		if((p>>i)&0x01)
		{
			find.Num++;
		}
	}
	//�ж��ұ�0�ĸ���
	for(i=0;i<8;i++)
	{
		if(((p>>i)&0x01)==0)
		{
			numr++;
		}
		else
		{
			i=8;//ֱ���˳�
		}
	}
	
	//�ж����0�ĸ���
	for(i=0;i<8;i++)
	{
		if(((p<<i)&0x80)==0)
		{
			numl++;
		}
		else
		{
			i=8;//ֱ���˳�
		}
	}		
	//����0�ĸ�����������0���ֵ�λ��   	//
	for(i=numr;i<(8-numl);i++)
	{
		if(((p>>i)&0x01)==0)
		{
			find.zeroLoctaion[find.zeroNum]=i;
			find.zeroNum++;						//�յ����
		}
	}
		
	//�ж϶ϵ��Ƿ�����
	if(find.zeroNum>0)
	{
		find.zeroContinue = 1;
		if(find.zeroNum>1)
		{
			for(i = 0;i<(find.zeroNum-1);i++)
			{
				if(find.zeroLoctaion[i+1]-find.zeroLoctaion[i]==1)
				{
					continue;
				}
				else
				{
					find.zeroContinue++;
				}
			}
		}
	}
	//������Ч���������λ�õľ���
	if(find.Num==0)//û�ҵ���Ч�㣬���뷵��0
		find.Error = 1;
	else
	{
//		if((findtemp.zeroNum==0)&&(find.zeroNum>4)&&(findtemp.Num>0))
//		{
//				if(find.zeroLoctaion[find.zeroNum/2]>=9)
//					dirr = 2;
//				else
//					dirr = 1;
//		}
		if(find.zeroNum>0)//�жϵ㣬����0
		{
			if(find.zeroContinue==1)//һ���ֲ�
			{
//				BEEP = 1;

				if(dirr==2)//�ҷֲ�
				{
					for(i=numr;i<8;i++)
					{
						if((p>>i)&0x01)
						{
							p = p&(~(0x01<<i));
						}
						else
						{
							i = 8;
						}
					}
					goto AA;
				}
				else if(dirr==1)//��ֲ�
				{
					for(i=numl;i<8;i++)
					{
						if((p<<i)&0x80)
						{
							p = p&(~(0x80>>i));
						}
						else
						{
							i = 8;
						}
					}
					goto AA;
				}
				else if(dirr == 0)//ֱ��
				{
					//Ĩ���ֲ沿��
					if(numl > numr)//�ұ߿յ��٣�Ĩ���ұ�
					{
						for(i=numl;i<8;i++)
						{
							if((p<<i)&0x80)
							{
								p = p&(~(0x80>>i));
							}
							else
							{
								i = 8;
							}
						}						
					}
					else if(numl < numr)//��߿յ��٣�Ĩ�����
					{
						for(i=numr;i<8;i++)
						{
							if((p>>i)&0x01)
							{
								p = p&(~(0x01<<i));
							}
							else
							{
								i = 8;
							}
						}
						goto AA;
					}
				}
				else
				{
					find.Error = 1;
				}
			}
			
		}
		else//�������
		{
			//�ж���Ч���Ƿ񳬹�����ֵ
			if(find.Num>NormalNum)
			{
				//ȫ��ʱ�����������ǹ�ʮ��·�ڵ�ʱ��
				if(find.Num==8)
				{
					find.Distance = 0;
				}
				else
				if(dirr==2)//�ҷֲ�
				{
					//Ĩ����λ�����
					for(i=numr;i<find.Num-NormalNum+numr;i++)
					{
						p = p&(~(0x01<<i));
					}
					goto AA;
				}
				else if(dirr==1)//��ֲ�
				{
					//Ĩ����λ�����
					for(i=numl;i<find.Num-NormalNum+numl;i++)
					{
						p = p&(~(0x80>>i));
					}
					goto AA;
				}
				else if(dirr == 0)//ֱ��
				{
					//Ĩ������ؽϽ��Ķ����
					if(numl > numr)//�ұ߿յ��٣�Ĩ���ұ߶����
					{
						//Ĩ����λ�����
						for(i=numr;i<find.Num-NormalNum+numr;i++)
						{
							p = p&(~(0x01<<i));
						}
						goto AA;						
					}
					else if(numl < numr)//��߿յ��٣�Ĩ����߶����
				   	{
						//Ĩ����λ�����
						for(i=numl;i<find.Num-NormalNum+numl;i++)
						{
							p = p&(~(0x80>>i));
						}
						goto AA;
					}
				}
			}
			else
			if(find.Num<=8)
			{
				find.Distance = numl - numr ;				
			}
			else
			{
				find.Error = 1;
			}
		}
	}
	
	findtemp = find;
	return find;

}







/*********************************************************
							�Զ�����ŵ�������ƫ�ƺ���
������
--bitNum:�ŵ���λ��
--data:�ŵ���ԭʼ����
--dir:�ֲ淽��(0��ֱ�У�1����ֲ棬2���ҷֲ�)
--NormalNum;//�����������Ч��ĸ���,�����Ч��ʱ������
˵����
�ŵ������ݣ�
--��Ч�ԣ�1��Ч��0��Ч
--�ߵ�λ�����ҷ����Ӧ��ϵ������Ӧ��λ���Ҳ��Ӧ��λ
--�ŵ���λ��Ĭ�ϴ����Ϊ��0λ����ʼ���㣬����ĸ�λ������Ч
**********************************************************/
Fencha_struct CdhFind(u8 bitNum,u16 data,u8 dir)
{
	u16 i=0;
	u16 datatemp=data;//����һ������
	u16 numr=0,numl=0;//��������δ������ĸ���
	Fencha_struct find;//����ṹ��
	u32 HigtBit = (0x1<<(bitNum-1));//�൱��0x8000�����ڸ�λ����
	AA:	
	//�ṹ���ʼ��
	find.zeroNum = 0;
	find.zeroContinue = 0;
	find.Num = 0;
	find.Distance = 0;
	find.Error = 0;
	
	numr=0;
	numl=0;
	
	//������Ч��ĸ���
	for(i=0;i<bitNum;i++)
	{
		if((datatemp>>i)&0x1)
		{
			find.Num++;
		}
	}
	//�ж��ұ�0�ĸ���
	for(i=0;i<bitNum;i++)
	{
		if(((datatemp>>i)&0x1)==0)
		{
			numr++;
		}
		else
		{
			i=bitNum;//ֱ���˳�
		}
	}
	//�ж����0�ĸ���
	for(i=0;i<bitNum;i++)
	{
		if(((datatemp<<i)&HigtBit)==0)
		{
			numl++;
		}
		else
		{
			i=bitNum;//ֱ���˳�
		}
	}		
	//����0�ĸ�����������0���ֵ�λ��
	for(i=numr;i<(bitNum-numl);i++)
	{
		if(((datatemp>>i)&0x1)==0)
		{
			find.zeroLoctaion[find.zeroNum]=i;
			find.zeroNum++;
		}
	}
	//�ж϶ϵ��Ƿ�����
	if(find.zeroNum>0)
	{
		find.zeroContinue = 1;
		if(find.zeroNum>1)
		{
			for(i = 0;i<(find.zeroNum-1);i++)
			{
				if(find.zeroLoctaion[i+1]-find.zeroLoctaion[i]==1)
				{
					continue;
				}
				else
				{
					find.zeroContinue++;
				}
			}
		}
	}
	//������Ч���������λ�õľ���
	if(find.Num==0)//û�ҵ���Ч�㣬���뷵��0
	{
		find.Distance = 0;
		find.Error = 1;		
	}
	else
	{
		if(find.zeroNum>0)//�жϵ㣬����0
		{
			if(find.zeroContinue==1)//һ���ֲ�
			{
				if(dir==2)//�ҷֲ棬Ĩ����ߣ���λ
				{
					for(i=numl;i<bitNum;i++)
					{
						if((datatemp<<i)&HigtBit)
						{
							datatemp = datatemp&(~(HigtBit>>i));
						}
						else
						{
							i = bitNum;
						}
					}
					goto AA;
				}
				else if(dir==1)//��ֲ�,Ĩ���ұߣ���λ
				{
					for(i=numr;i<bitNum;i++)
					{
						if((datatemp>>i)&0x1)
						{
							datatemp = datatemp&(~(0x1<<i));
						}
						else
						{
							i = bitNum;
						}
					}
					goto AA;
				}
				else if(dir == 0)//ֱ��
				{
					//Ĩ���ֲ沿��
					if(numl > numr)//�ұ߿յ��٣�Ĩ���ұ�
					{
						for(i=numr;i<bitNum;i++)
						{
							if((datatemp>>i)&0x1)
							{
								datatemp = datatemp&(~(0x1<<i));
							}
							else
							{
								i = bitNum;
							}
						}
					}
					else if(numl < numr)//��߿յ��٣�Ĩ�����
					{
						for(i=numl;i<bitNum;i++)
						{
							if((datatemp<<i)&HigtBit)
							{
								datatemp = datatemp&(~(HigtBit>>i));
							}
							else
							{
								i = bitNum;
							}
						}				
						goto AA;
					}
					else if(numl == numr)//����һ���ֱ࣬�Ӿ��д���
					{
						//���ϵ紦ȫ�����
						for(i=numl;i<bitNum-numr;i++)
						{
							//���Ϊ0
							if(((datatemp<<i)&HigtBit)==0)
							{
								//����λ��1
								datatemp = datatemp|(HigtBit>>i);
							}
						}		
					}
				}
				else
				{
					find.Error = 1;
				}
			}
		}
		else//�޶ϵ㣬�������
		{
			//�ж���Ч���Ƿ񳬹�����ֵ
			if(find.Num>NormalNum)
			{
				//ȫ��ʱ�����������ǹ�ʮ��·�ڵ�ʱ��
				if(find.Num==bitNum)
				{
					find.Distance = 0;
				}
				else
				if(dir==2)//�ҷֲ�
				{
					//Ĩ����λ�����
					for(i=numl;i<find.Num-NormalNum+numl;i++)
					{
						datatemp = datatemp&(~(HigtBit>>i));
					}					
					goto AA;
				}
				else if(dir==1)//��ֲ�
				{
					//Ĩ����λ�����
					for(i=numr;i<find.Num-NormalNum+numr;i++)
					{
						datatemp = datatemp&(~(0x1<<i));
					}
					goto AA;
				}
				else if(dir == 0)//ֱ��
				{
					//Ĩ������ؽϽ��Ķ����
					if(numl > numr)//�ұ߿յ��٣�Ĩ���ұ߶����
					{
						//Ĩ����λ�����
						for(i=numr;i<find.Num-NormalNum+numr;i++)
						{
							datatemp = datatemp&(~(0x1<<i));
						}
						goto AA;						
					}
					else if(numl < numr)//��߿յ��٣�Ĩ����߶����
					{
						//Ĩ����λ�����
						for(i=numl;i<find.Num-NormalNum+numl;i++)
						{
							datatemp = datatemp&(~(HigtBit>>i));
						}
						goto AA;
					}
				}
			}
			else
			if(find.Num<=bitNum)
			{				
				find.Distance = numl - numr;
			}
			else
			{
				find.Error = 1;
			}
		}
	}
	findtemp = find;
	return find;
}

































