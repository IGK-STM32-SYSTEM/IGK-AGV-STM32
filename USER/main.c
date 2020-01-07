#include "sys.h"
#include "igk_dfs.h"
#include "igk_map.h"
#include "igk_ucos.h"

int main(void)
{
//	OS_ERR err;
//	CPU_SR_ALLOC();

	delay_init(168);  	//时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	LED_Init();         //LED初始化
	DMA_adc1_Init();
	DMA_Uart4_Init();
	DMA_Uart2_Init();
	DMA_Uart6_Init();
	
	DMA_Uart3_Init();
	DMA_Uart5_Init();
	ADC1_Configuration();
	USART1_Configuration(9600);
	USART4_Configuration(115200);
	USART6_Configuration(115200);
	USART2_Configuration(115200);//485-2接控制软件
	USART3_Configuration(115200);//485-3接调试信息打印
	USART5_Configuration(115200);
//	TIM4_PWM_Init(1000 - 1, 8 - 1);	//84M/8=10Mhz?????,????1000,??PWM??? 10M/50000=200hz. //500-10   20hz-1000hz
	CAN1_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN1初始化普通模式,波特率500Kbps
	CAN2_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN2初始化普通模式,波特率500Kbps
	PID_Init();
	TIM2_Int_Init(5 - 1, 84 - 1);			//Tout=((arr+1)*(psc+1))/Ft us.  5us
//	IWDG_Init(4, 500);
	SPI1_Init();
  TLC5620_Init();
	
	//指针类型变量指向默认地址
	IgkAgvOs.TongXin.NowRfid = &PLC_Data[41];
	IgkAgvOs.Speed = &PLC_Data[61];

	
	//设定默认参数
	IgkAgvOs.OsTime.Hour = 0;
	IgkAgvOs.OsTime.Minute = 0;
	IgkAgvOs.OsTime.Second = 0;
	IgkAgvOs.OsTime.Millisecond = 0;
	
	IgkAgvOs.WorkMode = Enum_LocalAuto;//自动模式
	IgkAgvOs.Dir = Enum_QianJin;//前进方向
	*IgkAgvOs.TongXin.NowRfid = 1;
	IgkAgvOs.RunOrStop = Enum_Stop;//停止状态
	*IgkAgvOs.Speed = 50;//速度50%;	
	
	IGK_SysTimePrintln("正在初始化...");
	//初始化Flash
	W25QXX_Init();			//W25QXX初始化
	while(W25QXX_ReadID()!=W25Q128)//检测不到W25Q128，LED闪烁
	{
		delay(0,0,0,100);
	}

	//系统启动，蜂鸣器提示
	for(u8 i=0;i<4;i++)
	{
		BEEP =~BEEP;
		osdelay_ms(60);
	}
	IGK_SysTimePrintln("Flash Ok!");	
	//初始化内存池[采用原子内存管理]
	my_mem_init(SRAMIN);
	IGK_SysTimePrintln("开启动态内存：%dKByte!",MEM1_MAX_SIZE/1024);	
	//初始化ucos
	UCOS_Init();
	while(1);
}

//模式
void task1(void *p_arg)
{
	p_arg = p_arg;
	osdelay_ms(10);
	while(1)
	{
		//监控摇杆按键,切换模式
		if(IgkAgvOs.YaoGan.key==KeyDown)
		{
			delay(0, 0, 1, 0);
			if(IgkAgvOs.YaoGan.key==KeyDown)
			{
				if(IgkAgvOs.WorkMode == Enum_LocalManual)
				{
					IgkAgvOs.WorkMode = Enum_LocalAuto;
					IGK_Speek("自动模式");
				}
				else
				{
					IgkAgvOs.WorkMode = Enum_LocalManual;
					IGK_Speek("手动模式");
				}
			}
		}
		//手动控制模式
		if(IgkAgvOs.WorkMode == Enum_LocalManual)
		{
			//直接将速度赋值到驱动器
			//判断当前动作
			if(abs(IgkAgvOs.YaoGan.y)>0)//前进后退
			{
				*IgkAgvOs.Speed = abs(IgkAgvOs.YaoGan.y)*1;
				//转向速度
				s8 speed = IgkAgvOs.YaoGan.z*0.5;
				if(IgkAgvOs.YaoGan.y>0)
				{
					if(speed >= 0)
						DriverQinJinSpeed(*IgkAgvOs.Speed-speed,*IgkAgvOs.Speed);
					else
						DriverQinJinSpeed(*IgkAgvOs.Speed,*IgkAgvOs.Speed+speed);
				}
				else
				if(IgkAgvOs.YaoGan.y<0)
				{
					if(speed >= 0)
						DriverHouTuiSpeed(*IgkAgvOs.Speed,*IgkAgvOs.Speed-speed);
					else
						DriverHouTuiSpeed(*IgkAgvOs.Speed+speed,*IgkAgvOs.Speed);
				}
			}
			else
			if(abs(IgkAgvOs.YaoGan.z)>0)//左右旋转
			{
				*IgkAgvOs.Speed = abs(IgkAgvOs.YaoGan.z)*0.8;
				if(IgkAgvOs.YaoGan.z<0)
					DriverZuoXuan(*IgkAgvOs.Speed);
				else
				if(IgkAgvOs.YaoGan.z>0)
					DriverYouXuan(*IgkAgvOs.Speed);
			}
			else // 停止
				DriverTingZhi();
		}
		delay(0, 0, 0, 5);
	}
}
//自动运行模式
void Auto_task(void *p_arg)
{
	p_arg = p_arg;
	//初始化PID
	PID_Init();
	while(1)
	{
		//自动模式
		if(IgkAgvOs.WorkMode == Enum_LocalAuto)
		{
			//前进
			if(IgkAgvOs.Dir == Enum_QianJin)
			{ 
				if(IgkAgvOs.RunOrStop == Enum_Stop)
					DriverTingZhi();
				else
				{
					switch(IgkAgvOs.Action)
					{
						case Enum_ZuoXuan://左旋
							DriverZuoXuan(50);
							break;
						case Enum_YouXuan://右旋
							DriverYouXuan(50);
							break;
						case Enum_ZuoFenCha://左分叉
						case Enum_YouFenCha://右分叉
						case Enum_ZhiXing:  //直行
							if(IgkAgvOs.QianCiDaoHang.Error == 0)
							{
								//PID动态调节循迹
								float Inc = PosPIDCalc(IgkAgvOs.QianCiDaoHang.Distance);
								//更新速度
								s16 s1 = *IgkAgvOs.Speed+Inc;
								s16 s2 = *IgkAgvOs.Speed-Inc;
								//判定边界
								s1 = s1>100?100:s1;
								s1 = s1<0?0:s1;
								s2 = s2>100?100:s2;
								s2 = s2<0?0:s2;
								DriverQinJinSpeed(s1,s2);
							}
							//出轨停止
							else
							{
								DriverTingZhi();
							}
							break;
						default:
							break;
					}
				}
			}
			else
			//后退
			if(IgkAgvOs.Dir == Enum_HouTui)
			{
				if(IgkAgvOs.RunOrStop == Enum_Stop)
					DriverTingZhi();
				else
				{
					switch(IgkAgvOs.Action)
					{
						case Enum_ZuoXuan://左旋
							DriverZuoXuan(50);
							break;
						case Enum_YouXuan://右旋
							DriverYouXuan(50);
							break;
						case Enum_ZuoFenCha://左分叉
						case Enum_YouFenCha://右分叉
						case Enum_ZhiXing:  //直行
							if(IgkAgvOs.HouCiDaoHang.Error == 0)
							{
								//PID动态调节循迹
								float Inc = PosPIDCalc(-IgkAgvOs.HouCiDaoHang.Distance);
								//更新速度
								s16 s1 = *IgkAgvOs.Speed + Inc;
								s16 s2 = *IgkAgvOs.Speed - Inc;
								//判定边界
								s1 = s1>100?100:s1;
								s1 = s1<0?0:s1;
								s2 = s2>100?100:s2;
								s2 = s2<0?0:s2;
								DriverHouTuiSpeed(s1,s2);
							}
							//出轨停止
							else
							{
								DriverTingZhi();
							}
							break;
						default:
							break;
					}
				}
			}
		}
		delay(0, 0, 0, 10);
	}
}

//计算目标位置
u16 target = 1;
void Manual_task(void *p_arg)
{
	p_arg = p_arg;
	osdelay_s(1);
	yinling(1);
	IGK_Speek("系统自检完成");
	while(1)
	{
		/*1.-----------------等待执行信号--------------------------*/
		IGK_SysTimePrintln("等待任务!");
		while(PLC_Data[43]!=1)
		{
			osdelay_ms(10);
		}
		//清除执行信号
		PLC_Data[43] = 0;
		/*2.-----------------判断目标标签合法性--------------------------*/
		if(PLC_Data[40]==*IgkAgvOs.TongXin.NowRfid)
		{
			IGK_Speek("目标标签和当前标签相同,任务完成！");
			IGK_SysTimePrintln("目标标签和当前标签相同,任务完成！");
			//跳过
			continue;
		}
		else
		if(PLC_Data[40]==0)
		{
			IGK_Speek("目标标签为0,操作非法！");
			IGK_SysTimePrintln("目标标签为0,操作非法！");
			//跳过
			continue;
		}
		else
		if(PLC_Data[40] > NodeMaxNum)
		{
			IGK_Speek("目标标签不能大于%d,操作非法！",NodeMaxNum);
			IGK_SysTimePrintln("目标标签不能大于%d,操作非法！",NodeMaxNum);
			//跳过
			continue;
		}	
		//更新目标位置到临时变量
		target = PLC_Data[40];
		//搜索路径
		FindRoute(*IgkAgvOs.TongXin.NowRfid,target);
		IGK_SysTimePrintln("路径搜索完成!");

		//播报目标位置
		IGK_Speek("准备前往%d号标签",target);
		osdelay_s(3);
		if(BestPath.PathTotal==0)
		{
			IGK_SysPrintf("未找到有效路径！\r\n\r\n");
			IGK_Speek("未找到有效路径,请重新设置");
		}
		else
		{
			IGK_SysPrintf("共找到%d条路径！\r\n\r\n",BestPath.PathTotal);
			IGK_Speek("共找到%d条路径！",BestPath.PathTotal);
			osdelay_s(2);
		/*3.-----------------执行路径--------------------------*/
			//启动
			IgkAgvOs.RunOrStop = Enum_Run;
			for(int i=0;i < BestPath.NodeCount;i++)
			{
				//给定默认速度
				*IgkAgvOs.Speed = 50;
				u16 nodeId = BestPath.NodeList[i];
				//读取对应站点的地图
				StaionMapStruct mapStruct;
				ReadToMapStruct(nodeId,&mapStruct);
				IGK_SysPrintf("标签号:[%d]",nodeId);
				//下一个位置更新到界面
				PLC_Data[42] = nodeId;
				//等待标签,起点不用等待
				if(i!=0)
				{
						IGK_Speek("等待%d号标签!",nodeId);
						//osdelay_s(2);
						while(*IgkAgvOs.TongXin.NowRfid != nodeId )
						{
							osdelay_ms(5);
						}
						if(i == BestPath.NodeCount-1)
						{
							//到达终点
							break;
						}
				}
				//直行站点方向，动作
				for(int j=0;j<StationMapType;j++)
				{
					if(mapStruct.Stop[j] == BestPath.NodeList[i+1])
					{
						//方向
						if(mapStruct.Dir[j] == Enum_QianJin)
						{
							if(IgkAgvOs.Dir != Enum_QianJin)
							{
								//停止
								IgkAgvOs.RunOrStop = Enum_Stop;
								DriverTingZhi();
								//切换方向
								IgkAgvOs.Dir = Enum_QianJin;
								//等待
								osdelay_ms(500);
							}
							//打印数据
							IGK_SysPrintf("前进-");
							//语音播报
							IGK_Speek("前进");					
						}
						else
						if(mapStruct.Dir[j] == Enum_HouTui)
						{
							if(IgkAgvOs.Dir != Enum_HouTui)
							{
								//停止
								IgkAgvOs.RunOrStop = Enum_Stop;
								DriverTingZhi();
								//切换方向
								IgkAgvOs.Dir = Enum_HouTui;
								//等待
								osdelay_ms(500);
							}
							//打印数据
							IGK_SysPrintf("后退-");
							//语音播报
							IGK_Speek("后退");
						}
						else
						if(mapStruct.Dir[j] == Enum_PingYi)
						{
							if(IgkAgvOs.Dir != Enum_ZuoFenCha)
							{
								osdelay_ms(300);
								IGK_SysPrintf("平移-");
								IGK_Speek("平移");
							}
						}
						
						//动作
						if(mapStruct.Action[j] == Enum_ZuoFenCha)
						{
							IgkAgvOs.Action = Enum_ZuoFenCha;
							IGK_SysPrintf("左分叉-");
							IGK_Speek("左分叉");
						}
						else
						if(mapStruct.Action[j] == Enum_YouFenCha)
						{
							IgkAgvOs.Action = Enum_YouFenCha;
							IGK_SysPrintf("右分叉-");
							IGK_Speek("右分叉");
						}
						else
						if(mapStruct.Action[j] == Enum_ZhiXing)
						{
							IgkAgvOs.Action = Enum_ZhiXing;
							IGK_SysPrintf("直行-");
							IGK_Speek("直行");
						}
						else
						if(mapStruct.Action[j] == Enum_ZuoXuan)
						{
							//停车
							IgkAgvOs.RunOrStop = Enum_Stop;
							DriverTingZhi();
							IGK_Speek("左旋%d度",mapStruct.Angle[j]);
							//原地旋转
							osdelay_ms(200);
							//左旋
							IgkAgvOs.Action = Enum_ZuoXuan;
							//启动
							IgkAgvOs.RunOrStop = Enum_Run;
							//根据方向选择磁导航传感器
							Fencha_struct * PCiDaoHang;
							if(IgkAgvOs.Dir == Enum_QianJin)
								PCiDaoHang = &IgkAgvOs.QianCiDaoHang;
							else
								PCiDaoHang = &IgkAgvOs.HouCiDaoHang;
							//等待离开磁条
							while(PCiDaoHang->Error==0)
								osdelay_ms(10);
							//等待检测到磁条[至少两个点并且居中]
							while(PCiDaoHang->Num <3)
								osdelay_ms(10);
							if(mapStruct.Angle[j]==180)
							{
								//等待离开磁条
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//等待检测到磁条[至少两个点并且居中]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
							}
							else
							if(mapStruct.Angle[j]==270)
							{
								//等待离开磁条
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//等待检测到磁条[至少两个点并且居中]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
								//等待离开磁条
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//等待检测到磁条[至少两个点并且居中]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
							}
							//停止
							IgkAgvOs.RunOrStop = Enum_Stop;
							DriverTingZhi();
							osdelay_ms(200);
							//切换直行
							IgkAgvOs.Action = Enum_ZhiXing;
							//切换为运动状态
							osdelay_ms(200);
							IgkAgvOs.RunOrStop = Enum_Run;
							IGK_SysPrintf("左旋[%d]度-",mapStruct.Angle[j]);
						}
						else
						if(mapStruct.Action[j] == Enum_YouXuan)
						{
							//停车
							IgkAgvOs.RunOrStop = Enum_Stop;
							DriverTingZhi();
							IGK_Speek("右旋%d度",mapStruct.Angle[j]);
							//原地旋转
							osdelay_ms(200);
							//右旋
							IgkAgvOs.Action = Enum_YouXuan;
							//启动
							IgkAgvOs.RunOrStop = Enum_Run;
							//根据方向选择磁导航传感器
							Fencha_struct * PCiDaoHang;
							if(IgkAgvOs.Dir == Enum_QianJin)
								PCiDaoHang = &IgkAgvOs.QianCiDaoHang;
							else
								PCiDaoHang = &IgkAgvOs.HouCiDaoHang;
							//等待离开磁条
							while(PCiDaoHang->Error==0)
								osdelay_ms(10);
							//等待检测到磁条[至少两个点并且居中]
							while(PCiDaoHang->Num <3)
								osdelay_ms(10);
							if(mapStruct.Angle[j]==180)
							{
								//等待离开磁条
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//等待检测到磁条[至少两个点并且居中]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
							}
							else
							if(mapStruct.Angle[j]==270)
							{
								//等待离开磁条
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//等待检测到磁条[至少两个点并且居中]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
								//等待离开磁条
								while(PCiDaoHang->Error==0)
									osdelay_ms(10);
								//等待检测到磁条[至少两个点并且居中]
								while(PCiDaoHang->Num <3)
									osdelay_ms(10);
							}
							//停止
							IgkAgvOs.RunOrStop = Enum_Stop;
							DriverTingZhi();
							osdelay_ms(200);
							//切换直行
							IgkAgvOs.Action = Enum_ZhiXing;
							//切换为运动状态
							osdelay_ms(200);
							//原地旋转
							IGK_SysPrintf("右旋[%d]度-",mapStruct.Angle[j]);
						}
						IgkAgvOs.RunOrStop = Enum_Run;
						//到达
						IGK_SysPrintf("[%d]\r\n\r\n",mapStruct.Stop[j]);
						//找到对应站点，跳出循环
						break;
					}
				}
			}
			//到达目标位置,停车
			IgkAgvOs.RunOrStop = Enum_Stop;
			IGK_Speek("到达终点，任务完成");
			IGK_SysTimePrintln("到达终点，任务完成");
		}
		delay(0, 0, 0, 10); //延时10ms
	}
}
//Modbus同步进程
void PID_task(void *p_arg)
{
	while(1)
	{
		ReadWriteMap(p_arg);
		delay(0, 0, 0, 100);
	}
}
//监听按键
void float_task(void *p_arg)
{
	while(1)
	{
		if(IN6 == KeyDown)
		{
			//延时消抖
			delay(0, 0, 0, 30);
			if(IN6 == KeyDown)
			{
				IgkAgvOs.RunOrStop = Enum_Run;
				IGK_Speek("启动");
				//等待按键抬起
				while(IN6 == KeyDown){delay(0, 0, 0, 5);}
			}
		}
		if(IN7 == KeyDown)
		{
			//延时消抖
			delay(0, 0, 0, 30);
			if(IN7 == KeyDown)
			{
				IgkAgvOs.RunOrStop = Enum_Stop;
				IGK_Speek("停止");
				//等待按键抬起
				while(IN7 == KeyDown){delay(0, 0, 0, 5);}
			}
		}
		delay(0, 0, 0, 5); //延时5ms
	}
}

void DEMO_task(void *p_arg)//LED,DA，FLASH，语音测试
{
    
	while(1)
	{
		LED1 = ~LED1;
		delay(0, 0, 0, 300);
	}
}

//启动
void DEMO1_task(void *p_arg)//网口测试
{
//	OS_ERR err;
//	CPU_STK_SIZE  n_free;
//  CPU_STK_SIZE  n_used;  
	while(1)
	{
		delay(0, 0, 1, 10);
//		OSTaskStkChk(&Task1TCB,
//                 &n_free,
//                 &n_used,
//                 &err);
//		IGK_SysTimePrintln("Task1:%d，占用：%d，使用率：%f",TASK1_STK_SIZE,n_used,n_used/(float)TASK1_STK_SIZE);
//		OSTaskStkChk(&ManualTaskTCB,
//                 &n_free,
//                 &n_used,
//                 &err);
//		IGK_SysTimePrintln("Manual:%d，占用：%d，使用率：%f",Manual_STK_SIZE,n_used,n_used/(float)Manual_STK_SIZE);
	}
}
//喂狗,更新系统运行时间
void DEMO2_task(void *p_arg)
{
	u16 num = 0;
	while(1)
	{
		//定时喂狗
		num++;
		if(num == 20)
		{
			num = 0;
			IWDG_Feed();//喂狗
		}
		//更新系统运行时间
		GetSysRunTime(&IgkAgvOs.OsTime,p_arg);
		delay(0, 0, 0, 5);
	}
}


