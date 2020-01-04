/******************************************
��ͼ�洢��ʽ�����ͼ�洢����
***********************************************/
#ifndef __IGK_MAP_H
#define __IGK_MAP_H
#include "Modbus_slave.h"
#include "igk_os.h"

typedef unsigned short int u16;
typedef unsigned char u8;

#define StartAddress 0   //��ͼ��ʼ��ַ
#define StationMax 1000  //վ�����ֵ
#define StationMapSpace 17  //վ���ͼռ�ÿռ�(��λ:16Bit)
#define StationMapType 12   //վ���ͼ����(��:��ת,��ֲ�,ֱ��...)
#define TotalSpace StationMax*StationMapSpace  //ռ�ÿռ�����(��λ:16Bit)
#define StationStartAddress(N) StationMapSpace*(N-1)//��N��վ����ʼ��ַ

//����ָ��,ָ��ͨ�ŽӿڼĴ�����ַ
extern u16 *ApiRegister;

//վ���ͼ�ṹ��
typedef struct {
u16 Start[StationMapType];       //���1-65535
u16 Stop[StationMapType];        //�յ�1-65535
enum EnumDir  Dir[StationMapType];      //����
enum EnumAction  Action[StationMapType];//����
u16 Angle[StationMapType];       //�Ƕ�0-360
}StaionMapStruct;

/*******************************************************
* ���ƣ���վ���ͼ��������
* stationNum:վ���
* buffer:վ���ͼ������
*******************************************************/
void ReadMapToBuffer(u16 num,u16* buffer);
/*******************************************************
* ���ƣ�վ���ͼ������תվ���ͼ�ṹ��
* num:վ���
* buffer:վ���ͼ������
* mapStruct:վ���ͼ�ṹ��ָ��
*******************************************************/
void BufferToMapStruct(u16 num,u16* buffer,StaionMapStruct *mapStruct);
/*******************************************************
* ���ƣ�����ͼ����ͼ�ṹ��
* num:վ���
* mapStruct:վ���ͼ�ṹ��ָ��
*******************************************************/
void ReadToMapStruct(u16 num,StaionMapStruct *mapStruct);
/*******************************************************
* ���ƣ�վ���ͼ�ṹ��תͨ�ŽӿڼĴ���
* map:վ���ͼ�ṹ��ָ��
* reg:ͨ�ŽӿڼĴ���ָ��
*******************************************************/
void MapSturctToReg(StaionMapStruct *map,u16 *reg);
/*******************************************************
* ���ƣ�����ͼ���ӿڼĴ���
* num:վ���
* reg:�ӿڼĴ���ָ��
*******************************************************/
void ReadMapToReg(u16 num);
/*******************************************************
* ���ƣ��ӿڴ���ת��ͼ��������
* reg:�ӿڼĴ���ָ��
* buf:��ͼ������ָ��
*******************************************************/
void MapRegToBuf(u16* reg,u16* buf);
/*******************************************************
* ���ƣ�д���ͼ�ӻ�����
* num:վ���
* buffer:��ͼ������
*******************************************************/
void WriteMap(u16 num);
/*******************************************************
* ���ƣ����ݽӿڼĴ���״̬,��д��ͼ
*******************************************************/
void ReadWriteMap(void* p_arg);
#endif












