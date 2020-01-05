#include "sys.h"
u8 PLC_InPut[64]={0};//PLC����״̬�Ĵ���1x
u8 PLC_OutPut[128]={0};//PLC���״̬�Ĵ���0x
u16 PLC_Data[128]={0};//PLC���ݼĴ���  4x
u16 reclength = 500;
u16 sendlength;
void AnalyzeRecieve()                              //������������Ӧ����
{
	uint16_t _crc;
	uint8_t a1, a2;
	_crc = CalcCrc(rece3_buf, (rece3_index - 2)); //����У��CRC
	a1 = _crc & 0xff;                      //CRC���ֽ�
	a2 = (_crc >> 8) & 0xff;               //CRC���ֽ�
	if(a1 == rece3_buf[rece3_index - 2] && a2 == rece3_buf[rece3_index - 1]) //У����ȷ
	{
		switch(rece3_buf[1])
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
				errorsend(rece3_buf[1], 0x04);
				break;           //��֧�ֵĹ�����
		}
	}
	else                                              //����У��������
	{
		switch(rece3_buf[1])
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
	Uart3_Start_DMA_Tx(sendlength);
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

	startaddH = rece3_buf[2];
	startaddL = rece3_buf[3];
	bit_numH = rece3_buf[4];
	bit_numL = rece3_buf[5];
	startadd = (startaddH << 8) + startaddL; //Ҫ���ص���ʼ��ַ
	bit_num = (bit_numH << 8) + bit_numL;	 //Ҫ�����ֽ�����,��λ��λ
	if((startadd + bit_num) >= reclength)            //�������32��λ���ӵ�4��λ��ʼ��
	{
		errorsend(0x01, 0x02);    //��Ӧ�Ĵ�������������Χ
	}
	else
	{

		send3_buf[0] = 0x01;          //վ��
		send3_buf[1] = 0x01;         //������
		if((bit_num % 8) == 0)
		{ send3_buf[2] = (bit_num) / 8; }  //Ҫ���ص��ֽ���
		else
		{ send3_buf[2] = ((bit_num) / 8) + 1; } //��������8��ʱ��Ҫ�෵��һ���ֽ�
		for(i = 0; i < send3_buf[2]; i++)
		{
			send3_buf[3 + i] = 0;                   //�����㸴λ
			for(j = 0; j < 8; j++)            //ÿ8��λ״̬���һ���ֽڷ���
			{
				send3_buf[3 + i] = (uint8_t)((PLC_OutPut[startadd + i * 8 + j] & 0x01) << j) + send3_buf[3 + i]; //��λ��ǰ����λ�ں�

			}
		}
		aakj = CalcCrc(send3_buf, (send3_buf[2] + 3));            //CRCУ��
		send3_buf[3 + send3_buf[2]] = (uint8_t)(aakj & 0xff);             //CRC���ֽ�
		send3_buf[4 + send3_buf[2]] = (uint8_t)((aakj >> 8) & 0xff);      //CRC���ֽ�
		sendlength = send3_buf[2] + 5;
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

	startaddH = rece3_buf[2];
	startaddL = rece3_buf[3];
	bit_numH = rece3_buf[4];
	bit_numL = rece3_buf[5];
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


		send3_buf[0] = 0x01;          //վ��
		send3_buf[1] = 0x02;         //������
		if((bit_num % 8) == 0)
		{ send3_buf[2] = (bit_num) / 8; }  //Ҫ���ص��ֽ���
		else
		{ send3_buf[2] = ((bit_num) / 8) + 1; } //��������8��ʱ��Ҫ�෵��һ���ֽ�
		for(i = 0; i < send3_buf[2]; i++)
		{
			send3_buf[3 + i] = 0;                   //�����㸴λ
			for(j = 0; j < 8; j++)            //ÿ8��λ״̬���һ���ֽڷ���
			{
				send3_buf[3 + i] = (uint8_t)((PLC_InPut[startadd + i * 8 + j] & 0x01) << j) + send3_buf[3 + i]; //��λ��ǰ����λ�ں�

			}
		}
		aakj = CalcCrc(send3_buf, (send3_buf[2] + 3));            //CRCУ��
		send3_buf[3 + send3_buf[2]] = (uint8_t)(aakj & 0xff);             //CRC���ֽ�
		send3_buf[4 + send3_buf[2]] = (uint8_t)((aakj >> 8) & 0xff);      //CRC���ֽ�
		sendlength = send3_buf[2] + 5;
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

	startaddH = rece3_buf[2];
	startaddL = rece3_buf[3];
	lengthH = rece3_buf[4];
	lengthL = rece3_buf[5];
	startadd = (startaddH << 8) + startaddL; //Ҫ���ص���ʼ��ַ
	length = (lengthH << 8) + lengthL;	 //Ҫ�����ֽ�����
	if((startadd + length) > reclength)            //���ֻ�ܷ���32���Ĵ���,64���ֽڣ�ע�ⷵ�صĳ��Ȳ��ܳ����������鳤�ȣ������������´���
	{
		errorsend(0x03, 0x02);    //��Ӧ�Ĵ�������������Χ
	}
	else
	{
		send3_buf[0] = 0x01;          //վ��
		send3_buf[1] = 0x03;         //������
		send3_buf[2] = length * 2;  //Ҫ���ص��ֽ����������ĵĵ�����ֽ�*2
		for(i = 0; i < length; i++)
		{

			// 		 // IIC_ReadAll();//������һ�μ��ɣ�����״̬�¶��ڴ�����
			// 		 send3_buf[3+i*2]=I2c_Buf[(c)*2];   //���ؼĴ���ֵ�ĸ��ֽ�
			// 		 send3_buf[4+i*2]=I2c_Buf[(startadd+i)*2+1];        //���ؼĴ���ֵ�õ��ֽ�
			//
			send3_buf[3 + i * 2] = (PLC_Data[startadd + i] >> 8) & 0xff; //���ؼĴ���ֵ�ĸ��ֽ�
			send3_buf[4 + i * 2] = PLC_Data[startadd + i] & 0xff; //���ؼĴ���ֵ�õ��ֽ�
		}
		aakj = CalcCrc(send3_buf, (length * 2) + 3);          //CRCУ��
		send3_buf[3 + length * 2] = (uint8_t)(aakj & 0xff);           //CRC���ֽ�
		send3_buf[4 + length * 2] = (uint8_t)((aakj >> 8) & 0xff);    //CRC���ֽ�
		sendlength = (length * 2) + 5;
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
	startaddH = rece3_buf[2];
	startaddL = rece3_buf[3];
	bit_numH = rece3_buf[4];
	bit_numL = rece3_buf[5];
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


		send3_buf[0] = 0x01;          //վ��
		send3_buf[1] = 0x04;         //������
		send3_buf[2] = bit_num * 2;
		for(i = 0; i < send3_buf[2]; i++)
		{
			send3_buf[3 + i] = 0;                   //�����㸴λ
			for(j = 0; j < 8; j++)            //ÿ8��λ״̬���һ���ֽڷ���
			{
				send3_buf[3 + i] = (uint8_t)((PLC_InPut[startadd + i * 8 + j] & 0x01) << j) + send3_buf[3 + i]; //��λ��ǰ����λ�ں�

			}
		}
		aakj = CalcCrc(send3_buf, (send3_buf[2] + 3));            //CRCУ��
		send3_buf[3 + send3_buf[2]] = (uint8_t)(aakj & 0xff);             //CRC���ֽ�
		send3_buf[4 + send3_buf[2]] = (uint8_t)((aakj >> 8) & 0xff);      //CRC���ֽ�
		sendlength = send3_buf[2] + 5;
	}
}

/*����05�����뱨�ģ�������Ӧ����*/

void Recirve_05()
{

	uint16_t startadd;
	uint8_t startaddH, startaddL;
	uint8_t bit_valueH, bit_valueL;
	uint16_t aakj;
	startaddH = rece3_buf[2];
	startaddL = rece3_buf[3];
	bit_valueH = rece3_buf[4];
	bit_valueL = rece3_buf[5];
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

		send3_buf[0] = 0x01;          //վ��
		send3_buf[1] = 0x05;         //������
		send3_buf[2] = startaddH;   //��ַ���ֽ�
		send3_buf[3] = startaddL;   //��ַ���ֽ�
		send3_buf[4] = bit_valueH;   //��ַ���ֽ�
		send3_buf[5] = bit_valueL;   //��ַ���ֽ�
		aakj = CalcCrc(send3_buf, 6);              //CRCУ��
		send3_buf[6] = (uint8_t)(aakj & 0xff);               //CRC���ֽ�
		send3_buf[7] = (uint8_t)((aakj >> 8) & 0xff);        //CRC���ֽ�
		sendlength = 8;
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
	startaddH = rece3_buf[2];
	startaddL = rece3_buf[3];
	wdataH_06 = rece3_buf[4];
	wdataL_06 = rece3_buf[5];
	startadd = (startaddH << 8) + startaddL; //Ҫд�����ʼ��ַ
	wdata_06 = (wdataH_06 << 8) + wdataL_06;	     //Ҫд�����ֵ

	if(startadd > reclength)                  //�Ĵ�����ַ������Χ
	{ errorsend(0x06, 0x02); }        //��Ӧ�Ĵ�������������Χ
	else if(wdata_06 > 0xFFFF)
	{ errorsend(0x06, 0x03); }        //��Ӧ���ݴ���
	else
	{

		PLC_Data[startadd] = wdata_06;    //����ֵд��Ĵ���
		// 		I2c_Buf[startadd*2]=wdata_06>>8;
		// 		I2c_Buf[startadd*2+1]=wdata_06&0xff;
		//
		// 		//д������flash
		// 		FlashWriteStr(FLASH_ADR,sizeof(I2c_Buf),(u8*)&I2c_Buf);

		send3_buf[0] = 0x01;          //վ��
		send3_buf[1] = 0x06;         //������
		send3_buf[2] = startaddH;    //���ص�ַ���ֽ�
		send3_buf[3] = startaddL;    //���ص�ַ���ֽ�
		send3_buf[4] = (uint8_t)(((PLC_Data[startadd]) >> 8) & 0xff); //���ؼĴ���ֵ���ֽ�
		send3_buf[5] = (uint8_t)(PLC_Data[startadd] & 0xff);     //���ؼĴ���ֵ���ֽ�
		aakj = CalcCrc(send3_buf, 6);              //CRCУ��
		send3_buf[6] = (uint8_t)(aakj & 0xff);               //CRC���ֽ�
		send3_buf[7] = (uint8_t)((aakj >> 8) & 0xff);        //CRC���ֽ�
		sendlength = 8;                                //����8���ֽڳ���
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
	startaddH = rece3_buf[2];
	startaddL = rece3_buf[3];
	register_numH = rece3_buf[4];
	register_numL = rece3_buf[5];
	startadd = (startaddH << 8) + startaddL; //Ҫ���ص���ʼ��ַ
	//   register_num=(register_numH<<8)+register_numL;   //�Ĵ�������
	length = rece3_buf[6];	                             //Ҫд���ֽ�����

	if((startadd + (length / 2)) > reclength)          //�������д32���Ĵ���
	{
		errorsend(0x10, 0x02);    //��Ӧ�Ĵ�������������Χ
	}
	else
	{
		for(i = 0; i < (length / 2); i++) //��ֵд��Ĵ���
		{
			PLC_Data[startadd + i] = (rece3_buf[7 + i * 2] << 8) + rece3_buf[8 + i * 2] & 0xff;
		}
		send3_buf[0] = 0x01;          //վ��
		send3_buf[1] = 0x10;         //������
		send3_buf[2] = startaddH;    //���ص�ַ��λ
		send3_buf[3] = startaddL;    //���ص�ַ��λ
		send3_buf[4] = register_numH;
		send3_buf[5] = register_numL;
		aakj = CalcCrc(send3_buf, 6);              //CRCУ��
		send3_buf[6] = (uint8_t)(aakj & 0xff);               //CRC���ֽ�
		send3_buf[7] = (uint8_t)((aakj >> 8) & 0xff);        //CRC���ֽ�
		sendlength = 8;
	}
}



/*���󷵻�*/

void errorsend(uint8_t func, uint8_t type)
{
	uint16_t _crc;
	uint8_t crcH, crcL;

	send3_buf[0] = 0x01;                    //����վ��
	switch(type)
	{
		case 0x08:
			send3_buf[1] = 0x80 + func;  //���ش�������
			send3_buf[2] = 0x08;         //���ش�����룬08��CRCУ�����
			break;
		case 0x01:
			send3_buf[1] = 0x80 + func;  //���ش�������
			send3_buf[2] = 0x01;         //���ش�����룬01�����������
			break;
		case 0x02:
			send3_buf[1] = 0x80 + func;  //���ش�������
			send3_buf[2] = 0x02;         //���ش�����룬02����ַ����
			break;
		case 0x03:
			send3_buf[1] = 0x80 + func;  //���ش�������
			send3_buf[2] = 0x03;         //���ش�����룬03�����ݴ���
			break;
		case 0x04:
			send3_buf[0] = 0x80 + func;  //���ش�������
			send3_buf[1] = 0x04;         //���ش�����룬04����֧�ֵĹ�����
			break;
	}
	_crc = CalcCrc(send3_buf, 3);
	crcH = (uint8_t)((_crc >> 8) & 0xff);
	crcL = (uint8_t)(_crc & 0xff);
	send3_buf[3] = crcL;                    //У����ֽ�
	send3_buf[4] = crcH;                    //У����ֽ�
	sendlength = 5;
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
