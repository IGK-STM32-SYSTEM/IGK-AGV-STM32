#include "igk_map.h"

//定义指针,指向通信接口寄存器地址
u16 *ApiRegister = PLC_Data;

/*******************************************************
* 名称：读站点地图到缓冲区
* num:站点号
* buffer:站点地图缓冲区
*******************************************************/
void ReadMapToBuffer(u16 num,u16* buffer){
	W25QXX_Read_16(buffer,StationStartAddress(num),StationMapSpace);
}
/*******************************************************
* 名称：站点地图缓冲区转站点地图结构体
* num:站点号
* buffer:站点地图缓冲区
* mapStruct:站点地图结构提指针
*******************************************************/
void BufferToMapStruct(u16 num,u16* buffer,StaionMapStruct *mapStruct){
	//循环读取地图信息,并解析到地图结构体
	for(u8 i=0;i<StationMapType;i++)
	{
		//当前站点
		mapStruct->Start[i] = num;
		//按位取数据
		u8 bit = igk_GetBit(buffer[0],i);
		switch(i)
		{
			case 0://前进-左旋
				if(bit==0){
					mapStruct->Stop[i] = buffer[1];//目标站点
					mapStruct->Dir[i] = Enum_QianJin;//方向
					mapStruct->Action[i] = Enum_ZuoXuan;
					mapStruct->Angle[i] = buffer[13];//旋转角度
                    mapStruct->AfterAngle[i] = (int16_t)buffer[17];//到达后旋转角度
				}
				else
				//没有设置，目标站点置为0
				{
					mapStruct->Stop[i] = 0;
					mapStruct->Angle[i] = 0;//旋转角度
                    mapStruct->AfterAngle[i] = 0 ;//旋转角度
				}
				break;
			case 1://前进-左分叉
				if(bit==0){
					mapStruct->Stop[i] = buffer[2];//目标站点
					mapStruct->Dir[i] = Enum_QianJin;//方向
					mapStruct->Action[i] = Enum_ZuoFenCha;
					mapStruct->Angle[i] = 0;//旋转角度
                    mapStruct->AfterAngle[i] = (int16_t)buffer[18];//到达后旋转角度
				}
				else
                {
                    //没有设置，目标站点置为0
					mapStruct->Stop[i] = 0;
                    mapStruct->AfterAngle[i] = 0 ;//旋转角度
                }
				break;
			case 2://前进-直行
				if(bit==0){
					mapStruct->Stop[i] = buffer[3];//目标站点
					mapStruct->Dir[i] = Enum_QianJin;//方向
					mapStruct->Action[i] = Enum_ZhiXing;
					mapStruct->Angle[i] = 0;//旋转角度
                    mapStruct->AfterAngle[i] = (int16_t)buffer[19];//到达后旋转角度
				}
				else
                {
                    //没有设置，目标站点置为0
					mapStruct->Stop[i] = 0;
                    mapStruct->AfterAngle[i] = 0 ;//旋转角度
                }
				break;
			case 3://前进-右分叉
				if(bit==0){
					mapStruct->Stop[i] = buffer[4];//目标站点
					mapStruct->Dir[i] = Enum_QianJin;//方向
					mapStruct->Action[i] = Enum_YouFenCha;
					mapStruct->Angle[i] = 0;//旋转角度
                    mapStruct->AfterAngle[i] = (int16_t)buffer[20];//到达后旋转角度
				}
				else
                {
                    //没有设置，目标站点置为0
					mapStruct->Stop[i] = 0;
                    mapStruct->AfterAngle[i] = 0 ;//旋转角度
                }
				break;
			case 4://前进-右旋
				if(bit==0){
					mapStruct->Stop[i] = buffer[5];//目标站点
					mapStruct->Dir[i] = Enum_QianJin;//方向
					mapStruct->Action[i] = Enum_YouXuan;
					mapStruct->Angle[i] = buffer[14];//旋转角度
                    mapStruct->AfterAngle[i] = (int16_t)buffer[21];//到达后旋转角度
				}
				else
				//没有设置，目标站点置为0
				{
					mapStruct->Stop[i] = 0;
					mapStruct->Angle[i] = 0;//旋转角度
                    mapStruct->AfterAngle[i] = 0 ;//旋转角度
				}
				break;
			case 5://平移-左移
				if(bit==0){
					mapStruct->Stop[i] = buffer[6];//目标站点
					mapStruct->Dir[i] = Enum_PingYi;//方向
					mapStruct->Action[i] = Enum_ZuoYi;
					mapStruct->Angle[i] = 0;//旋转角度
                    mapStruct->AfterAngle[i] = (int16_t)buffer[22];//到达后旋转角度
				}
				else
                {
					//没有设置，目标站点置为0
					mapStruct->Stop[i] = 0;
                    mapStruct->AfterAngle[i] = 0 ;//旋转角度
                }
				break;
			case 6://平移-右移
				if(bit==0){
					mapStruct->Stop[i] = buffer[7];//目标站点
					mapStruct->Dir[i] = Enum_PingYi;//方向
					mapStruct->Action[i] = Enum_YouYi;
					mapStruct->Angle[i] = 0;//旋转角度
                    mapStruct->AfterAngle[i] = (int16_t)buffer[23];//到达后旋转角度
				}
				else
                {
                    //没有设置，目标站点置为0
					mapStruct->Stop[i] = 0;
                    mapStruct->AfterAngle[i] = 0 ;//旋转角度
                }
				break;
			case 7://后退-左旋
				if(bit==0){
					mapStruct->Stop[i] = buffer[8];//目标站点
					mapStruct->Dir[i] = Enum_HouTui;//方向
					mapStruct->Action[i] = Enum_ZuoXuan;
					mapStruct->Angle[i] = buffer[15];//旋转角度
                    mapStruct->AfterAngle[i] = (int16_t)buffer[24];//到达后旋转角度
				}
				else
				//没有设置，目标站点置为0
				{
					mapStruct->Stop[i] = 0;
					mapStruct->Angle[i] = 0;//旋转角度
                    mapStruct->AfterAngle[i] = 0 ;//旋转角度
				}
				break;
			case 8://后退-左分叉
				if(bit==0){
					mapStruct->Stop[i] = buffer[9];//目标站点
					mapStruct->Dir[i] = Enum_HouTui;//方向
					mapStruct->Action[i] = Enum_ZuoFenCha;
					mapStruct->Angle[i] = 0;//旋转角度
                    mapStruct->AfterAngle[i] = (int16_t)buffer[25];//到达后旋转角度
				}
				else
                {
                    //没有设置，目标站点置为0
					mapStruct->Stop[i] = 0;
                    mapStruct->AfterAngle[i] = 0 ;//旋转角度
                }
				break;
			case 9://后退-直行
				if(bit==0){
					mapStruct->Stop[i] = buffer[10];//目标站点
					mapStruct->Dir[i] = Enum_HouTui;//方向
					mapStruct->Action[i] = Enum_ZhiXing;
					mapStruct->Angle[i] = 0;//旋转角度
                    mapStruct->AfterAngle[i] = (int16_t)buffer[26];//到达后旋转角度
				}
				else
				//没有设置，目标站点置为0
				{
					mapStruct->Stop[i] = 0;
					mapStruct->Angle[i] = 0;//旋转角度
                    mapStruct->AfterAngle[i] = 0 ;//旋转角度
				}
				break;
			case 10://后退-右分叉
				if(bit==0){
					mapStruct->Stop[i] = buffer[11];//目标站点
					mapStruct->Dir[i] = Enum_HouTui;//方向
					mapStruct->Action[i] = Enum_YouFenCha;
					mapStruct->Angle[i] = 0;//旋转角度
                    mapStruct->AfterAngle[i] = (int16_t)buffer[27];//到达后旋转角度
				}
				else
                {
                    //没有设置，目标站点置为0
					mapStruct->Stop[i] = 0;
                    mapStruct->AfterAngle[i] = 0 ;//旋转角度
                }
				break;
			case 11://后退-右旋
				if(bit==0){
					mapStruct->Stop[i] = buffer[12];//目标站点
					mapStruct->Dir[i] = Enum_HouTui;//方向
					mapStruct->Action[i] = Enum_YouXuan;
					mapStruct->Angle[i] = buffer[16];//旋转角度
                    mapStruct->AfterAngle[i] = (int16_t)buffer[28];//到达后旋转角度
				}
				else
				//没有设置，目标站点置为0
				{
					mapStruct->Stop[i] = 0;
					mapStruct->Angle[i] = 0;//旋转角度
                    mapStruct->AfterAngle[i] = 0 ;//旋转角度
				}
				break;
			default:break;
		}
	}
}
/*******************************************************
* 名称：读地图到地图结构体
* num:站点号
* mapStruct:站点地图结构提指针
*******************************************************/
void ReadToMapStruct(u16 num,StaionMapStruct *mapStruct)
{
	//定义缓冲区
	u16 Buf[StationMapSpace]={0};
	//1.读地图到缓冲区
	ReadMapToBuffer(num,Buf);
	//2.缓冲区到地图结构体
	BufferToMapStruct(num,Buf,mapStruct);
}
/*******************************************************
* 名称：站点地图结构体转通信接口寄存器
* map:站点地图结构提指针
* reg:通信接口寄存器指针
*******************************************************/
void MapSturctToReg(StaionMapStruct *map,u16 *reg){
	//清除旋转角度
	reg[13] = reg[14] = reg[15] = reg[16] = 0;
	//取目标位置和旋转角度到接口寄存器
	for(u8 i=0;i<StationMapType;i++)
	{
		reg[i+1] = map->Stop[i];
        reg[17+i] = (u16)(map->AfterAngle[i]);
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
* 名称：读地图到接口寄存器
* num:站点号
* reg:接口寄存器指针
*******************************************************/
void ReadMapToReg(u16 num){
	//定义缓冲区
	u16 Buf[StationMapSpace]={0};
	//定义地图结构体
	StaionMapStruct mapStruct;
	//1.读地图到缓冲区
	ReadMapToBuffer(num,Buf);
	//2.缓冲区到地图结构体
	BufferToMapStruct(num,Buf,&mapStruct);
	//3.地图结构体到接口寄存器
	MapSturctToReg(&mapStruct,ApiRegister);
}

/*******************************************************
* 名称：接口存器转地图缓冲数组
* reg:接口寄存器指针
* buf:地图缓冲区指针
*******************************************************/
void MapRegToBuf(u16* reg,u16* buf){
	//标记初始状态为1
	buf[0] = 0xffff;
	//取目标位置和旋转角度到接口寄存器
	for(u8 i=0;i<StationMapType;i++)
	{
		//将目标位置写入缓冲区
		buf[i+1] = reg[i+1];
		//根据目标是否为0,标记buf[0],位操作，且位的0为有效
		if(reg[i+1]==0) 
			igk_SetBit(buf[0],i);
		else
			igk_ClearBit(buf[0],i);
        //取到位后的旋转角度
        buf[17+i] = reg[17+i];
		//取角度
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
* 名称：写入地图从缓冲区
* num:站点号
* buffer:地图缓冲区
*******************************************************/
void WriteMap(u16 num){
	//定义缓冲区
	u16 Buf[StationMapSpace]={0};
	MapRegToBuf(ApiRegister,Buf);
	W25QXX_Write_16(Buf,StationStartAddress(num),StationMapSpace);
}
/*******************************************************
* 名称：响应地图操作
*******************************************************/
void Igk_Map_Response()
{
	//当前位置
	u16 nowNum = ApiRegister[30];
	//请求写入
	if(ApiRegister[31] == 1){
		WriteMap(nowNum);
		ApiRegister[31] = 0;
		IGK_SysTimePrintln("写入%d号标签地图!",nowNum);
	}
	//请求读取
	if(ApiRegister[32] == 1){
		ReadMapToReg(nowNum);
		ApiRegister[32] = 0;
		IGK_SysTimePrintln("读取%d号标签地图!",nowNum);
	}
	//删除地图
	if(ApiRegister[36] == 1){
		DeleteMap();
		ApiRegister[36] = 0;
		IGK_SysTimePrintln("执行删除地图完成!");
	}
}

/*******************************************************
* 名称：删除地图
*******************************************************/
void DeleteMap()
{
	u16 OneNum = 60;//单次清除站点数
	//定义指针，并且申请一个具有100个站点地图信息的空间 29*60*2=3480(字节)
    u16* p = (u16*)mymalloc(SRAMIN,sizeof(u16)*StationMapSpace*OneNum); 
	u8 percent = my_mem_perused(SRAMIN);
	//置默认数据
	for(int i=0;i<100;i++)
	{
		p[i*StationMapSpace] = 0xFFFF;
		for(int j=1;j<StationMapSpace;j++)
		{
			p[i*StationMapSpace+j] = 0; 
		}
	}
	//需要删除次数
	u16 count = StationMax / OneNum;
	//余数
	u16 yu = StationMax % OneNum;
	//写入整数部分
	for(int i=0;i<count;i++)
	{
		W25QXX_Write_16(p,StationStartAddress(i*OneNum),StationMapSpace*OneNum);
		//适当延时，避免看门狗触发
		osdelay_ms(10);
	}
	//写入余数部分
	if(yu!=0)
	{
		W25QXX_Write_16(p,StationStartAddress(count*OneNum),StationMapSpace*yu);
	}
  
	//释放内存
	myfree(SRAMIN,p);  
	//防止产生野指针  
	p = NULL; 
	//语音提示  
	IGK_Speek("地图已清除！");
}






