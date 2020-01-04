#include "sys.h"

//定时器3通道1输入捕获配置
//arr：自动重装值(TIM2,TIM3是32位的!!)
//psc：时钟预分频数
void TIM3_CH1_Cap_Init1(u32 arr,u16 psc)
{
	TIM_ICInitTypeDef  TIM3_ICInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  		//TIM3时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; 		//GPIOA6、7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;						//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 					//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 						//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 									//初始化PA

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); 	//PA6复用位定时器3
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3); 	//PA7复用位定时器3

	TIM_TimeBaseStructure.TIM_Period=arr;   								//自动重装载值  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  							//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);


	//初始化TIM3输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	选择输入端 
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM3_ICInitStructure.TIM_ICFilter = 0xF;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//初始化TIM3输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
	TIM3_ICInitStructure.TIM_ICFilter = 0xF;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);


	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

	TIM_ITConfig(TIM3,TIM_IT_CC1|TIM_IT_CC2,ENABLE);//允许CC1IE捕获中断	

	TIM_Cmd(TIM3,ENABLE ); 	//使能定时器3
	
}

u32	count31=0,count32=0;	//输入捕获值
//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) //捕获1发生捕获事件
	{	
		count31++;											//每捕获成功一次就计数加1
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); 		//清除中断标志位
	}	
	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET) //捕获2发生捕获事件
	{	
		count32++;											//每捕获成功一次就计数加1
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC2); 		//清除中断标志位
	}			
}

