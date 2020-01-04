#ifndef __FUNCTION_H
#define __FUNCTION_H	
#include "sys.h"

void Manual_speed(void);
void start(void);
void QJ(void);
void HT(void);
void ZY(void);
void YY(void);
void ZX(void);
void YX(void);
void stop(void);
void delay(u32 h,u32 m,u32 s,u32 ms);
void UserConfigInit(void);
void Motor_SetVul(u8 Motor,u32 PWM);
void Motor_Zzhuan(u8 Motor,u32 PWM);
void Motor_Fzhuan(u8 Motor,u32 PWM);
void Moter_EnableAll(void);
void CanSend_data(u8 dir);
void PID_Adjust(u16 j_speed,float kp,float ki,float kd);
void Clear_ReceBuf(u8 num);
void MotoStop(u8 num);
long FloatTohex(float HEX);
float HexTofloat(long HEX);
void u6_printf(char* fmt,...);
void LiDian_send(void);
u8 HexToChar(u8 bChar);
void StationAction(u16 num);

extern float ADC_POWER;
extern u8 Jhwai_switch,ld_juli,car_statu,Manual_juli;
extern u16 keynumber;
extern u8 CAN1_Sebuf[5],tubian_num,XuanZhuanNUM,PingYiNum;

typedef struct
{
	u8 beep_flag;
	u8 flag_qd;
	u8 flag_tz;
	u8 speek_flag;
	u8 quxiaorenwu_flag;
	u8 flag_szd;
	u8 startAsk;
	u8 stopAsk;
	u8 jiting_flag;	
	u8 speek1_flag;
	u8 speek2_flag;
	u8 speek3_flag;
	u8 yuyin_flag;
}flag;
extern flag All_flag;
typedef struct
{
	u16 voltage;
	u16 MAh;
	u16 Current;
}battery_data;

extern battery_data lidian;

typedef struct
{
	u16 jisu_speed;
	u16 xunzhuan_speed;
	u16 pingyi_speed;
	int moter1_speed;
	int moter2_speed;
	int moter3_speed;
	int moter4_speed;
}sudu;

extern sudu speed;
#endif
