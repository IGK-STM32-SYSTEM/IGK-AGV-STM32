#include "cdhdata.h"

u8 NormalNum=4;//正常情况下有效点的个数


//孙先生的点数处理函数
Fencha_struct front_cdh16,back_cdh16,backl_cdh8,backr_cdh8;//定义一个Fencha_struct类型的全局变量
Fencha_struct findtemp={0,0,0,0,0,0};//孙先生函数中的结构体数组定义



//16位磁导航数据处理函数

//dst:磁导航的原始16进制的16位数
//num:处理时正常情况的最多点数（1~15）
//dir:分叉方向0：直行，1：左分叉，2：右分叉
// 左侧;高位，右侧：低位
//返回值：Fencha_struct结构体
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
	
	//计算有效点的个数
	for(i=0;i<16;i++)
	{
		if((p>>i)&0x0001)
		{
			find.Num++;
		}
	}
	//判断右边0的个数
	for(i=0;i<16;i++)
	{
		if(((p>>i)&0x0001)==0)
		{
			numr++;
		}
		else
		{
			i=16;//直接退出
		}
	}
	//判断左边0的个数
	for(i=0;i<16;i++)
	{
		if(((p<<i)&0x8000)==0)
		{
			numl++;
		}
		else
		{
			i=16;//直接退出
		}
	}		
	//计算0的个数，并计算0出现的位置
	for(i=numr;i<(16-numl);i++)
	{
		if(((p>>i)&0x0001)==0)
		{
			find.zeroLoctaion[find.zeroNum]=i;
			find.zeroNum++;
		}
	}
	//判断断点是否连续
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
	//计算有效点距离中心位置的距离
	if(find.Num==0)//没找到有效点，距离返回0
	{
		find.Distance = 0;
		find.Error = 1;		
	}
	else
	{
		if(find.zeroNum>0)//有断点，返回0
		{
			if(find.zeroContinue==1)//一个分叉
			{
				if(dirr==2)//右分叉，抹掉左边，高位
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
				else if(dirr==1)//左分叉,抹掉右边，低位
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
				else if(dirr == 0)//直行
				{
					//抹掉分叉部分
					if(numl > numr)//右边空点少，抹除右边
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
					else if(numl < numr)//左边空点少，抹除左边
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
					else if(numl == numr)//两边一样多，直接居中处理
					{
						//将断电处全部填充
						for(i=numl;i<16-numr;i++)
						{
							//如果为0
							if(((p<<i)&0x8000)==0)
							{
								//将该位置1
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
		else//无断点，正常情况
		{
			//判断有效点是否超过正常值
			if(find.Num>NormalNum)
			{
				//全亮时不调整，就是过十字路口的时候
				if(find.Num==16)
				{
					find.Distance = 0;
				}
				else
				if(dirr==2)//右分叉
				{
					//抹掉高位多余点
					for(i=numl;i<find.Num-NormalNum+numl;i++)
					{
						p = p&(~(0x8000>>i));
					}					
					goto AA;
				}
				else if(dirr==1)//左分叉
				{
					//抹掉低位多余点
					for(i=numr;i<find.Num-NormalNum+numr;i++)
					{
						p = p&(~(0x0001<<i));
					}
					goto AA;
				}
				else if(dirr == 0)//直行
				{
					//抹掉离边沿较近的多余点
					if(numl > numr)//右边空点少，抹除右边多余点
					{
						//抹掉低位多余点
						for(i=numr;i<find.Num-NormalNum+numr;i++)
						{
							p = p&(~(0x0001<<i));
						}
						goto AA;						
					}
					else if(numl < numr)//左边空点少，抹除左边多余点
					{
						//抹掉高位多余点
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
//8位磁导航数据处理函数
//dst:磁导航的原始16进制的8位数
//num:处理时正常情况的最多点数（1~7）
//dir:分叉方向1：左分叉，2：右分叉
//返回值：Fencha_struct结构体
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
	
	//计算有效点的个数
	for(i=0;i<8;i++)
	{
		if((p>>i)&0x01)
		{
			find.Num++;
		}
	}
	//判断右边0的个数
	for(i=0;i<8;i++)
	{
		if(((p>>i)&0x01)==0)
		{
			numr++;
		}
		else
		{
			i=8;//直接退出
		}
	}
	
	//判断左边0的个数
	for(i=0;i<8;i++)
	{
		if(((p<<i)&0x80)==0)
		{
			numl++;
		}
		else
		{
			i=8;//直接退出
		}
	}		
	//计算0的个数，并计算0出现的位置   	//
	for(i=numr;i<(8-numl);i++)
	{
		if(((p>>i)&0x01)==0)
		{
			find.zeroLoctaion[find.zeroNum]=i;
			find.zeroNum++;						//空点个数
		}
	}
		
	//判断断点是否连续
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
	//计算有效点距离中心位置的距离
	if(find.Num==0)//没找到有效点，距离返回0
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
		if(find.zeroNum>0)//有断点，返回0
		{
			if(find.zeroContinue==1)//一个分叉
			{
//				BEEP = 1;

				if(dirr==2)//右分叉
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
				else if(dirr==1)//左分叉
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
				else if(dirr == 0)//直行
				{
					//抹掉分叉部分
					if(numl > numr)//右边空点少，抹除右边
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
					else if(numl < numr)//左边空点少，抹除左边
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
		else//正常情况
		{
			//判断有效点是否超过正常值
			if(find.Num>NormalNum)
			{
				//全亮时不调整，就是过十字路口的时候
				if(find.Num==8)
				{
					find.Distance = 0;
				}
				else
				if(dirr==2)//右分叉
				{
					//抹掉低位多余点
					for(i=numr;i<find.Num-NormalNum+numr;i++)
					{
						p = p&(~(0x01<<i));
					}
					goto AA;
				}
				else if(dirr==1)//左分叉
				{
					//抹掉高位多余点
					for(i=numl;i<find.Num-NormalNum+numl;i++)
					{
						p = p&(~(0x80>>i));
					}
					goto AA;
				}
				else if(dirr == 0)//直行
				{
					//抹掉离边沿较近的多余点
					if(numl > numr)//右边空点少，抹除右边多余点
					{
						//抹掉低位多余点
						for(i=numr;i<find.Num-NormalNum+numr;i++)
						{
							p = p&(~(0x01<<i));
						}
						goto AA;						
					}
					else if(numl < numr)//左边空点少，抹除左边多余点
				   	{
						//抹掉高位多余点
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
							自动计算磁导航数据偏移函数
参数：
--bitNum:磁导航位数
--data:磁导航原始数据
--dir:分叉方向(0：直行，1：左分叉，2：右分叉)
--NormalNum;//正常情况下有效点的个数,清除无效点时起作用
说明：
磁导航数据：
--有效性：1有效，0无效
--高低位和左右方向对应关系：左侧对应高位，右侧对应低位
--磁导航位数默认从最低为（0位）开始计算，多余的高位数据无效
**********************************************************/
Fencha_struct CdhFind(u8 bitNum,u16 data,u8 dir)
{
	u16 i=0;
	u16 datatemp=data;//缓存一份数据
	u16 numr=0,numl=0;//左右两侧未触发点的个数
	Fencha_struct find;//结果结构体
	u32 HigtBit = (0x1<<(bitNum-1));//相当于0x8000，用于高位计算
	AA:	
	//结构体初始化
	find.zeroNum = 0;
	find.zeroContinue = 0;
	find.Num = 0;
	find.Distance = 0;
	find.Error = 0;
	
	numr=0;
	numl=0;
	
	//计算有效点的个数
	for(i=0;i<bitNum;i++)
	{
		if((datatemp>>i)&0x1)
		{
			find.Num++;
		}
	}
	//判断右边0的个数
	for(i=0;i<bitNum;i++)
	{
		if(((datatemp>>i)&0x1)==0)
		{
			numr++;
		}
		else
		{
			i=bitNum;//直接退出
		}
	}
	//判断左边0的个数
	for(i=0;i<bitNum;i++)
	{
		if(((datatemp<<i)&HigtBit)==0)
		{
			numl++;
		}
		else
		{
			i=bitNum;//直接退出
		}
	}		
	//计算0的个数，并计算0出现的位置
	for(i=numr;i<(bitNum-numl);i++)
	{
		if(((datatemp>>i)&0x1)==0)
		{
			find.zeroLoctaion[find.zeroNum]=i;
			find.zeroNum++;
		}
	}
	//判断断点是否连续
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
	//计算有效点距离中心位置的距离
	if(find.Num==0)//没找到有效点，距离返回0
	{
		find.Distance = 0;
		find.Error = 1;		
	}
	else
	{
		if(find.zeroNum>0)//有断点，返回0
		{
			if(find.zeroContinue==1)//一个分叉
			{
				if(dir==2)//右分叉，抹掉左边，高位
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
				else if(dir==1)//左分叉,抹掉右边，低位
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
				else if(dir == 0)//直行
				{
					//抹掉分叉部分
					if(numl > numr)//右边空点少，抹除右边
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
					else if(numl < numr)//左边空点少，抹除左边
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
					else if(numl == numr)//两边一样多，直接居中处理
					{
						//将断电处全部填充
						for(i=numl;i<bitNum-numr;i++)
						{
							//如果为0
							if(((datatemp<<i)&HigtBit)==0)
							{
								//将该位置1
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
		else//无断点，正常情况
		{
			//判断有效点是否超过正常值
			if(find.Num>NormalNum)
			{
				//全亮时不调整，就是过十字路口的时候
				if(find.Num==bitNum)
				{
					find.Distance = 0;
				}
				else
				if(dir==2)//右分叉
				{
					//抹掉高位多余点
					for(i=numl;i<find.Num-NormalNum+numl;i++)
					{
						datatemp = datatemp&(~(HigtBit>>i));
					}					
					goto AA;
				}
				else if(dir==1)//左分叉
				{
					//抹掉低位多余点
					for(i=numr;i<find.Num-NormalNum+numr;i++)
					{
						datatemp = datatemp&(~(0x1<<i));
					}
					goto AA;
				}
				else if(dir == 0)//直行
				{
					//抹掉离边沿较近的多余点
					if(numl > numr)//右边空点少，抹除右边多余点
					{
						//抹掉低位多余点
						for(i=numr;i<find.Num-NormalNum+numr;i++)
						{
							datatemp = datatemp&(~(0x1<<i));
						}
						goto AA;						
					}
					else if(numl < numr)//左边空点少，抹除左边多余点
					{
						//抹掉高位多余点
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

































