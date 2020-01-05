#include "igk_map.h"

//����ָ��,ָ��ͨ�ŽӿڼĴ�����ַ
u16 *ApiRegister = &PLC_Data[0];

/*******************************************************
* ���ƣ���վ���ͼ��������
* num:վ���
* buffer:վ���ͼ������
*******************************************************/
void ReadMapToBuffer(u16 num,u16* buffer){
	W25QXX_Read_16(buffer,StationStartAddress(num),StationMapSpace);
}
/*******************************************************
* ���ƣ�վ���ͼ������תվ���ͼ�ṹ��
* num:վ���
* buffer:վ���ͼ������
* mapStruct:վ���ͼ�ṹ��ָ��
*******************************************************/
void BufferToMapStruct(u16 num,u16* buffer,StaionMapStruct *mapStruct){
	//ѭ����ȡ��ͼ��Ϣ,����������ͼ�ṹ��
	for(u8 i=0;i<StationMapType;i++)
	{
		//��ǰվ��
		mapStruct->Start[i] = num;
		//��λȡ����
		u8 bit = igk_GetBit(buffer[0],i);
		switch(i)
		{
			case 0://ǰ��-����
				if(bit==0){
					mapStruct->Stop[i] = buffer[1];//Ŀ��վ��
					mapStruct->Dir[i] = Enum_QianJin;//����
					mapStruct->Action[i] = Enum_ZuoXuan;
					mapStruct->Angle[i] = buffer[13];//��ת�Ƕ�
				}
				else
				//û�����ã�Ŀ��վ����Ϊ0
				{
					mapStruct->Stop[i] = 0;
					mapStruct->Angle[i] = 0;//��ת�Ƕ�
				}
				break;
			case 1://ǰ��-��ֲ�
				if(bit==0){
					mapStruct->Stop[i] = buffer[2];//Ŀ��վ��
					mapStruct->Dir[i] = Enum_QianJin;//����
					mapStruct->Action[i] = Enum_ZuoFenCha;
					mapStruct->Angle[i] = 0;//��ת�Ƕ�
				}
				else
					//û�����ã�Ŀ��վ����Ϊ0
					mapStruct->Stop[i] = 0;
				break;
			case 2://ǰ��-ֱ��
				if(bit==0){
					mapStruct->Stop[i] = buffer[3];//Ŀ��վ��
					mapStruct->Dir[i] = Enum_QianJin;//����
					mapStruct->Action[i] = Enum_ZhiXing;
					mapStruct->Angle[i] = 0;//��ת�Ƕ�
				}
				else
					//û�����ã�Ŀ��վ����Ϊ0
					mapStruct->Stop[i] = 0;
				break;
			case 3://ǰ��-�ҷֲ�
				if(bit==0){
					mapStruct->Stop[i] = buffer[4];//Ŀ��վ��
					mapStruct->Dir[i] = Enum_QianJin;//����
					mapStruct->Action[i] = Enum_YouFenCha;
					mapStruct->Angle[i] = 0;//��ת�Ƕ�
				}
				else
					//û�����ã�Ŀ��վ����Ϊ0
					mapStruct->Stop[i] = 0;
				break;
			case 4://ǰ��-����
				if(bit==0){
					mapStruct->Stop[i] = buffer[5];//Ŀ��վ��
					mapStruct->Dir[i] = Enum_QianJin;//����
					mapStruct->Action[i] = Enum_YouXuan;
					mapStruct->Angle[i] = buffer[14];//��ת�Ƕ�
				}
				else
				//û�����ã�Ŀ��վ����Ϊ0
				{
					mapStruct->Stop[i] = 0;
					mapStruct->Angle[i] = 0;//��ת�Ƕ�
				}
				break;
			case 5://ƽ��-����
				if(bit==0){
					mapStruct->Stop[i] = buffer[6];//Ŀ��վ��
					mapStruct->Dir[i] = Enum_PingYi;//����
					mapStruct->Action[i] = Enum_ZuoYi;
					mapStruct->Angle[i] = 0;//��ת�Ƕ�
				}
				else
					//û�����ã�Ŀ��վ����Ϊ0
					mapStruct->Stop[i] = 0;
				break;
			case 6://ƽ��-����
				if(bit==0){
					mapStruct->Stop[i] = buffer[7];//Ŀ��վ��
					mapStruct->Dir[i] = Enum_PingYi;//����
					mapStruct->Action[i] = Enum_YouYi;
					mapStruct->Angle[i] = 0;//��ת�Ƕ�
				}
				else
					//û�����ã�Ŀ��վ����Ϊ0
					mapStruct->Stop[i] = 0;
				break;
			case 7://����-����
				if(bit==0){
					mapStruct->Stop[i] = buffer[8];//Ŀ��վ��
					mapStruct->Dir[i] = Enum_HouTui;//����
					mapStruct->Action[i] = Enum_ZuoXuan;
					mapStruct->Angle[i] = buffer[15];//��ת�Ƕ�
				}
				else
				//û�����ã�Ŀ��վ����Ϊ0
				{
					mapStruct->Stop[i] = 0;
					mapStruct->Angle[i] = 0;//��ת�Ƕ�
				}
				break;
			case 8://����-��ֲ�
				if(bit==0){
					mapStruct->Stop[i] = buffer[9];//Ŀ��վ��
					mapStruct->Dir[i] = Enum_HouTui;//����
					mapStruct->Action[i] = Enum_ZuoFenCha;
					mapStruct->Angle[i] = 0;//��ת�Ƕ�
				}
				else
					//û�����ã�Ŀ��վ����Ϊ0
					mapStruct->Stop[i] = 0;
				break;
			case 9://����-ֱ��
				if(bit==0){
					mapStruct->Stop[i] = buffer[10];//Ŀ��վ��
					mapStruct->Dir[i] = Enum_HouTui;//����
					mapStruct->Action[i] = Enum_ZhiXing;
					mapStruct->Angle[i] = 0;//��ת�Ƕ�
				}
				else
				//û�����ã�Ŀ��վ����Ϊ0
				{
					mapStruct->Stop[i] = 0;
					mapStruct->Angle[i] = 0;//��ת�Ƕ�
				}
				break;
			case 10://����-�ҷֲ�
				if(bit==0){
					mapStruct->Stop[i] = buffer[11];//Ŀ��վ��
					mapStruct->Dir[i] = Enum_HouTui;//����
					mapStruct->Action[i] = Enum_YouFenCha;
					mapStruct->Angle[i] = 0;//��ת�Ƕ�
				}
				else
					//û�����ã�Ŀ��վ����Ϊ0
					mapStruct->Stop[i] = 0;
				break;
			case 11://����-����
				if(bit==0){
					mapStruct->Stop[i] = buffer[12];//Ŀ��վ��
					mapStruct->Dir[i] = Enum_HouTui;//����
					mapStruct->Action[i] = Enum_YouXuan;
					mapStruct->Angle[i] = buffer[16];//��ת�Ƕ�
				}
				else
				//û�����ã�Ŀ��վ����Ϊ0
				{
					mapStruct->Stop[i] = 0;
					mapStruct->Angle[i] = 0;//��ת�Ƕ�
				}
				break;
			default:break;
		}
	}
}
/*******************************************************
* ���ƣ�����ͼ����ͼ�ṹ��
* num:վ���
* mapStruct:վ���ͼ�ṹ��ָ��
*******************************************************/
void ReadToMapStruct(u16 num,StaionMapStruct *mapStruct)
{
	//���建����
	u16 Buf[StationMapSpace]={0};
	//1.����ͼ��������
	ReadMapToBuffer(num,Buf);
	//2.����������ͼ�ṹ��
	BufferToMapStruct(num,Buf,mapStruct);
}
/*******************************************************
* ���ƣ�վ���ͼ�ṹ��תͨ�ŽӿڼĴ���
* map:վ���ͼ�ṹ��ָ��
* reg:ͨ�ŽӿڼĴ���ָ��
*******************************************************/
void MapSturctToReg(StaionMapStruct *map,u16 *reg){
	//�����ת�Ƕ�
	reg[13] = reg[14] = reg[15] = reg[16] = 0;
	//ȡĿ��λ�ú���ת�Ƕȵ��ӿڼĴ���
	for(u8 i=0;i<StationMapType;i++)
	{
		reg[i+1] = map->Stop[i];
			if(map->Dir[i]==Enum_QianJin && map->Action[i]==Enum_ZuoXuan)
				reg[13] = map->Angle[i];
			else
			if(map->Dir[i]==Enum_QianJin && map->Action[i]==Enum_YouXuan)
				reg[14] = map->Angle[i];
			else
			if(map->Dir[i]==Enum_HouTui && map->Action[i]==Enum_ZuoXuan)
				reg[15] = map->Angle[i];
			else
			if(map->Dir[i]==Enum_HouTui && map->Action[i]==Enum_YouXuan)
				reg[16] = map->Angle[i];
			else
				continue;
	}
}
/*******************************************************
* ���ƣ�����ͼ���ӿڼĴ���
* num:վ���
* reg:�ӿڼĴ���ָ��
*******************************************************/
void ReadMapToReg(u16 num){
	//���建����
	u16 Buf[StationMapSpace]={0};
	//�����ͼ�ṹ��
	StaionMapStruct mapStruct;
	//1.����ͼ��������
	ReadMapToBuffer(num,Buf);
	//2.����������ͼ�ṹ��
	BufferToMapStruct(num,Buf,&mapStruct);
	//3.��ͼ�ṹ�嵽�ӿڼĴ���
	MapSturctToReg(&mapStruct,ApiRegister);
}

/*******************************************************
* ���ƣ��ӿڴ���ת��ͼ��������
* reg:�ӿڼĴ���ָ��
* buf:��ͼ������ָ��
*******************************************************/
void MapRegToBuf(u16* reg,u16* buf){
	//��ǳ�ʼ״̬Ϊ1
	buf[0] = 0xffff;
	//ȡĿ��λ�ú���ת�Ƕȵ��ӿڼĴ���
	for(u8 i=0;i<StationMapType;i++)
	{
		//��Ŀ��λ��д�뻺����
		buf[i+1] = reg[i+1];
		//����Ŀ���Ƿ�Ϊ0,���buf[0],λ��������λ��0Ϊ��Ч
		if(reg[i+1]==0) 
			igk_SetBit(buf[0],i);
		else
			igk_ClearBit(buf[0],i);
		//ȡ�Ƕ�
		if((i+1)==1)
			buf[13] = reg[13];
		else
		if((i+1)==5)
			buf[14] = reg[14];
		else
		if((i+1)==8)
			buf[15] = reg[15];
		else
		if((i+1)==12)
			buf[16] = reg[16];
		else
			continue;
	}
}
/*******************************************************
* ���ƣ�д���ͼ�ӻ�����
* num:վ���
* buffer:��ͼ������
*******************************************************/
void WriteMap(u16 num){
	//���建����
	u16 Buf[StationMapSpace]={0};
	MapRegToBuf(ApiRegister,Buf);
	W25QXX_Write_16(Buf,StationStartAddress(num),StationMapSpace);
}
/*******************************************************
* ���ƣ����ݽӿڼĴ���״̬,��д��ͼ
*******************************************************/
void ReadWriteMap(void* p_arg)
{
	//��ǰλ��
	u16 nowNum = ApiRegister[30];
	//����д��
	if(ApiRegister[31] == 1){
		WriteMap(nowNum);
		ApiRegister[31] = 0;
		IGK_SysTimePrintln("д��%d�ű�ǩ��ͼ!",nowNum);
	}
	//�����ȡ
	if(ApiRegister[32] == 1){
		ReadMapToReg(nowNum);
		ApiRegister[32] = 0;
		IGK_SysTimePrintln("��ȡ%d�ű�ǩ��ͼ!",nowNum);
	}
}
