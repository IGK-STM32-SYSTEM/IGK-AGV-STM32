/******************************************
地图存储方式详见地图存储规则
***********************************************/
#ifndef __IGK_MAP_H
#define __IGK_MAP_H
#include "Modbus_slave.h"
#include "igk_system.h"
#include "malloc.h"
#include "igk_dfs.h"

typedef unsigned short int u16;
typedef unsigned char u8;

#define MapStartAddress 0   //地图起始地址
#define StationMax NodeMaxNum  //站点最大值
#define StationMapSpace 29  //站点地图占用空间(单位:16Bit)+增加了12个地址用于存储到达目标位置后的旋转角度
#define StationMapType 12   //站点地图类型(如:左转,左分叉,直行...)
#define TotalSpace StationMax*StationMapSpace  //占用空间总数(单位:16Bit)
#define StationStartAddress(N) MapStartAddress + StationMapSpace*(N)//第N个站点起始地址

//定义指针,指向通信接口寄存器地址
extern u16 *ApiRegister;

//站点地图结构体
typedef struct {
u16 Start[StationMapType];              //起点1-65535
u16 Stop[StationMapType];               //终点1-65535
enum EnumDir  Dir[StationMapType];      //方向
enum EnumAction  Action[StationMapType];//动作
u16 Angle[StationMapType];              //角度【0-360】
int16_t AfterAngle[StationMapType];     //到位后需要旋转的角度【±】【0-360】+：右转，顺时针，-：左转，逆时针
}StaionMapStruct;

/*******************************************************
* 名称：读站点地图到缓冲区
* stationNum:站点号
* buffer:站点地图缓冲区
*******************************************************/
void ReadMapToBuffer(u16 num,u16* buffer);
/*******************************************************
* 名称：站点地图缓冲区转站点地图结构体
* num:站点号
* buffer:站点地图缓冲区
* mapStruct:站点地图结构提指针
*******************************************************/
void BufferToMapStruct(u16 num,u16* buffer,StaionMapStruct *mapStruct);
/*******************************************************
* 名称：读地图到地图结构体
* num:站点号
* mapStruct:站点地图结构提指针
*******************************************************/
void ReadToMapStruct(u16 num,StaionMapStruct *mapStruct);
/*******************************************************
* 名称：站点地图结构体转通信接口寄存器
* map:站点地图结构提指针
* reg:通信接口寄存器指针
*******************************************************/
void MapSturctToReg(StaionMapStruct *map,u16 *reg);
/*******************************************************
* 名称：读地图到接口寄存器
* num:站点号
* reg:接口寄存器指针
*******************************************************/
void ReadMapToReg(u16 num);
/*******************************************************
* 名称：接口存器转地图缓冲数组
* reg:接口寄存器指针
* buf:地图缓冲区指针
*******************************************************/
void MapRegToBuf(u16* reg,u16* buf);
/*******************************************************
* 名称：写入地图从缓冲区
* num:站点号
* buffer:地图缓冲区
*******************************************************/
void WriteMap(u16 num);
/*******************************************************
* 名称：响应地图操作
*******************************************************/
void Igk_Map_Response(void);
/*******************************************************
* 名称：删除地图
*******************************************************/
void DeleteMap(void);

#endif
















