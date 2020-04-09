/******************************************
��ͼ�洢��ʽ�����ͼ�洢����
***********************************************/
#ifndef __IGK_MAP_H
#define __IGK_MAP_H
#include "Modbus_slave.h"
#include "igk_system.h"
#include "malloc.h"
#include "igk_dfs.h"

typedef unsigned short int u16;
typedef unsigned char u8;

#define MapStartAddress 0   //��ͼ��ʼ��ַ
#define StationMax NodeMaxNum  //վ�����ֵ
#define StationMapSpace 29  //վ���ͼռ�ÿռ�(��λ:16Bit)+������12����ַ���ڴ洢����Ŀ��λ�ú����ת�Ƕ�
#define StationMapType 12   //վ���ͼ����(��:��ת,��ֲ�,ֱ��...)
#define TotalSpace StationMax*StationMapSpace  //ռ�ÿռ�����(��λ:16Bit)
#define StationStartAddress(N) MapStartAddress + StationMapSpace*(N)//��N��վ����ʼ��ַ

//����ָ��,ָ��ͨ�ŽӿڼĴ�����ַ
extern u16 *ApiRegister;

//վ���ͼ�ṹ��
typedef struct {
u16 Start[StationMapType];              //���1-65535
u16 Stop[StationMapType];               //�յ�1-65535
enum EnumDir  Dir[StationMapType];      //����
enum EnumAction  Action[StationMapType];//����
u16 Angle[StationMapType];              //�Ƕȡ�0-360��
int16_t AfterAngle[StationMapType];     //��λ����Ҫ��ת�ĽǶȡ�������0-360��+����ת��˳ʱ�룬-����ת����ʱ��
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
* ���ƣ���Ӧ��ͼ����
*******************************************************/
void Igk_Map_Response(void);
/*******************************************************
* ���ƣ�ɾ����ͼ
*******************************************************/
void DeleteMap(void);

#endif
















