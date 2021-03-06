#include "sys.h"
#include "igk_dfs.h"
#include "igk_map.h"
#include "igk_ucos.h"
#include "malloc.h"
#include "motec.h"

#if DEF_TRUE
OS_TCB Task1TCB;//任务控制块
CPU_STK Task1_STK[256];//任务堆栈
void Task1_task(void *p_arg);//任务函数

OS_TCB Task2TCB;//任务控制块
CPU_STK Task2_STK[256];//任务堆栈
void Task2_task(void *p_arg);//任务函数

OS_TCB Task3TCB;//任务控制块
CPU_STK Task3_STK[256];//任务堆栈
void Task3_task(void *p_arg);//任务函数

OS_TCB Task4TCB;//任务控制块
CPU_STK Task4_STK[256];//任务堆栈
void Task4_task(void *p_arg);//任务函数

OS_TCB Task5TCB;//任务控制块
CPU_STK Task5_STK[256];//任务堆栈
void Task5_task(void *p_arg);//任务函数

OS_TCB Task6TCB;//任务控制块
CPU_STK Task6_STK[256];//任务堆栈
void Task6_task(void *p_arg);//任务函数

OS_TCB Task7TCB;//任务控制块
CPU_STK Task7_STK[256];//任务堆栈
void Task7_task(void *p_arg);//任务函数

OS_TCB Task8TCB;//任务控制块
CPU_STK Task8_STK[128];//任务堆栈
void Task8_task(void *p_arg);//任务函数

OS_TCB Task9TCB;//任务控制块
CPU_STK Task9_STK[128];//任务堆栈
void Task9_task(void *p_arg);//任务函数

#endif


int main(void)
{
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
	USART4_Configuration(9600);//电池
	USART6_Configuration(115200);
	USART2_Configuration(115200);//485-2接控制软件
	USART3_Configuration(115200);//485-3接调试信息打印
	USART5_Configuration(115200);
//	TIM4_PWM_Init(1000 - 1, 8 - 1);	//84M/8=10Mhz?????,????1000,??PWM??? 10M/50000=200hz. //500-10   20hz-1000hz
	CAN1_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN1初始化普通模式,波特率500Kbps
	CAN2_Mode_Init(CAN_SJW_1tq, CAN_BS2_6tq, CAN_BS1_7tq, 6, CAN_Mode_Normal); //CAN2初始化普通模式,波特率500Kbps
	TIM2_Int_Init(5 - 1, 84 - 1);			//Tout=((arr+1)*(psc+1))/Ft us.  5us
	IWDG_Init(4, 500);//大概1066ms
	SPI1_Init();
  TLC5620_Init();

	//初始化系统变量
	Igk_System_Init();
		
	/*------------------------------------------------------*/	
	IGK_SysTimePrintln("正在初始化...");
	//初始化Flash
	W25QXX_Init();			//W25QXX初始化
	while(W25QXX_ReadID()!=W25Q128)//检测不到W25Q128，LED闪烁
	{
		delay_ms(100);
	}
	//系统启动，蜂鸣器提示
	for(u8 i=0;i<4;i++)
	{
		BEEP =~BEEP;
		delay_ms(60);
	}

	IGK_SysTimePrintln("Flash Ok!");	
	//初始化内存池[采用原子内存管理]
	my_mem_init(SRAMIN);
	IGK_SysTimePrintln("开启动态内存：%dKByte!",MEM1_MAX_SIZE/1024);	
	//初始化ucos
	IGK_UCOS_Init();	
	while(1);
}

//开始任务函数,创建任务进程，执行一次后自动退出
void start_task(void *p_arg)
{
	p_arg = p_arg;
	//创建任务
	IGK_UCOS_Create(4,sizeof(Task1_STK)/4,Task1_STK,&Task1TCB,Task1_task);
	IGK_UCOS_Create(5,sizeof(Task2_STK)/4,Task2_STK,&Task2TCB,Task2_task);
	IGK_UCOS_Create(6,sizeof(Task3_STK)/4,Task3_STK,&Task3TCB,Task3_task);
	IGK_UCOS_Create(7,sizeof(Task4_STK)/4,Task4_STK,&Task4TCB,Task4_task);
	IGK_UCOS_Create(8,sizeof(Task5_STK)/4,Task5_STK,&Task5TCB,Task5_task);
	IGK_UCOS_Create(9,sizeof(Task6_STK)/4,Task6_STK,&Task6TCB,Task6_task);
	IGK_UCOS_Create(10,sizeof(Task7_STK)/4,Task7_STK,&Task7TCB,Task7_task);
	IGK_UCOS_Create(11,sizeof(Task8_STK)/4,Task8_STK,&Task8TCB,Task8_task);
	IGK_UCOS_Create(12,sizeof(Task9_STK)/4,Task9_STK,&Task9TCB,Task9_task);
	
	IGK_SysTimePrintln("UCOSIII初始化完成,系统进入时间轮片!");
}



/*【任务1】【调试专用】*****************************
****************************************************/
void Task1_task(void *p_arg)
{
	OS_ERR err;  
//	u16 num = 0;
	osdelay_s(1);
	yinling(1);
	IGK_Speek("系统自检完成");
	CPU_STK_SIZE free,used;
	OSTaskStkChk (&Task1TCB,&free,&used,&err);
	IGK_SysTimePrintln("Task1 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
	OSTaskStkChk (&Task2TCB,&free,&used,&err);
	IGK_SysTimePrintln("Task2 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
	OSTaskStkChk (&Task3TCB,&free,&used,&err); 	
	IGK_SysTimePrintln("Task3 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
	OSTaskStkChk (&Task4TCB,&free,&used,&err);
	IGK_SysTimePrintln("Task4 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
	OSTaskStkChk (&Task5TCB,&free,&used,&err);
	IGK_SysTimePrintln("Task5 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
	OSTaskStkChk (&Task6TCB,&free,&used,&err);
	IGK_SysTimePrintln("Task6 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
	OSTaskStkChk (&Task7TCB,&free,&used,&err);
	IGK_SysTimePrintln("Task7 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
	OSTaskStkChk (&Task8TCB,&free,&used,&err);	
	IGK_SysTimePrintln("Task8 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
	OSTaskStkChk (&Task9TCB,&free,&used,&err);	
	IGK_SysTimePrintln("Task9 used/free:%d/%d  Percent:%d",used,free,(used*100)/(used+free));  		
	while(1)
	{
	
		//IGK_SysTimePrintln("Current:%.3fA,Volate:%.2fV,Percent:%d",*IgkSystem.Battery.Current*0.01,*IgkSystem.Battery.Voltage*0.01,*IgkSystem.Battery.Percent);
		//计数测试
//		num++;
//		IGK_SysTimePrintln("计数：%d",num);
		LED1 = ~LED1;
//		delay(0, 0,0 , 20);
		//打印任务堆栈使用量
		osdelay_ms(200);
	}
}

/*【任务2】【自动模式】**************************************************
*                    
****************************************************/
void Task2_task(void *p_arg)
{
	//初始化PID
	PID_Init();
	while(1)
	{
		//自动模式
		if(IgkSystem.WorkMode == Enum_LocalAuto)
		{
			//前进
			if(IgkSystem.Dir == Enum_QianJin)
			{ 
				if(IgkSystem.RunOrStop == Enum_Stop)
					DriverTingZhi();
				else
				{
					switch(IgkSystem.Action)
					{
						case Enum_ZuoXuan://左旋
							DriverZuoXuan(10);
							break;
						case Enum_YouXuan://右旋
							DriverYouXuan(10);
							break;
						case Enum_ZuoFenCha://左分叉
						case Enum_YouFenCha://右分叉
						case Enum_ZhiXing:  //直行
							if(IgkSystem.QianCiDaoHang.Error == 0)
							{
								//PID动态调节循迹
								float Inc = PosPIDCalc(IgkSystem.QianCiDaoHang.Distance);
								//更新速度
								s16 s1 = *IgkSystem.AutoSpeed+Inc;
								s16 s2 = *IgkSystem.AutoSpeed-Inc;
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
			if(IgkSystem.Dir == Enum_HouTui)
			{
				if(IgkSystem.RunOrStop == Enum_Stop)
					DriverTingZhi();
				else
				{
					switch(IgkSystem.Action)
					{
						case Enum_ZuoXuan://左旋
							DriverZuoXuan(10);
							break;
						case Enum_YouXuan://右旋
							DriverYouXuan(10);
							break;
						case Enum_ZuoFenCha://左分叉
						case Enum_YouFenCha://右分叉
						case Enum_ZhiXing:  //直行
							if(IgkSystem.HouCiDaoHang.Error == 0)
							{
								//PID动态调节循迹
								float Inc = PosPIDCalc(-IgkSystem.HouCiDaoHang.Distance);
								//更新速度
								s16 s1 = *IgkSystem.AutoSpeed + Inc;
								s16 s2 = *IgkSystem.AutoSpeed - Inc;
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
		osdelay_ms(10);
	}
}


/*【任务3】【执行自动任务】**************************************************
1.监听执行
2.检查非法
3.计算最有路径
4.执行任务
****************************************************/
void Task3_task(void *p_arg)
{
	p_arg = p_arg;
	while(1)
	{
		GotoStart:
		/*1.-----------------等待执行信号--------------------------*/
		IGK_SysTimePrintln("等待任务!");
		while(*IgkSystem.Task.Execute != DEF_TRUE)
		{
			osdelay_ms(10);
		}
		//清除执行信号
		*IgkSystem.Task.Execute = DEF_FALSE;
		/*2.-----------------判断目标标签合法性--------------------------*/
		if(*IgkSystem.Task.Target == *IgkSystem.RFID)
		{
			IGK_Speek("目标标签和当前标签相同,任务完成！");
			IGK_SysTimePrintln("目标标签和当前标签相同,任务完成！");
			//跳过
			continue;
		}
		else
		if(*IgkSystem.Task.Target == 0)
		{
			IGK_Speek("目标标签为0,操作非法！");
			IGK_SysTimePrintln("目标标签为0,操作非法！");
			//跳过
			continue;
		}
		else
		if(*IgkSystem.Task.Target > NodeMaxNum)
		{
			IGK_Speek("目标标签不能大于%d,操作非法！",NodeMaxNum);
			IGK_SysTimePrintln("目标标签不能大于%d,操作非法！",NodeMaxNum);
			//跳过
			continue;
		}	
		//搜索路径
		FindRoute(*IgkSystem.RFID,*IgkSystem.Task.Target);
		IGK_SysTimePrintln("路径搜索完成!");

		//播报目标位置
		IGK_Speek("准备前往%d号标签",*IgkSystem.Task.Target);
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
			IgkSystem.RunOrStop = Enum_Run;
			for(int i=0;i < BestPath.NodeCount;i++)
			{
                //恢复角度临时变量
                int16_t RecoveryAngle = 0;
				u16 nodeId = BestPath.NodeList[i];
				//读取上一个站点的地图
				StaionMapStruct mapStructLast;
                //读取对应站点的地图
				StaionMapStruct mapStruct;
                if(nodeId == 0)
                    ReadToMapStruct(nodeId,&mapStructLast);
                else
                    ReadToMapStruct(nodeId-1,&mapStructLast);
                
				ReadToMapStruct(nodeId,&mapStruct);
                
                
				IGK_SysPrintf("标签号:[%d]",nodeId);
				//下一个位置更新到界面
				*IgkSystem.Task.Next = nodeId;
				//等待标签,起点不用等待
				if(i!=0)
				{
                    //等待标签
                    IGK_Speek("等待%d号标签!",nodeId);
                    while(*IgkSystem.RFID != nodeId )
                    {
                        //如果取消任务
                        if(*IgkSystem.Task.Cancel == DEF_TRUE)
                        {
                            //停车
                            IgkSystem.RunOrStop = Enum_Stop;
                            //清除取消标记
                            *IgkSystem.Task.Cancel = DEF_FALSE;
                            //更新目标和下一站点位当前
                            *IgkSystem.Task.Next  = *IgkSystem.RFID;
                            *IgkSystem.Task.Target  = *IgkSystem.RFID;
                            IGK_Speek("任务取消成功！");
                            IGK_SysTimePrintln("任务已取消！");
                            //程序退回到等待状态
                            goto GotoStart;
                        }
                        osdelay_ms(5);
                    }
						
                    /*判断是否需要做旋转恢复【程序说明：1.1】************************************
                      如果是最后一个点，直接恢复姿态
                    *****************************************************************************/
                    //判断是否需要旋转
                    for(int j=0;j<StationMapType;j++)
                    {
                        //当前点有通向上一个点的路，并且是左旋或者右旋
                        if(mapStructLast.Stop[j] == BestPath.NodeList[i])
                        {
                            //如果需要旋转
                            if(mapStructLast.AfterAngle[j]!=0)
                            {
                                //到达该站恢复车头方向需要旋转的角度
                                RecoveryAngle = mapStructLast.AfterAngle[j];
                            }
                            break;
                        }
                    }
                    
                    
                    //如果到达终点,不用执行后面的程序，直接退出
                    if(i == BestPath.NodeCount-1)
                    {
                        //停车
                        IgkSystem.RunOrStop = Enum_Stop;
                        if(RecoveryAngle>0)
                        {
                            IGK_Speek("右旋%d度，恢复姿态",RecoveryAngle);
                            IGK_SysPrintf("右旋[%d]度，恢复姿态-",RecoveryAngle);
                            osdelay(0,0,1,0);
                            //旋转
                            IGK_Rotate_Right(RecoveryAngle);
                            osdelay(0,0,2,0);		
                            //切换直行
                            IgkSystem.Action = Enum_ZhiXing;                             
                        }
                        else
                        if(RecoveryAngle<0)
                        { 
                            //改为正数
                            RecoveryAngle = abs(RecoveryAngle);
                            IGK_Speek("左旋%d度，恢复姿态",RecoveryAngle);
                            IGK_SysPrintf("左旋[%d]度，恢复姿态-",RecoveryAngle);
                            osdelay(0,0,1,0);
                            //旋转
                            IGK_Rotate_Left(RecoveryAngle);
                            osdelay(0,0,2,0);	
                            //切换直行
                            IgkSystem.Action = Enum_ZhiXing;                             
                        }
                        break;
                    }

                    //如果是起点或终点，不需要考虑到达站点后的恢复角度和再次出发的旋转角度
                    if(i==0||i == BestPath.NodeCount-1)
                    {
                        //到达该站如果恢复动作需要旋转的角度
                        RecoveryAngle = 0;
                    }
				}
				
				//TODO:
				//这里写站点动作,如停车时间，按键启动等
				
				//执行站点方向，动作
				for(int j=0;j<StationMapType;j++)
				{
          //如果当前动作类型可到达目标和下一点相同
					if(mapStruct.Stop[j] == BestPath.NodeList[i+1])
					{
						//方向
						if(mapStruct.Dir[j] == Enum_QianJin)
						{
							if(IgkSystem.Dir != Enum_QianJin)
							{
								//停止
								IgkSystem.RunOrStop = Enum_Stop;
								DriverTingZhi();
								//切换方向
								IgkSystem.Dir = Enum_QianJin;
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
							if(IgkSystem.Dir != Enum_HouTui)
							{
								//停止
								IgkSystem.RunOrStop = Enum_Stop;
								DriverTingZhi();
								//切换方向
								IgkSystem.Dir = Enum_HouTui;
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
							if(IgkSystem.Dir != Enum_ZuoFenCha)
							{
								osdelay_ms(300);
								IGK_SysPrintf("平移-");
								IGK_Speek("平移");
							}
						}
						
						
						//动作
						if(mapStruct.Action[j] == Enum_ZuoFenCha)
						{
							//修正角度，避免超过180度,并自动旋转
							RepairAngle(RecoveryAngle);
							IgkSystem.Action = Enum_ZuoFenCha;
							IGK_SysPrintf("左分叉-");
							IGK_Speek("左分叉");
						}
						else
						if(mapStruct.Action[j] == Enum_YouFenCha)
						{
							//修正角度，避免超过180度,并自动旋转
							RepairAngle(RecoveryAngle);
							IgkSystem.Action = Enum_YouFenCha;
							IGK_SysPrintf("右分叉-");
							IGK_Speek("右分叉");
						}
						else
						if(mapStruct.Action[j] == Enum_ZhiXing)
						{
							//修正角度，避免超过180度,并自动旋转
							RepairAngle(RecoveryAngle);
							IgkSystem.Action = Enum_ZhiXing;
							IGK_SysPrintf("直行-");
							IGK_Speek("直行");
						}
						else
						if(mapStruct.Action[j] == Enum_ZuoXuan)
						{
							//综合恢复车头方向需要的角度
							RecoveryAngle -= mapStruct.Angle[j];
							//修正角度，避免超过180度,并自动旋转
							RepairAngle(RecoveryAngle);
						}
						else
						if(mapStruct.Action[j] == Enum_YouXuan)
						{
							//综合恢复车头方向需要的角度
							RecoveryAngle += mapStruct.Angle[j];
							//修正角度，避免超过180度,并自动旋转
							RepairAngle(RecoveryAngle);
						}
						
						//启动
						IgkSystem.RunOrStop = Enum_Run;
						//到达
						IGK_SysPrintf("[%d]\r\n\r\n",mapStruct.Stop[j]);
						//找到对应站点，跳出循环
						break;
					}
				}
			}
			//到达目标位置,停车
			IgkSystem.RunOrStop = Enum_Stop;
			IGK_Speek("到达终点，任务完成");
			IGK_SysTimePrintln("到达终点，任务完成");
		}
		osdelay_ms(100);
	}
}

/*【任务4】【读电池状态】**************************************************
*                   
****************************************************/
void Task4_task(void *p_arg)
{
	p_arg = p_arg;
	while(1)
	{
		//读取电池数据
		Igk_Battery_Get();
		osdelay_s(2);
	}
}

/*【任务5】【手动、自动切换】**************************************************
*                   
****************************************************/
void Task5_task(void *p_arg)
{
	while(1)
	{
		//监控摇杆按键,切换模式
		if(IgkSystem.YaoGan.key==Enum_KeyDown)
		{
			osdelay_s(1);
			if(IgkSystem.YaoGan.key==Enum_KeyDown)
			{
				if(IgkSystem.WorkMode == Enum_LocalManual)
				{
					IgkSystem.WorkMode = Enum_LocalAuto;
					IGK_Speek("自动模式");
				}
				else
				{
					IgkSystem.WorkMode = Enum_LocalManual;
					IGK_Speek("手动模式");
				}
			}
		}
		//手动控制模式
		if(IgkSystem.WorkMode == Enum_LocalManual)
		{
			//直接将速度赋值到驱动器
			//判断当前动作
			if(abs(IgkSystem.YaoGan.y)>0)//前进后退
			{
				*IgkSystem.ManualSpeed = abs(IgkSystem.YaoGan.y)*1;
				//转向速度
				s8 speed = IgkSystem.YaoGan.z*0.5;
				if(IgkSystem.YaoGan.y>0)
				{
					if(speed >= 0)
						DriverQinJinSpeed(*IgkSystem.ManualSpeed-speed,*IgkSystem.ManualSpeed);
					else
						DriverQinJinSpeed(*IgkSystem.ManualSpeed,*IgkSystem.ManualSpeed+speed);
				}
				else
				if(IgkSystem.YaoGan.y<0)
				{
					if(speed >= 0)
						DriverHouTuiSpeed(*IgkSystem.ManualSpeed,*IgkSystem.ManualSpeed-speed);
					else
						DriverHouTuiSpeed(*IgkSystem.ManualSpeed+speed,*IgkSystem.ManualSpeed);
				}
			}
			else
			if(abs(IgkSystem.YaoGan.z)>0)//左右旋转
			{
				*IgkSystem.ManualSpeed = abs(IgkSystem.YaoGan.z)*0.8;
				if(IgkSystem.YaoGan.z<0)
					DriverZuoXuan(*IgkSystem.ManualSpeed);
				else
				if(IgkSystem.YaoGan.z>0)
					DriverYouXuan(*IgkSystem.ManualSpeed);
			}
			else // 停止
				DriverTingZhi();
		}
		osdelay_ms(10);
	}
}

/*【任务6】【IO监控】**************************************************
*                   
****************************************************/
void Task6_task(void *p_arg)
{
	while(1)
	{
		if(IN6 == Enum_KeyDown)
		{
			//延时消抖
			osdelay_ms(30);
			if(IN6 == Enum_KeyDown)
			{
				IgkSystem.RunOrStop = Enum_Run;
				IGK_Speek("启动");
				//等待按键抬起
				while(IN6 == Enum_KeyDown){osdelay_ms(5);}
			}
		}
		if(IN7 == Enum_KeyDown)
		{
			//延时消抖
			osdelay_ms(30);
			if(IN7 == Enum_KeyDown)
			{
				IgkSystem.RunOrStop = Enum_Stop;
				IGK_Speek("停止");
				//等待按键抬起
				while(IN7 == Enum_KeyDown){osdelay_ms(5);}
			}
		}
		osdelay_ms(10); //延时5ms
	}
}

/*【任务7】【Modbus】**************************************************
*                 
****************************************************/
void Task7_task(void *p_arg)
{
	p_arg = p_arg;
	while(1)
	{
		//响应地图操作
		Igk_Map_Response();
		//响应电池配置
		Igk_Battery_Response();
		osdelay_ms(100);
	}
}

/*【任务8】【喂狗+系统时间更新】**************************************************   
1.
2.更新系统运行时间
****************************************************/
void Task8_task(void *p_arg)
{
	while(1)
	{
		IWDG_Feed();//喂狗
		//更新系统运行时间
		GetSysRunTime(&IgkSystem.OsTime,p_arg);
		osdelay_ms(200);
	}
}


/*【任务9】【电机驱动】**************************************************   
1.
2.更新系统运行时间
****************************************************/
void Task9_task(void *p_arg)
{
	osdelay_s(1);
    g_motec_init.init_delay_val = 150;
	while(1)
	{
        check_Motec_init();
		osdelay_ms(200);
	}
}

















