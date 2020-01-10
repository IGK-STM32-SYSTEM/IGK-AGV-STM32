#include "sys.h"
u8  USART6_TX_BUF[100];
u8 CAN1_Sebuf[5]={1,0,1,0,0x55};


//将16进制转换为字符
u8 HexToChar(u8 bChar)
{
	if((bChar>=0x30)&&(bChar<=0x39))
	{
		bChar -= 0x30;
	}
	else if((bChar>=0x41)&&(bChar<=0x46))
	{
		bChar -= 0x37;
	}
	else if((bChar>=0x61)&&(bChar<=0x66))
	{
		bChar -= 0x57;
	}
	else 
	{
		bChar = 0xff;
	}
	return bChar;
}
//将浮点数转换为16进制数
long FloatTohex(float HEX)
{
	return *( long *)&HEX;//注意此处将long型指针强行指向浮点型HEX的地址
}
//将16进制转化为浮点数
float HexTofloat(long HEX)
{
	return *( float *)&HEX;
}





