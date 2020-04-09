#ifndef _MOTEC_H
#define _MOTEC_H
#include "sys.h"

#define MOTEC_Send_delay 0
typedef struct
{

	u8 Init_OK;						//初始化ok
	u8 init_delay_val ;				//等待填充值
	u8 zijian_enable ;				//等待500ms
	u8 zijian_shangdian ;			//等待500ms
	
	u8 flag_NoPowerUp;				//未上电标志|区别于断电

}wait_CanOpen_init;

extern wait_CanOpen_init g_motec_init;

void AGV_CanOpen_Send(void);
void AGV_CanOpen_Send1(void);
void AGV_CanOpen_Send2(void);
void AGV_CanOpen_Send3(u8 node_id,int speed);
void Motec_init(void);

void check_Motec_init(void);

//角度
void AGV_CanOpen_Send_Dir(void);

//电机正转
void Motor_Zzhuan(u8 Motor,u32 speed);
//电机反转
void Motor_Fzhuan(u8 Motor,u32 speed);

#endif





