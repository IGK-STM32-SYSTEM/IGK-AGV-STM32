#include "igk_run.h"
/****************************************
*左转
*angle:旋转角度
*return:旋转结果，成功或失败
*****************************************/
enum EnumBool IGK_Rotate_Left(u16 angle)
{
	//左旋
	IgkSystem.Action = Enum_ZuoXuan;
	//启动
	IgkSystem.RunOrStop = Enum_Run;
	//根据方向选择磁导航传感器
	Fencha_struct * PCiDaoHang;
	if(IgkSystem.Dir == Enum_QianJin)
		PCiDaoHang = &IgkSystem.QianCiDaoHang;
	else
		PCiDaoHang = &IgkSystem.HouCiDaoHang;
	//等待离开磁条
	while(PCiDaoHang->Error==0)
		osdelay_ms(10);
	//等待检测到磁条[至少两个点并且居中]
	while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
		osdelay_ms(10);
	if(angle==180)
	{
		//等待离开磁条
		while(PCiDaoHang->Error==0)
			osdelay_ms(10);
		//等待检测到磁条[至少两个点并且居中]
		while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
			osdelay_ms(10);
	}
	else
	if(angle==270)
	{
		//等待离开磁条
		while(PCiDaoHang->Error==0)
			osdelay_ms(10);
		//等待检测到磁条[至少两个点并且居中]
		while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
			osdelay_ms(10);
		//等待离开磁条
		while(PCiDaoHang->Error==0)
			osdelay_ms(10);
		//等待检测到磁条[至少两个点并且居中]
		while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
			osdelay_ms(10);
	}
	//停止
	IgkSystem.RunOrStop = Enum_Stop;
	DriverTingZhi();
	return Enum_True;
}
/****************************************
*右转
*angle:旋转角度
*return:旋转结果，成功或失败
*****************************************/
enum EnumBool IGK_Rotate_Right(u16 angle)
{
	//右旋
	IgkSystem.Action = Enum_YouXuan;
	//启动
	IgkSystem.RunOrStop = Enum_Run;
	//根据方向选择磁导航传感器
	Fencha_struct * PCiDaoHang;
	if(IgkSystem.Dir == Enum_QianJin)
		PCiDaoHang = &IgkSystem.QianCiDaoHang;
	else
		PCiDaoHang = &IgkSystem.HouCiDaoHang;
	//等待离开磁条
	while(PCiDaoHang->Error==0)
		osdelay_ms(10);
	//等待检测到磁条[至少两个点并且居中]
	while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
		osdelay_ms(10);
	if(angle==180)
	{
		//等待离开磁条
		while(PCiDaoHang->Error==0)
			osdelay_ms(10);
		//等待检测到磁条[至少两个点并且居中]
		while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
			osdelay_ms(10);
	}
	else
	if(angle==270)
	{
		//等待离开磁条
		while(PCiDaoHang->Error==0)
			osdelay_ms(10);
		//等待检测到磁条[至少两个点并且居中]
		while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
			osdelay_ms(10);
		//等待离开磁条
		while(PCiDaoHang->Error==0)
			osdelay_ms(10);
		//等待检测到磁条[至少两个点并且居中]
		while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
			osdelay_ms(10);
	}
	//停止
	IgkSystem.RunOrStop = Enum_Stop;
	DriverTingZhi();
	return Enum_True;
}
