#include "route.h"
u16 SystemParameter[SystemParaNum];//系统参数

u16 RouteStationNum[RouteNum]; 		//每条路径包含站点数
u16 NowRouteInfor[StationNum][StaiionInfor];   //当前路径信息
u16 StationInfor[StaiionInfor]; 
u16 HmiRouteNumFlag = 0; //上一个路径号，判断路径变化用
u16 HmiRouteCopiedNumTemp=0;//被复制路径号临时变量


u16 ProcessStepNum[ProcessNum]; 		   //每个流程包含步数
u16 NowProcessInfor[StepNum][StepInfor];  //当前流程信息
//u16 NowStepInfor[StepInfor];              //单个步骤信息

u16 HmiProcessNumFlag = 0; //上一个流程号，判断流程变化用
u16 HmiProcessCopiedNumTemp=0;//被复制流程号临时变量



//---------------系统------------//
//从系统获取一个参数 num:参数位置
u16 GetOneParameterFromSystem(u16 num)
{
	u16 buf[1];
	//读取系统参数	
	W25QXX_Read_16(buf,num,1);
	return buf[0];
}

//获取所有系统参数，并存入默认数组SystemParameter
void GetAllParameterFromSystem()
{
	//读取系统参数	
	W25QXX_Read_16(SystemParameter,0,SystemParaNum);
}


//设置一个参数到系统 value:参数值 num:参数序号
void SetOneParameterToSystem(u16 value,u16 num)
{
	u16 buf[1];
	buf[0] = value;
	W25QXX_Write_16(buf,num,1);
}

//设置所有参数到系统 将SystemParameter数组的数据存入Flash
void SetAllParameterToSystem()
{
	W25QXX_Write_16(SystemParameter,0,SystemParaNum);
}
//---------------路径------------//

//获取路径包含站点数 num:路径号
u16 GetRouteStationNum(u16 num)
{
	//读取
	W25QXX_Read_16(&RouteStationNum[num-1],RouteStationNumAdress(num),1);	
	
	//超出范围置零
	if(RouteStationNum[num-1]>StationNum)
		RouteStationNum[num-1] = 0;
	return RouteStationNum[num-1];
}

//保存所有路径包含的站点数
void SetAllStationNum()
{
	W25QXX_Write_16(RouteStationNum,RouteStationNumAdress(1),RouteNum);
}
//获取路径信息,num:路径号
void GetRouteData(u16 num)
{
	u16 i;
	//读取站点数
	HmiStationNum = GetRouteStationNum(num);
	
	//读取站点信息
	for(i=0;i<RouteStationNum[num-1];i++)
	{
		W25QXX_Read_16(&NowRouteInfor[i][0],NowStationInforAdress(num,i),StaiionInfor);
	}
}


//保存路径信息,num:路径号
void SetRouteData(u16 num)
{
	u16 i;
	//保存路径包含站点数
	RouteStationNum[HmiRouteNum-1] = HmiStationNum;
	//保存
	W25QXX_Write_16(&RouteStationNum[HmiRouteNum-1],RouteStationNumAdress(HmiRouteNum),1);	

	//保存路径站点包含信息
	for(i=0;i<RouteStationNum[num-1];i++)
	{
		W25QXX_Write_16(&NowRouteInfor[i][0],NowStationInforAdress(num,i),StaiionInfor);	
	}
}

//保存站点信息,num:路径号，stationnum
void SetStationData(u16 num,u16 stationnum)
{
	//保存路径包含站点数
	RouteStationNum[HmiRouteNum-1] = HmiStationNum;
	//保存
	W25QXX_Write_16(&RouteStationNum[HmiRouteNum-1],RouteStationNumAdress(HmiRouteNum),1);	

	//保存路径站点包含信息
	W25QXX_Write_16(&NowRouteInfor[stationnum][0],NowStationInforAdress(num,stationnum),StaiionInfor);	
}

//更新路径站点数据到界面
void UpdataStationToHmi()
{
		HmiRoadData1  = NowRouteInfor[HmiStationSerialNum-1][0];//远程红外 默认开
		HmiRoadData2  = NowRouteInfor[HmiStationSerialNum-1][1];//岔路口
		HmiRoadData3  = NowRouteInfor[HmiStationSerialNum-1][2];//突变次数
		HmiRoadData4  = NowRouteInfor[HmiStationSerialNum-1][3];//旋转次数
		HmiRoadData5  = NowRouteInfor[HmiStationSerialNum-1][4];//档位默认选择3
		HmiRoadData6  = NowRouteInfor[HmiStationSerialNum-1][5];//地标	
		HmiRoadData7  = NowRouteInfor[HmiStationSerialNum-1][6];//动作	0：停止	1：前进	2:后左	3：后右 4：通过
		HmiRoadData8  = NowRouteInfor[HmiStationSerialNum-1][7];//0：自动 1：人工
		HmiRoadData9  = NowRouteInfor[HmiStationSerialNum-1][8];//时间
}

//更新路径站点数据到当前
void UpdataStationToNow()
{
		NowRouteInfor[HmiStationSerialNum-1][0]  = HmiRoadData1 ;//远程红外 默认开
		NowRouteInfor[HmiStationSerialNum-1][1]  = HmiRoadData2 ;//岔路口
		NowRouteInfor[HmiStationSerialNum-1][2]  = HmiRoadData3 ;//突变次数
		NowRouteInfor[HmiStationSerialNum-1][3]  = HmiRoadData4 ;//旋转次数
		NowRouteInfor[HmiStationSerialNum-1][4]  = HmiRoadData5 ;//档位默认选择3
		NowRouteInfor[HmiStationSerialNum-1][5]  = HmiRoadData6 ;//地标	
		NowRouteInfor[HmiStationSerialNum-1][6]  = HmiRoadData7 ;//动作	0：停止	1：前进	2:后左	3：后右 4：通过
		NowRouteInfor[HmiStationSerialNum-1][7]  = HmiRoadData8 ;//0：自动 1：人工
		NowRouteInfor[HmiStationSerialNum-1][8]  = HmiRoadData9 ;//时间
}




//更新路径站点数据到界面
void ClearStationToHmi()
{
	//默认站点信息
	HmiRoadData1  = 0;	//远程红外 默认为0 0：保持1：开2：关
	HmiRoadData2  = 0;	//岔路口左分叉 0：保持1：左分叉 2：右分叉	
	HmiRoadData3  = 1;	//突变次数
	HmiRoadData4  = 1;	//旋转次数	
	HmiRoadData5  = 0;	//档位默认选择慢速 0：保持 1：慢速 2：中速 3：快速   	       
	HmiRoadData6  = 0;	//地标默认0	
	HmiRoadData7  = 0;	//动作	0：停止	1：前进	2:后左	3：后右  4：通过
	HmiRoadData8  = 0;	//0：自动 1：手动
	HmiRoadData9  = 0;	//时间
}


//---------------流程------------//

//保存及更新流程数据（当前操作的流程信息）
void SetProcessData()
{
	u16 i = 0;
	//流程步数
	
	ProcessStepNum[HmiProcessNum-1] = HmiStepNum;
	//保存
	W25QXX_Write_16(&ProcessStepNum[HmiProcessNum-1],ProcessStepNumAdress(HmiProcessNum),1);	

	
	//流程信息
	//更新当前单步的起始路径号
	NowProcessInfor[HmiProcessSerialNum-1][0] = HmiProcessStartNum;
	//更新当前单步的动作
	NowProcessInfor[HmiProcessSerialNum-1][1] = HmiProcessAction;
	//更新当前单步的结束路径号或重复执行次数
	NowProcessInfor[HmiProcessSerialNum-1][2] = HmiProcessStopNum;
	
	//保存
	for(i=0;i<HmiStepNum;i++)
		W25QXX_Write_16(&NowProcessInfor[i][0],NowStepInforAdress(HmiProcessNum,i),StepInfor);	
}


//获取流程包含步数
u16	GetProcessStepNum(u16 num)
{
	//读取
	W25QXX_Read_16(&ProcessStepNum[num-1],ProcessStepNumAdress(num),1);	
	
	return ProcessStepNum[num-1];
}

//保存所有流程包含步数
void SetAllStepNum()
{
	W25QXX_Write_16(ProcessStepNum,ProcessStepNumAdress(1),ProcessNum);
}

//获取及更新流程数据（当前操作的流程信息）
void GetProcessData()
{
	u16 i = 0;
	//流程步数
	
	//读取
	W25QXX_Read_16(&ProcessStepNum[HmiProcessNum-1],ProcessStepNumAdress(HmiProcessNum),1);	
	//更新
	HmiStepNum = ProcessStepNum[HmiProcessNum-1];
	
	HmiProcessSerialNum = 1;
	
	//流程步数超过设定最大值，直接置0
	if(HmiStepNum>StepNum)
	{
		//流程数设置为0
		HmiStepNum = 0;
		//序号设置为0
		HmiProcessSerialNum = 0;
	}
	
	//流程信息
	//读取
	for(i=0;i<HmiStepNum;i++)
		W25QXX_Read_16(&NowProcessInfor[i][0],NowStepInforAdress(HmiProcessNum,i),StepInfor);	
	
	//更新当前单步的起始路径号
	HmiProcessStartNum = 	NowProcessInfor[HmiProcessSerialNum-1][0];
	//更新当前单步的动作
	HmiProcessAction = 	NowProcessInfor[HmiProcessSerialNum-1][1];
	//更新当前单步的结束路径号或重复执行次数
	HmiProcessStopNum = 	NowProcessInfor[HmiProcessSerialNum-1][2];
}


//获取及更新指定流程数据（指定的流程信息）
void	GetProcessDataFrom(u16 num)
{
	u16 i = 0;
	//流程步数
	
	//读取
	W25QXX_Read_16(&ProcessStepNum[num-1],ProcessStepNumAdress(num),1);	
	//更新
	HmiStepNum = ProcessStepNum[num-1];
	
	HmiProcessSerialNum = 1;
	
	//流程步数超过设定最大值，直接置0
	if(HmiStepNum>StepNum)
	{
		//流程数设置为0
		HmiStepNum = 0;
		//序号设置为0
		HmiProcessSerialNum = 0;
	}
	
	//流程信息
	//读取
	for(i=0;i<HmiStepNum;i++)
		W25QXX_Read_16(&NowProcessInfor[i][0],NowStepInforAdress(num,i),StepInfor);	
	
	//更新当前单步的起始路径号
	HmiProcessStartNum = 	NowProcessInfor[HmiProcessSerialNum-1][0];
	//更新当前单步的动作
	HmiProcessAction = 	NowProcessInfor[HmiProcessSerialNum-1][1];
	//更新当前单步的结束路径号或重复执行次数
	HmiProcessStopNum = 	NowProcessInfor[HmiProcessSerialNum-1][2];
}



//更新流程数据到界面
void UpdataProcessToHmi()
{
	//更新当前单步的起始路径号
	HmiProcessStartNum = 	NowProcessInfor[HmiProcessSerialNum-1][0];
	//更新当前单步的动作
	HmiProcessAction = 	NowProcessInfor[HmiProcessSerialNum-1][1];
	//更新当前单步的结束路径号或重复执行次数
	HmiProcessStopNum = 	NowProcessInfor[HmiProcessSerialNum-1][2];
}

//更新流程数据到界面
void ClearStepToHmi()
{
	//更新当前单步的起始路径号
	HmiProcessStartNum = 1;
	//更新当前单步的动作
	HmiProcessAction = 0;
	//更新当前单步的结束路径号或重复执行次数
	HmiProcessStopNum = 0;
}
//插入及更新流程数据（当前操作的流程信息）
void	InsertProcessData()
{
	u16 i,j;
	//流程步数
	
	ProcessStepNum[HmiProcessNum-1] = HmiStepNum;
	//保存
	W25QXX_Write_16(&ProcessStepNum[HmiProcessNum-1],ProcessStepNumAdress(HmiProcessNum),1);	

	
	//流程信息
	//将当前及后面所数据向后移动一位
	for(i=HmiStepNum;i>=HmiProcessSerialNum;i--)
	{
		for(j=0;j<StepInfor;j++)
		{
			NowProcessInfor[i-1][j] = NowProcessInfor[i-1-1][j];
		}
	}
	
	//更新新插入数据
	
	//更新当前单步的起始路径号
	NowProcessInfor[HmiProcessSerialNum-1][0] = HmiProcessStartNum;
	//更新当前单步的动作
	NowProcessInfor[HmiProcessSerialNum-1][1] = HmiProcessAction;
	//更新当前单步的结束路径号或重复执行次数
	NowProcessInfor[HmiProcessSerialNum-1][2] = HmiProcessStopNum;

		
	//保存
	for(i=0;i<HmiStepNum;i++)
		W25QXX_Write_16(&NowProcessInfor[i][0],NowStepInforAdress(HmiProcessNum,i),StepInfor);	
}


//删除及更新流程数据（当前操作的流程信息）
void DeleteProcessData()
{
	u16 i,j;
	//流程步数
	if(HmiStepNum>0)
	{
		//将后面所数据向前移动一位
		if((HmiStepNum>0)&&(HmiStepNum!=HmiProcessSerialNum))
		{
			for(i=HmiProcessSerialNum;i<HmiStepNum;i++)
			{
				for(j=0;j<StepInfor;j++)
				{
					NowProcessInfor[i-1][j] = NowProcessInfor[i][j];
				}
			}
		}
		
		HmiStepNum--;
		if(HmiStepNum==0)
			HmiProcessSerialNum = 0;
		else
			if((HmiStepNum+1)==HmiProcessSerialNum)
				HmiProcessSerialNum--;
		
		//保存
		for(i=0;i<HmiStepNum;i++)
			W25QXX_Write_16(&NowProcessInfor[i][0],NowStepInforAdress(HmiProcessNum,i),StepInfor);	
		
		ProcessStepNum[HmiProcessNum-1] = HmiStepNum;
		//保存
		W25QXX_Write_16(&ProcessStepNum[HmiProcessNum-1],ProcessStepNumAdress(HmiProcessNum),1);	

		//更新界面
		UpdataProcessToHmi();	
	}
}

//自动界面显示信息重载
void HmiAutoReload()
{
	//如果正在执行流程，则重载路径号
	if(HmiTask == 2)
	{
		GetProcessData();
		HmiRouteNum = NowProcessInfor[0][0];
	}
	//重载站点数
	//获取路径包含站点数
	HmiStationNum = GetRouteStationNum(HmiRouteNum);
	//当前地标清零
	HmiRfidNum = 0;
	
	//获取当前路径信息
	GetRouteData(HmiRouteNum);	
	//重载下一地标
	HmiNextRfidNum = NowRouteInfor[0][5];
}































