#include "motec.h"
#include "igk_speek.h"
#include "delay.h"
#include "can.h"
#include "sys.h"

wait_CanOpen_init g_motec_init;
//
//������Canopen����
//
u8 CanOpen_buff[8];
u8 Motec_off = 0;
u8 Motec_enable = 0;
u8 Motec_NoPowerUp = 0;
void AGV_CanOpen_Send(void)		//���� CAN2
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

//�ٶ�
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

	if(node_id > 127)					//�ڵ�ID���ܳ���127
	{
		node_id = 127;
	}
	temp_COB_ID = 0x200+node_id;		//11λCOB_ID
	
	CanOpen_buff[0] = 0x0f;
	CanOpen_buff[1] = 0;
	CanOpen_buff[2] = speed & 0xff;
	CanOpen_buff[3] = (speed>>8) & 0xff;	
	
	CAN2_Send_Msg(temp_COB_ID,(char*)CanOpen_buff,4);
}


//�Ƕ�
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
	g_motec_init.Init_OK = 0;						//����ʹ��ok��־����
//	g_Warning.Drive_NOPwoer = 1;
	
	Motec_init();									//AGVϵͳ�ϵ緢�ͳ�ʼ������
	while(1)
	{
		//�ж�����ʹ�����
		if(g_motec_init.zijian_enable)				//������ʹ��
		{
			g_motec_init.flag_NoPowerUp = 0;		//�����ϵ�
			Motec_enable = 1;	//����_����ʹ��
			osdelay(0,0,0,20);
            IGK_Speek("����ʹ��");
//			DwqXunZheng_QH();						//��λ��Ѱ��	
			g_motec_init.Init_OK = 1;			
//			g_Warning.Drive_NOPwoer = 0;
			
		}
		else
		{		
			g_motec_init.flag_NoPowerUp = 1;		//����δ�ϵ�
            IGK_Speek("����δʹ��");
			g_motec_init.Init_OK = 0;
//			g_Warning.Drive_NOPwoer = 1;
			g_motec_init.zijian_shangdian = 0;
		}
		
		//������ʹ�ܵ�ѭ��
		while(g_motec_init.zijian_enable)			
		{
			g_motec_init.zijian_enable--;			//150
			osdelay(0,0,0,20);	
		}
		g_motec_init.zijian_enable = 0;				//�Լ�Ϊ0 -- �����޵�
		
		//�ж�������δ�ϵ绹�Ƕϵ�
		if(g_motec_init.flag_NoPowerUp)				
		{
			Motec_NoPowerUp = 1;	//����δ�ϵ�
			osdelay(0,0,0,20);
		}
		else
		{
			Motec_off = 1;		//�����ϵ�
			osdelay(0,0,0,20);	
            IGK_Speek("�����ϵ�");
		}
		g_motec_init.Init_OK = 0;
//		g_Warning.Drive_NOPwoer = 1;
		g_motec_init.zijian_shangdian = 0;
		
		//�����������ѭ��	
		while(!g_motec_init.zijian_shangdian)		//�ȴ��������ϵ� -- 0x7F
		{	
			osdelay(0,0,0,20);
		}	
		Motec_init();								//�ϵ緢��ʼ������
		
		osdelay(0,0,1,10);
	}
}










u16 MaxSpeed = 1500;
//�����ת
void Motor_Zzhuan(u8 Motor,u32 speed)
{	
	if(speed > MaxSpeed)
	{
		speed =MaxSpeed;
	}
	switch(Motor)
	{
		case 1://���1		
			AGV_CanOpen_Send3(1,speed);
		break;
		case 2://���2
			AGV_CanOpen_Send3(2,speed);
		break;                                                                 
		default:
		break;
	}
}
//�����ת
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
		case 1:									//���1		
			AGV_CanOpen_Send3(1,tmep_spd);
		break;
		case 2:									//���2
			AGV_CanOpen_Send3(2,tmep_spd);
		break;                                  
		default:
		break;
	}
}





















