#include "sys.h"
u8 PLC_InPut[128]={0};//PLC����״̬�Ĵ���1x
u8 PLC_OutPut[128]={0};//PLC���״̬�Ĵ���0x
u16 PLC_Data[HoldingRegMax]={0};//PLC���ݼĴ���  4x
u16 reclength = 128;
u16 ModbusSendLength=0;
//�����������ָ��
u8 *ModbusReceBuf = rece3_buf;
//���巢������ָ��
u8 *ModbusSendBuf = send3_buf;
//������ճ���ָ��
u16 *ModbusReceLength = &rece3_index;

void AnalyzeRecieve()                              //������������Ӧ����
{
	uint16_t _crc;
	uint8_t a1, a2;
	_crc = CalcCrc(ModbusReceBuf, (*ModbusReceLength - 2)); //����У��CRC
	a1 = _crc & 0xff;                      //CRC���ֽ�
	a2 = (_crc >> 8) & 0xff;               //CRC���ֽ�
	if(a1 == ModbusReceBuf[*ModbusReceLength - 2] && a2 == ModbusReceBuf[*ModbusReceLength - 1]) //У����ȷ
	{
		switch(ModbusReceBuf[1])
		{
			case 0x01:
				Recirve_01();
				break;                    //��λ״̬
			case 0x02:
				Recirve_02();
				break;                    //������λ״̬	ok
			case 0x03:
				Recirve_03();
				break;	                   //���Ĵ���
			case 0x04:
				Recirve_04();
				break;                    //������Ĵ���
			case 0x05:
				Recirve_05();
				break;                    //д����λ״̬ok
			case 0x06:
				Recirve_06();
				break;                    //д�����Ĵ���
			case 0x10:
				Recirve_10();
				break;                    //д����Ĵ���
			default:
				errorsend(ModbusReceBuf[1], 0x04);
				break;           //��֧�ֵĹ�����
		}
	}
	else                                              //����У��������
	{
		switch(ModbusReceBuf[1])
		{
			case 0x01:
				errorsend(0x01, 0x08);
				break;
			case 0x02:
				errorsend(0x02, 0x08);
				break;
			case 0x03:
				errorsend(0x03, 0x08);
				break;
			case 0x04:
				errorsend(0x04, 0x08);
				break;
			case 0x05:
				errorsend(0x05, 0x08);
				break;
			case 0x06:
				errorsend(0x06, 0x08);
				break;
			case 0x10:
				errorsend(0x10, 0x08);
				break;
		}
	}
	send_comm();
}

/*�ӻ���Ӧ����*/

void  send_comm( void )
{
	Uart3_Start_DMA_Tx(ModbusSendLength);
}

/*����01�����뱨�ģ�������Ӧ����*/
void Recirve_01()
{

	uint16_t startadd;
	uint16_t bit_num;
	uint8_t startaddH, startaddL;
	uint8_t bit_numH, bit_numL;
	uint16_t i, j;
	uint16_t aakj;

	startaddH = ModbusReceBuf[2];
	startaddL = ModbusReceBuf[3];
	bit_numH = ModbusReceBuf[4];
	bit_numL = ModbusReceBuf[5];
	startadd = (startaddH << 8) + startaddL; //Ҫ���ص���ʼ��ַ
	bit_num = (bit_numH << 8) + bit_numL;	 //Ҫ�����ֽ�����,��λ��λ
	if((startadd + bit_num) >= reclength)            //�������32��λ���ӵ�4��λ��ʼ��
	{
		errorsend(0x01, 0x02);    //��Ӧ�Ĵ�������������Χ
	}
	else
	{

		ModbusSendBuf[0] = 0x01;          //վ��
		ModbusSendBuf[1] = 0x01;         //������
		if((bit_num % 8) == 0)
		{ ModbusSendBuf[2] = (bit_num) / 8; }  //Ҫ���ص��ֽ���
		else
		{ ModbusSendBuf[2] = ((bit_num) / 8) + 1; } //��������8��ʱ��Ҫ�෵��һ���ֽ�
		for(i = 0; i < ModbusSendBuf[2]; i++)
		{
			ModbusSendBuf[3 + i] = 0;                   //�����㸴λ
			for(j = 0; j < 8; j++)            //ÿ8��λ״̬���һ���ֽڷ���
			{
				ModbusSendBuf[3 + i] = (uint8_t)((PLC_OutPut[startadd + i * 8 + j] & 0x01) << j) + ModbusSendBuf[3 + i]; //��λ��ǰ����λ�ں�

			}
		}
		aakj = CalcCrc(ModbusSendBuf, (ModbusSendBuf[2] + 3));            //CRCУ��
		ModbusSendBuf[3 + ModbusSendBuf[2]] = (uint8_t)(aakj & 0xff);             //CRC���ֽ�
		ModbusSendBuf[4 + ModbusSendBuf[2]] = (uint8_t)((aakj >> 8) & 0xff);      //CRC���ֽ�
		ModbusSendLength = ModbusSendBuf[2] + 5;
	}
}

/*����02�����뱨�ģ�������Ӧ����*/

void Recirve_02()// �㶨****************************************************
{

	uint16_t startadd;
	uint16_t bit_num;
	uint8_t startaddH, startaddL;
	uint8_t bit_numH, bit_numL;
	uint16_t i, j;
	uint16_t aakj;

	uint16_t P3 = 0xffff;

	for(i = 0; i < 32; i++)
	{
		if(PLC_InPut[i])
		{ P3 = P3 - (0x01 << (i)); }
	}
	// 	 if(!OK_Key)P3 =0xffff-(0x01<<(10-1));
	// 	 else
	// 		 P3 =0xffff;

	startaddH = ModbusReceBuf[2];
	startaddL = ModbusReceBuf[3];
	bit_numH = ModbusReceBuf[4];
	bit_numL = ModbusReceBuf[5];
	startadd = (startaddH << 8) + startaddL; //Ҫ���ص���ʼ��ַ
	bit_num = (bit_numH << 8) + bit_numL;	 //Ҫ�����ֽ�����,��λ��λ
	if((startadd + bit_num) >= reclength || startadd > 11) //��������ֻ��4������λ�ɹ���
	{
		errorsend(0x01, 0x02);               //��Ӧ�Ĵ�������������Χ
	}
	else
	{
		for(i = 0; i < 32; i++)
		{ PLC_InPut[i] = (~(P3 >> i)) & 0x01; }      //�ȶ�������ڵ�״̬
		// 	for(i=12;i<32;i++)
		// 	inputBit[i]=0;                              //û��λ״̬������


		ModbusSendBuf[0] = 0x01;          //վ��
		ModbusSendBuf[1] = 0x02;         //������
		if((bit_num % 8) == 0)
		{ ModbusSendBuf[2] = (bit_num) / 8; }  //Ҫ���ص��ֽ���
		else
		{ ModbusSendBuf[2] = ((bit_num) / 8) + 1; } //��������8��ʱ��Ҫ�෵��һ���ֽ�
		for(i = 0; i < ModbusSendBuf[2]; i++)
		{
			ModbusSendBuf[3 + i] = 0;                   //�����㸴λ
			for(j = 0; j < 8; j++)            //ÿ8��λ״̬���һ���ֽڷ���
			{
				ModbusSendBuf[3 + i] = (uint8_t)((PLC_InPut[startadd + i * 8 + j] & 0x01) << j) + ModbusSendBuf[3 + i]; //��λ��ǰ����λ�ں�

			}
		}
		aakj = CalcCrc(ModbusSendBuf, (ModbusSendBuf[2] + 3));            //CRCУ��
		ModbusSendBuf[3 + ModbusSendBuf[2]] = (uint8_t)(aakj & 0xff);             //CRC���ֽ�
		ModbusSendBuf[4 + ModbusSendBuf[2]] = (uint8_t)((aakj >> 8) & 0xff);      //CRC���ֽ�
		ModbusSendLength = ModbusSendBuf[2] + 5;
	}
}


/*����03�����뱨�ģ�������Ӧ����*/
void Recirve_03()
{

	uint16_t startadd;
	uint16_t length;
	uint8_t startaddH, startaddL;
	uint8_t lengthH, lengthL;
	uint16_t i;
	uint16_t aakj;

	startaddH = ModbusReceBuf[2];
	startaddL = ModbusReceBuf[3];
	lengthH = ModbusReceBuf[4];
	lengthL = ModbusReceBuf[5];
	startadd = (startaddH << 8) + startaddL; //Ҫ���ص���ʼ��ַ
	length = (lengthH << 8) + lengthL;	 //Ҫ�����ֽ�����
	if((startadd + length) > HoldingRegMax)            //���ֻ�ܷ���32���Ĵ���,64���ֽڣ�ע�ⷵ�صĳ��Ȳ��ܳ����������鳤�ȣ������������´���
	{
		errorsend(0x03, 0x02);    //��Ӧ�Ĵ�������������Χ
	}
	else
	{
		ModbusSendBuf[0] = 0x01;          //վ��
		ModbusSendBuf[1] = 0x03;         //������
		ModbusSendBuf[2] = length * 2;  //Ҫ���ص��ֽ����������ĵĵ�����ֽ�*2
		for(i = 0; i < length; i++)
		{
			ModbusSendBuf[3 + i * 2] = (PLC_Data[startadd + i] >> 8) & 0xff; //���ؼĴ���ֵ�ĸ��ֽ�
			ModbusSendBuf[4 + i * 2] = PLC_Data[startadd + i] & 0xff; //���ؼĴ���ֵ�õ��ֽ�
		}
		aakj = CalcCrc(ModbusSendBuf, (length * 2) + 3);          //CRCУ��
		ModbusSendBuf[3 + length * 2] = (uint8_t)(aakj & 0xff);           //CRC���ֽ�
		ModbusSendBuf[4 + length * 2] = (uint8_t)((aakj >> 8) & 0xff);    //CRC���ֽ�
		ModbusSendLength = (length * 2) + 5;
	}
}

/*����04�����뱨�ģ�������Ӧ����*/
/*��߷��ص�������ڵļĴ���ֵ*/
void Recirve_04()
{

	uint16_t startadd;
	uint16_t bit_num;
	uint8_t startaddH, startaddL;
	uint8_t bit_numH, bit_numL;
	uint16_t i, j;
	uint16_t aakj;
	startaddH = ModbusReceBuf[2];
	startaddL = ModbusReceBuf[3];
	bit_numH = ModbusReceBuf[4];
	bit_numL = ModbusReceBuf[5];
	startadd = (startaddH << 8) + startaddL; //Ҫ���ص���ʼ��ַ
	bit_num = (bit_numH << 8) + bit_numL;	 //Ҫ�����ֽ�����,��λ��λ
	if((startadd + bit_num) >= reclength || startadd > 1) //��������ֻ��4������λ1���Ĵ����ɹ���
	{
		errorsend(0x01, 0x02);    //��Ӧ�Ĵ�������������Χ
	}
	else
	{
		for(i = 2; i < 6; i++)
			//     inputBit[i-2]=(~(P3>>i))&0x01;            //�ȶ�������ڵ�״̬
			for(i = 4; i < 32; i++)
			{ PLC_InPut[i] = 0; }                            //û��λ״̬������


		ModbusSendBuf[0] = 0x01;          //վ��
		ModbusSendBuf[1] = 0x04;         //������
		ModbusSendBuf[2] = bit_num * 2;
		for(i = 0; i < ModbusSendBuf[2]; i++)
		{
			ModbusSendBuf[3 + i] = 0;                   //�����㸴λ
			for(j = 0; j < 8; j++)            //ÿ8��λ״̬���һ���ֽڷ���
			{
				ModbusSendBuf[3 + i] = (uint8_t)((PLC_InPut[startadd + i * 8 + j] & 0x01) << j) + ModbusSendBuf[3 + i]; //��λ��ǰ����λ�ں�

			}
		}
		aakj = CalcCrc(ModbusSendBuf, (ModbusSendBuf[2] + 3));            //CRCУ��
		ModbusSendBuf[3 + ModbusSendBuf[2]] = (uint8_t)(aakj & 0xff);             //CRC���ֽ�
		ModbusSendBuf[4 + ModbusSendBuf[2]] = (uint8_t)((aakj >> 8) & 0xff);      //CRC���ֽ�
		ModbusSendLength = ModbusSendBuf[2] + 5;
	}
}

/*����05�����뱨�ģ�������Ӧ����*/

void Recirve_05()
{

	uint16_t startadd;
	uint8_t startaddH, startaddL;
	uint8_t bit_valueH, bit_valueL;
	uint16_t aakj;
	startaddH = ModbusReceBuf[2];
	startaddL = ModbusReceBuf[3];
	bit_valueH = ModbusReceBuf[4];
	bit_valueL = ModbusReceBuf[5];
	startadd = (startaddH << 8) + startaddL; //Ҫд��ĵ�ַ
	if(startadd >= reclength)
	{
		errorsend(0x01, 0x02);    //��Ӧ�Ĵ�������������Χ
	}
	else
	{

		if(bit_valueH == 0xff && bit_valueL == 0x00) //��λ��Ȧ

		{
			// *(PLC_OutPut+startadd)=1;
			PLC_OutPut[startadd] = 1;
		}
		if(bit_valueH == 0x00 && bit_valueL == 0x00) //��λ��Ȧ
		{
			// *(PLC_OutPut+startadd)=0;
			PLC_OutPut[startadd] = 0;
		}

		ModbusSendBuf[0] = 0x01;          //վ��
		ModbusSendBuf[1] = 0x05;         //������
		ModbusSendBuf[2] = startaddH;   //��ַ���ֽ�
		ModbusSendBuf[3] = startaddL;   //��ַ���ֽ�
		ModbusSendBuf[4] = bit_valueH;   //��ַ���ֽ�
		ModbusSendBuf[5] = bit_valueL;   //��ַ���ֽ�
		aakj = CalcCrc(ModbusSendBuf, 6);              //CRCУ��
		ModbusSendBuf[6] = (uint8_t)(aakj & 0xff);               //CRC���ֽ�
		ModbusSendBuf[7] = (uint8_t)((aakj >> 8) & 0xff);        //CRC���ֽ�
		ModbusSendLength = 8;
	}
}




/*����06�����뱨�ģ�������Ӧ����*/
void Recirve_06()//���ֽ�д��
{

	uint16_t startadd;
	uint16_t wdata_06;
	uint8_t startaddH, startaddL;
	uint8_t wdataH_06, wdataL_06;
	uint16_t aakj;
	startaddH = ModbusReceBuf[2];
	startaddL = ModbusReceBuf[3];
	wdataH_06 = ModbusReceBuf[4];
	wdataL_06 = ModbusReceBuf[5];
	startadd = (startaddH << 8) + startaddL; //Ҫд�����ʼ��ַ
	wdata_06 = (wdataH_06 << 8) + wdataL_06;	     //Ҫд�����ֵ

	if(startadd > HoldingRegMax)                  //�Ĵ�����ַ������Χ
	{ errorsend(0x06, 0x02); }        //��Ӧ�Ĵ�������������Χ
	else if(wdata_06 > 0xFFFF)
	{ errorsend(0x06, 0x03); }        //��Ӧ���ݴ���
	else
	{

		PLC_Data[startadd] = wdata_06;    //����ֵд��Ĵ���
		ModbusSendBuf[0] = 0x01;          //վ��
		ModbusSendBuf[1] = 0x06;         //������
		ModbusSendBuf[2] = startaddH;    //���ص�ַ���ֽ�
		ModbusSendBuf[3] = startaddL;    //���ص�ַ���ֽ�
		ModbusSendBuf[4] = (uint8_t)(((PLC_Data[startadd]) >> 8) & 0xff); //���ؼĴ���ֵ���ֽ�
		ModbusSendBuf[5] = (uint8_t)(PLC_Data[startadd] & 0xff);     //���ؼĴ���ֵ���ֽ�
		aakj = CalcCrc(ModbusSendBuf, 6);              //CRCУ��
		ModbusSendBuf[6] = (uint8_t)(aakj & 0xff);               //CRC���ֽ�
		ModbusSendBuf[7] = (uint8_t)((aakj >> 8) & 0xff);        //CRC���ֽ�
		ModbusSendLength = 8;                                //����8���ֽڳ���
	}
}

/*����10�����뱨�ģ�������Ӧ����*/
void Recirve_10()//���յ�����
{

	uint16_t startadd;
	//   uint16_t register_num;
	uint8_t startaddH, startaddL;
	uint8_t register_numH, register_numL;
	uint8_t length;
	uint16_t i;
	uint16_t aakj;
	startaddH = ModbusReceBuf[2];
	startaddL = ModbusReceBuf[3];
	register_numH = ModbusReceBuf[4];
	register_numL = ModbusReceBuf[5];
	startadd = (startaddH << 8) + startaddL; //Ҫ���ص���ʼ��ַ
	length = ModbusReceBuf[6];	                             //Ҫд���ֽ�����

	if((startadd + (length / 2)) > HoldingRegMax)
	{
		errorsend(0x10, 0x02);    //��Ӧ�Ĵ�������������Χ
	}
	else
	{
		for(i = 0; i < (length / 2); i++) //��ֵд��Ĵ���
		{
			PLC_Data[startadd + i] = (ModbusReceBuf[7 + i * 2] << 8) + ModbusReceBuf[8 + i * 2] & 0xff;
		}
		ModbusSendBuf[0] = 0x01;          //վ��
		ModbusSendBuf[1] = 0x10;         //������
		ModbusSendBuf[2] = startaddH;    //���ص�ַ��λ
		ModbusSendBuf[3] = startaddL;    //���ص�ַ��λ
		ModbusSendBuf[4] = register_numH;
		ModbusSendBuf[5] = register_numL;
		aakj = CalcCrc(ModbusSendBuf, 6);              //CRCУ��
		ModbusSendBuf[6] = (uint8_t)(aakj & 0xff);               //CRC���ֽ�
		ModbusSendBuf[7] = (uint8_t)((aakj >> 8) & 0xff);        //CRC���ֽ�
		ModbusSendLength = 8;
	}
}



/*���󷵻�*/

void errorsend(uint8_t func, uint8_t type)
{
	uint16_t _crc;
	uint8_t crcH, crcL;

	ModbusSendBuf[0] = 0x01;                    //����վ��
	switch(type)
	{
		case 0x08:
			ModbusSendBuf[1] = 0x80 + func;  //���ش�������
			ModbusSendBuf[2] = 0x08;         //���ش�����룬08��CRCУ�����
			break;
		case 0x01:
			ModbusSendBuf[1] = 0x80 + func;  //���ش�������
			ModbusSendBuf[2] = 0x01;         //���ش�����룬01�����������
			break;
		case 0x02:
			ModbusSendBuf[1] = 0x80 + func;  //���ش�������
			ModbusSendBuf[2] = 0x02;         //���ش�����룬02����ַ����
			break;
		case 0x03:
			ModbusSendBuf[1] = 0x80 + func;  //���ش�������
			ModbusSendBuf[2] = 0x03;         //���ش�����룬03�����ݴ���
			break;
		case 0x04:
			ModbusSendBuf[0] = 0x80 + func;  //���ش�������
			ModbusSendBuf[1] = 0x04;         //���ش�����룬04����֧�ֵĹ�����
			break;
	}
	_crc = CalcCrc(ModbusSendBuf, 3);
	crcH = (uint8_t)((_crc >> 8) & 0xff);
	crcL = (uint8_t)(_crc & 0xff);
	ModbusSendBuf[3] = crcL;                    //У����ֽ�
	ModbusSendBuf[4] = crcH;                    //У����ֽ�
	ModbusSendLength = 5;
}


/*************************************************
crc16У������㺯��,�����㷨��
1������crcУ��Ĵ����ĳ�ֵΪ0xffff;
2�������ĵĵ�һ���ֽ���crc�Ĵ����ĵ��ֽ���򣬽������crc�Ĵ���
3���ж�crc�ĵ�һλ�Ƿ�Ϊ1�������1,crc����1λ���0xa001������Ϊ0��������1λ��
4���ظ�����3��ֱ�����8��λ��
5���ظ�����2��3��4ֱ����������ֽ�
6�����ؼ�����
***********************************************/
uint16_t CalcCrc(unsigned char *chData, unsigned short uNo)
{
	uint16_t crc = 0xffff;
	uint16_t i, j;
	for(i = 0; i < uNo; i++)
	{
		crc ^= chData[i];
		for(j = 0; j < 8; j++)
		{
			if(crc & 1)
			{
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
			{ crc >>= 1; }
		}
	}
	return (crc);
}
