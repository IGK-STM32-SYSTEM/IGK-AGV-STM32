#include "sys.h"


void TLC5620_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//DAC芯片端口初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
    
	DADATA=1;
	DACLK =1;
	DALOAD=1;
	DALDAC=1;
} 

/******************************
*DAC芯片转换函数
*CHn 转换的通道号:1,2,3,4
*欲转换的电压值:0~5V之间的任何数值
* A0 A1 RNG   D7~D0 共11位
******************************/
void TLC5620_OUT(u8 CHn,double date)
{
		u8 i=0,dat=0,datt=0;
		DACLK =0;
		DADATA=0;
		DALOAD=1;
		DALDAC=0;
		delay_us(2);
		for(i=0;i<3;i++)
		{
				switch(CHn)
				{
					case 1://00 x:A通道1+x倍增益
					{
						if(i==2)
						{
							DADATA=TLC5620RNG;
						}
						else 
						{
							DADATA=0;
						}
					}break;
					case 2://01 x:B通道1+x倍增益
					{
						if(i==2)
						{
							DADATA=TLC5620RNG;
						}
						else if(i==1)
						{
							DADATA=1;
						}
						else
						{
							DADATA=0;
						}
					}break;
					case 3://10 x:C通道1+x倍增益
					{
						if(i==2)
						{
							DADATA=TLC5620RNG;
						}
						else if(i==1)
						{
							DADATA=0;
						}
						else
						{
							DADATA=1;
						}
					}break;
					case 4://11 x:D通道1+x倍增益
					{
						if(i==2)
						{
							DADATA=TLC5620RNG;
						}
						else
						{
							DADATA=1;
						}
					}break;
				}
				DACLK =1;
				delay_us(2);
				DACLK =0;
				delay_us(2);
		}	
		datt=(u8)(date*255/(3.3*2.0*100));//基准电压为3.3V不放大增益
		for(i=0;i<8;i++)
		{
				dat=datt&0x80;//按位送数据
				if(dat)
					DADATA=1;
				else 
					DADATA=0;
				DACLK =1;
				delay_us(2);
				DACLK =0;
				delay_us(2);
				datt<<=1;
		}
		DALOAD=0;		
		delay_us(1);
		DALOAD=1;		
		delay_us(1);
}

void TLC5620_OUTnum(u8 CHn,u8 date)
{
		u8 i=0,dat=0;
		DACLK =0;
		DADATA=0;
		DALOAD=1;
		DALDAC=0;
		delay_us(2);
		for(i=0;i<3;i++)
		{
				switch(CHn)
				{
					case 1://00 x:A通道1+x倍增益
					{
						if(i==2)
						{
							DADATA=TLC5620RNG;
						}
						else 
						{
							DADATA=0;
						}
					}break;
					case 2://01 x:B通道1+x倍增益
					{
						if(i==2)
						{
							DADATA=TLC5620RNG;
						}
						else if(i==1)
						{
							DADATA=1;
						}
						else
						{
							DADATA=0;
						}
					}break;
					case 3://10 x:C通道1+x倍增益
					{
						if(i==2)
						{
							DADATA=TLC5620RNG;
						}
						else if(i==1)
						{
							DADATA=0;
						}
						else
						{
							DADATA=1;
						}
					}break;
					case 4://11 x:D通道1+x倍增益
					{
						if(i==2)
						{
							DADATA=TLC5620RNG;
						}
						else
						{
							DADATA=1;
						}
					}break;
				}
				DACLK =1;
				delay_us(2);
				DACLK =0;
				delay_us(2);
		}	
//		datt=(u8)(date*255/(3.3*2.0));//基准电压为3.3V不放大增益
		for(i=0;i<8;i++)
		{
				dat=date&0x80;//按位送数据
				if(dat)
					DADATA=1;
				else 
					DADATA=0;
				DACLK =1;
				delay_us(2);
				DACLK =0;
				delay_us(2);
				date<<=1;
		}
		DALOAD=0;		
		delay_us(1);
		DALOAD=1;		
		delay_us(1);
}


