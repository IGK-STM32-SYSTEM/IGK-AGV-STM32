#include "igk_run.h"
/****************************************
*��ת
*angle:��ת�Ƕ�
*return:��ת������ɹ���ʧ��
*****************************************/
enum EnumBool IGK_Rotate_Left(u16 angle)
{
	//����
	IgkSystem.Action = Enum_ZuoXuan;
	//����
	IgkSystem.RunOrStop = Enum_Run;
	//���ݷ���ѡ��ŵ���������
	Fencha_struct * PCiDaoHang;
	if(IgkSystem.Dir == Enum_QianJin)
		PCiDaoHang = &IgkSystem.QianCiDaoHang;
	else
		PCiDaoHang = &IgkSystem.HouCiDaoHang;
	//�ȴ��뿪����
	while(PCiDaoHang->Error==0)
		osdelay_ms(10);
	//�ȴ���⵽����[���������㲢�Ҿ���]
	while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
		osdelay_ms(10);
	if(angle==180)
	{
		//�ȴ��뿪����
		while(PCiDaoHang->Error==0)
			osdelay_ms(10);
		//�ȴ���⵽����[���������㲢�Ҿ���]
		while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
			osdelay_ms(10);
	}
	else
	if(angle==270)
	{
		//�ȴ��뿪����
		while(PCiDaoHang->Error==0)
			osdelay_ms(10);
		//�ȴ���⵽����[���������㲢�Ҿ���]
		while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
			osdelay_ms(10);
		//�ȴ��뿪����
		while(PCiDaoHang->Error==0)
			osdelay_ms(10);
		//�ȴ���⵽����[���������㲢�Ҿ���]
		while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
			osdelay_ms(10);
	}
	//ֹͣ
	IgkSystem.RunOrStop = Enum_Stop;
	DriverTingZhi();
	return Enum_True;
}
/****************************************
*��ת
*angle:��ת�Ƕ�
*return:��ת������ɹ���ʧ��
*****************************************/
enum EnumBool IGK_Rotate_Right(u16 angle)
{
	//����
	IgkSystem.Action = Enum_YouXuan;
	//����
	IgkSystem.RunOrStop = Enum_Run;
	//���ݷ���ѡ��ŵ���������
	Fencha_struct * PCiDaoHang;
	if(IgkSystem.Dir == Enum_QianJin)
		PCiDaoHang = &IgkSystem.QianCiDaoHang;
	else
		PCiDaoHang = &IgkSystem.HouCiDaoHang;
	//�ȴ��뿪����
	while(PCiDaoHang->Error==0)
		osdelay_ms(10);
	//�ȴ���⵽����[���������㲢�Ҿ���]
	while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
		osdelay_ms(10);
	if(angle==180)
	{
		//�ȴ��뿪����
		while(PCiDaoHang->Error==0)
			osdelay_ms(10);
		//�ȴ���⵽����[���������㲢�Ҿ���]
		while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
			osdelay_ms(10);
	}
	else
	if(angle==270)
	{
		//�ȴ��뿪����
		while(PCiDaoHang->Error==0)
			osdelay_ms(10);
		//�ȴ���⵽����[���������㲢�Ҿ���]
		while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
			osdelay_ms(10);
		//�ȴ��뿪����
		while(PCiDaoHang->Error==0)
			osdelay_ms(10);
		//�ȴ���⵽����[���������㲢�Ҿ���]
		while(PCiDaoHang->Num <3 || abs(PCiDaoHang->Distance) >2)
			osdelay_ms(10);
	}
	//ֹͣ
	IgkSystem.RunOrStop = Enum_Stop;
	DriverTingZhi();
	return Enum_True;
}
