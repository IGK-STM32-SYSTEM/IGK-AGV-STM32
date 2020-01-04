/******************************************
����������Ƽ����޹�˾
ϵͳ����
***********************************************/
#ifndef __IGK_OS_H
#define __IGK_OS_H
#include "sys.h"
#include "cdhdata.h"
typedef unsigned short int u16;
typedef unsigned char u8;
typedef  unsigned  int u32;
#define igk_SetBit(x,y) x|=(1<<y)//ָ����ĳһλ����1
#define igk_ClearBit(x,y) x&=~(1<<y)//ָ����ĳһλ����0
#define igk_ReverseBit(x,y) x^=(1<<y)//ָ����ĳһλ��ȡ��
#define igk_GetBit(x,y) ((x) >> (y)&1)//��ȡ��ĳһλ��ֵ
//����ö��,ȫ��ͨ��
enum ButtonEnum {KeyDown,KeyUp};
//����ģʽ���ֶ��������Զ���Զ�̿��ơ�
enum WorkModeEnum {Enum_LocalManual,Enum_LocalAuto,Enum_RemoteControl};
//��ͣ״̬��������ֹͣ��������ֹͣ��������ֱ��·��ʱ������ֹͣ��־�����ϰ����ֹͣ��һ��
enum RunOrStopEnum {Enum_Run,Enum_Stop};
//����ö�� 0:ǰ��,1:����,2:ƽ��
enum EnumDir{ Enum_QianJin, Enum_HouTui, Enum_PingYi};
//����ö�� 0:��ֲ棬1:�ҷֲ棬2:���ƣ�3:���ƣ�4:ֱ�У�5:������6:����
enum EnumAction{ Enum_ZuoFenCha, Enum_YouFenCha, Enum_ZuoYi, Enum_YouYi, Enum_ZhiXing, Enum_ZuoXuan, Enum_YouXuan};

//ҡ�˽ṹ��
typedef struct {
	signed char x;//[-100,100]
	signed char y;//[-100,100]
	signed char z;//[-100,100]
	enum ButtonEnum key;//KeyDown,KeyUp
}IGK_Struct_YaoGan;

//ʱ��ṹ��
typedef struct {
	u16 Hour;//ʱ
	u8 Minute;//��
	u8 Second;//��
	u16 Millisecond;//����
}IGK_Struct_DateTime;

//����ģʽ
typedef struct {
	u16 Manual;//ʱ
	u8 Auto;//��
	u8 Second;//��
	u16 Millisecond;//����
}IGK_Struct_WorkMode;

//ϵͳ�ṹ��,ȫ��ͨ��
typedef struct {
	//ϵͳ����ʱ��
	IGK_Struct_DateTime OsTime;
	//ҡ�˿�����
	IGK_Struct_YaoGan YaoGan;
	//��ǰ����ģʽ
	enum WorkModeEnum WorkMode;
	//�ٶ�
	u16 Speed;
	//��ͣ״̬
	enum RunOrStopEnum RunOrStop;
	//�Զ��˶�����
	enum EnumDir Dir;
	//�Զ��˶�����Ϊ
	enum EnumAction Action;
	//ǰ�ŵ���
	Fencha_struct QianCiDaoHang;
	//��ŵ���
	Fencha_struct HouCiDaoHang;
	//RFID
	u16 RFID;
	//Ŀ��λ��
	u16 Target;
	//��һ��λ��
	u16 Next;
	//
	
}IgkAgvOsStruct;






//������AGVϵͳ�ṹ��
extern IgkAgvOsStruct IgkAgvOs;
//����תʱ��ṹ��
void MillisecondToDateTime(u32 millis,IGK_Struct_DateTime *dt);
//��ȡϵͳ����ʱ��
void GetSysRunTime(IGK_Struct_DateTime *dt,void *p_arg);

#endif












