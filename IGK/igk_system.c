#include "igk_system.h"
#include "os.h"

//������AGVϵͳ�ṹ��
IgkSystemStruct IgkSystem;

//ϵͳ�Ĵ���ָ�뼰��ʼֵ����
void Igk_System_Init(void)
{
	/*-----------ϵͳָ�����ͱ���ָ��Ĭ�ϵ�ַ---------------*/
	//
	IgkSystem.RFID = &PLC_Data[41];//ʵʱRFIDֵ,�Ͷ�����������ֵͬ��
	IgkSystem.AutoSpeed = &PLC_Data[120];//�Զ�ģʽ�ٶ�
	IgkSystem.ManualSpeed = &PLC_Data[121];//�ֶ�ģʽ�ٶ�
	
	//����Task
	IgkSystem.Task.Target = &PLC_Data[40];     //Ŀ���ǩ
	IgkSystem.Task.Next = &PLC_Data[42];   //��һ��λ��
	IgkSystem.Task.Execute = &PLC_Data[43];    //ִ��
	IgkSystem.Task.Cancel = &PLC_Data[44];     //ȡ��
	IgkSystem.Task.SerialNum = &PLC_Data[45];  //�����š�ϵͳ�Զ����ӣ���ͨ���ӿڸ��¡�
	IgkSystem.Task.Avoid[0] = &PLC_Data[46];   //���õ�46-55��ռ��10���ռ�
	IgkSystem.Task.Avoid[1] = &PLC_Data[47];   //���õ�46-55��ռ��10���ռ�
	IgkSystem.Task.Avoid[2] = &PLC_Data[48];   //���õ�46-55��ռ��10���ռ�
	IgkSystem.Task.Avoid[3] = &PLC_Data[49];   //���õ�46-55��ռ��10���ռ�
	IgkSystem.Task.Avoid[4] = &PLC_Data[50];   //���õ�46-55��ռ��10���ռ�
	IgkSystem.Task.Avoid[5] = &PLC_Data[51];   //���õ�46-55��ռ��10���ռ�
	IgkSystem.Task.Avoid[6] = &PLC_Data[52];   //���õ�46-55��ռ��10���ռ�
	IgkSystem.Task.Avoid[7] = &PLC_Data[53];   //���õ�46-55��ռ��10���ռ�
	IgkSystem.Task.Avoid[8] = &PLC_Data[54];   //���õ�46-55��ռ��10���ռ�
	IgkSystem.Task.Avoid[9] = &PLC_Data[55];   //���õ�46-55��ռ��10���ռ�
	
	//���
	IgkSystem.Battery.Percent = &PLC_Data[100];  //ʣ������ٷֱ�
	IgkSystem.Battery.Voltage = &PLC_Data[101];   //��ѹ
	IgkSystem.Battery.Current = &PLC_Data[102];	//����
	IgkSystem.Battery.Total = &PLC_Data[103];		//�������
	IgkSystem.Battery.Temperature = &PLC_Data[104];//�¶�
	IgkSystem.Battery.Charge = (enum EnumCharge*)&PLC_Data[105];//�������򡾳�ŵ硿	//PID	
	
	//�������
	IgkSystem.BatteryConfig.Code = &PLC_Data[106];  //������
	IgkSystem.BatteryConfig.Data = &PLC_Data[107];   //Data1�ڸ�λ��Data2�ڵ�λ
	IgkSystem.BatteryConfig.Save = (enum EnumBool*)&PLC_Data[108];		//Ϊ1ʱ����
	
	//PID����
	IgkSystem.PID.SetTarget = (s16*)&PLC_Data[200]; 
	IgkSystem.PID.SumError = (s16*)&PLC_Data[201]; 
	IgkSystem.PID.Error = (s16*)&PLC_Data[202]; 
	IgkSystem.PID.LastError = (s16*)&PLC_Data[203]; 
	IgkSystem.PID.Kp = &PLC_Data[205]; 
	IgkSystem.PID.Ki = &PLC_Data[206]; 
	IgkSystem.PID.Kd = &PLC_Data[207]; 
	
	/*------------�趨Ĭ�ϲ���-------------------------------*/
	//��ʼ��ʱ��
	IgkSystem.OsTime.Hour = 0;
	IgkSystem.OsTime.Minute = 0;
	IgkSystem.OsTime.Second = 0;
	IgkSystem.OsTime.Millisecond = 0;
	
	IgkSystem.WorkMode = Enum_LocalAuto;//�Զ�ģʽ
	IgkSystem.Dir = Enum_QianJin;       //ǰ������
	IgkSystem.Action = Enum_ZhiXing;    //ֱ��
	*IgkSystem.RFID = 1;                //RFIDֵ
	IgkSystem.RunOrStop = Enum_Stop;    //ֹͣ״̬
	*IgkSystem.AutoSpeed = 50;          //�Զ��ٶ�;	
	*IgkSystem.ManualSpeed = 80;        //�ֶ��ٶ�;
	
}

//����У���
u8 IGK_CheckSum(u8 *buf,u16 len)
{
	u8 add = 0;
	for(int i=0;i<len;i++)
	{
		 add +=*buf;
		 buf++;
	}
	return add;
}

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










