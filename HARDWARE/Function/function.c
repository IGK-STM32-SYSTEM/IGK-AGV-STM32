#include "sys.h"
u8  USART6_TX_BUF[100];
u8 CAN1_Sebuf[5]={1,0,1,0,0x55};


//��16����ת��Ϊ�ַ�
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
//��������ת��Ϊ16������
long FloatTohex(float HEX)
{
	return *( long *)&HEX;//ע��˴���long��ָ��ǿ��ָ�򸡵���HEX�ĵ�ַ
}
//��16����ת��Ϊ������
float HexTofloat(long HEX)
{
	return *( float *)&HEX;
}





