#include "motec.h"
#include "igk_speek.h"
#include "delay.h"
#include "can.h"
#include "sys.h"

wait_CanOpen_init g_motec_init;
//
//驱动器Canopen命令
//
u8 CanOpen_buff[8];
u8 Motec_off = 0;
u8 Motec_enable = 0;
u8 Motec_NoPowerUp = 0;
void AGV_CanOpen_Send(void)		//换到 CAN2
{
	CanOpen_buff[0] = 1;
	CanOpen_buff[1] = 0;
	
	CAN2_Send_Msg(0,(char*)CanOpen_buff,2);
}

void AGV_CanOpen_Send1(void)
{
	CanOpen_buff[0] = 6;
	CanOpen_buff[1] = 0;
	CanOpen_buff[2] = 0;
	CanOpen_buff[3] = 0;	
	CanOpen_buff[4] = 0;
	CanOpen_buff[5] = 0;	

	CAN2_Send_Msg(0x201,(char*)CanOpen_buff,6);
	CAN2_Send_Msg(0x202,(char*)CanOpen_buff,6);
}

void AGV_CanOpen_Send2(void)
{
	CanOpen_buff[0] = 7;
	CanOpen_buff[1] = 0;
	CanOpen_buff[2] = 0;
	CanOpen_buff[3] = 0;	
	CanOpen_buff[4] = 0;
	CanOpen_buff[5] = 0;	

	CAN2_Send_Msg(0x201,(char*)CanOpen_buff,6);
	CAN2_Send_Msg(0x202,(char*)CanOpen_buff,6);
}
void Motec_init(void)		
{
	osdelay(0,0,1,500);
	AGV_CanOpen_Send();
	AGV_CanOpen_Send1();
	AGV_CanOpen_Send2();
	
	osdelay(0,0,1,500);
}

//速度
void AGV_CanOpen_Send3(u8 node_id,int speed) 
{
	u16 temp_COB_ID=0;
//	if(AgvSystem.Waring.Action==enumWaringAction.Normal)	
//	{
//		speed=-speed;
//	}
//	else
//	if(AgvSystem.Waring.Action==enumWaringAction.Stop)	
//	{
//		speed = 0;
//	}
    speed=-speed;

	if(node_id > 127)					//节点ID不能超过127
	{
		node_id = 127;
	}
	temp_COB_ID = 0x200+node_id;		//11位COB_ID
	
	CanOpen_buff[0] = 0x0f;
	CanOpen_buff[1] = 0;
	CanOpen_buff[2] = speed & 0xff;
	CanOpen_buff[3] = (speed>>8) & 0xff;	
	
	CAN2_Send_Msg(temp_COB_ID,(char*)CanOpen_buff,4);
}


//角度
void AGV_CanOpen_Send_Dir() 
{
	u16 temp_COB_ID=0;

	temp_COB_ID = 0x0a;		//
	
	CanOpen_buff[0] = 0x04;
	CanOpen_buff[1] = temp_COB_ID;
	CanOpen_buff[2] = 0x01;
	CanOpen_buff[3] = 0x00;	
	
	CAN2_Send_Msg(temp_COB_ID,(char*)CanOpen_buff,4);

	temp_COB_ID = 0x0B;		//
	
	CanOpen_buff[0] = 0x04;
	CanOpen_buff[1] = temp_COB_ID;
	CanOpen_buff[2] = 0x01;
	CanOpen_buff[3] = 0x00;	
	
	CAN2_Send_Msg(temp_COB_ID,(char*)CanOpen_buff,4);
}




void check_Motec_init(void)
{
	g_motec_init.Init_OK = 0;						//驱动使能ok标志清零
//	g_Warning.Drive_NOPwoer = 1;
	
	Motec_init();									//AGV系统上电发送初始化命令
	while(1)
	{
		//判断驱动使能与否
		if(g_motec_init.zijian_enable)				//驱动器使能
		{
			g_motec_init.flag_NoPowerUp = 0;		//驱动上电
			Motec_enable = 1;	//语音_驱动使能
			osdelay(0,0,0,20);
            IGK_Speek("驱动使能");
//			DwqXunZheng_QH();						//电位器寻正	
			g_motec_init.Init_OK = 1;			
//			g_Warning.Drive_NOPwoer = 0;
			
		}
		else
		{		
			g_motec_init.flag_NoPowerUp = 1;		//驱动未上电
            IGK_Speek("驱动未使能");
			g_motec_init.Init_OK = 0;
//			g_Warning.Drive_NOPwoer = 1;
			g_motec_init.zijian_shangdian = 0;
		}
		
		//驱动器使能的循环
		while(g_motec_init.zijian_enable)			
		{
			g_motec_init.zijian_enable--;			//150
			osdelay(0,0,0,20);	
		}
		g_motec_init.zijian_enable = 0;				//自减为0 -- 驱动无电
		
		//判断驱动是未上电还是断电
		if(g_motec_init.flag_NoPowerUp)				
		{
			Motec_NoPowerUp = 1;	//驱动未上电
			osdelay(0,0,0,20);
		}
		else
		{
			Motec_off = 1;		//驱动断电
			osdelay(0,0,0,20);	
            IGK_Speek("驱动断电");
		}
		g_motec_init.Init_OK = 0;
//		g_Warning.Drive_NOPwoer = 1;
		g_motec_init.zijian_shangdian = 0;
		
		//驱动器掉电的循环	
		while(!g_motec_init.zijian_shangdian)		//等待驱动器上电 -- 0x7F
		{	
			osdelay(0,0,0,20);
		}	
		Motec_init();								//上电发初始化命令
		
		osdelay(0,0,1,10);
	}
}










u16 MaxSpeed = 1500;
//电机正转
void Motor_Zzhuan(u8 Motor,u32 speed)
{	
	if(speed > MaxSpeed)
	{
		speed =MaxSpeed;
	}
	switch(Motor)
	{
		case 1://电机1		
			AGV_CanOpen_Send3(1,speed);
		break;
		case 2://电机2
			AGV_CanOpen_Send3(2,speed);
		break;                                                                 
		default:
		break;
	}
}
//电机反转
void Motor_Fzhuan(u8 Motor,u32 speed)
{
	int tmep_spd=0;
	if(speed == 0)
	{
		tmep_spd = 0;
	}
	else if(speed > MaxSpeed)
	{
		tmep_spd = -MaxSpeed;
	}
	else
	{
		tmep_spd = -speed;
	}
	switch(Motor)
	{
		case 1:									//电机1		
			AGV_CanOpen_Send3(1,tmep_spd);
		break;
		case 2:									//电机2
			AGV_CanOpen_Send3(2,tmep_spd);
		break;                                  
		default:
		break;
	}
}





















