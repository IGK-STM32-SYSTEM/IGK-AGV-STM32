#include "igk_system.h"
#include "os.h"

//������AGVϵͳ�ṹ��
IgkSystemStruct IgkSystem;

//����תʱ��ṹ��
void MillisecondToDateTime(u32 millis,IGK_Struct_DateTime *dt)
{
	//����תʱ��
	u32 temp = millis;
	u32 hper = 60 * 60 * 1000;
	u32 mper = 60 * 1000;
	u32 sper = 1000;
	u16 hh=0,mm=0,ss=0,mmm=0;
	if (temp / hper > 0) {
		hh  = temp / hper;
	}
	temp = temp % hper;
	if (temp / mper > 0) {
		mm = temp / mper;
	}
	temp = temp % mper;
	if (temp / sper > 0) {
		ss = temp / sper;
	}
	mmm = temp % sper;
	dt->Hour = hh;
	dt->Minute = mm;
	dt->Second = ss;
	dt->Millisecond = mmm;
}

//��ȡϵͳ����ʱ��
void GetSysRunTime(IGK_Struct_DateTime *dt,void *p_arg)
{
	//��ӡϵͳʱ��
	u32 tick = OSTimeGet(p_arg);
	//ϵͳ������5
	tick*=5;
	MillisecondToDateTime(tick,dt);
}










