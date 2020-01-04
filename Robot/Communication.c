#include "communication.h"
#include "calculate.h"

//32λ������
typedef union
{
	float fv;
	long int lv;
	u8 buf[4]; 
} union32;

//��������ָ��
void AnalysisRevice() {
	union32 u1;
	u8 i=0;
	if(rece2_index>0)
	{
		//������ͷ��CRCУ�飬���ݳ���У��
		if(CRC16Check(rece2_buf,rece2_index)&&rece2_buf[0]==0xAA && rece2_index == (rece2_buf[2] + 5))
		{
			switch (rece2_buf[1]) {
				case 0:
					//��ӡ��Ϣ				
					my_printf(USART3,"ָ��0:��ԭ��!\r\n");
					Axis1HomeFlag = 0;
					Axis2HomeFlag = 0;
					Axis3HomeFlag = 0;
					AxisNowSteps1 = 0;
				  AxisNowSteps2 = 0;
				  AxisNowSteps3 = 0;
					GoHome();
					break;
				case 1:
					//��ӡ��Ϣ				
				my_printf(USART3,"ָ��1:��ĳ�������߹̶�����,����˶�!\r\n");
				RobotRunSteps(rece2_buf[3], rece2_buf[4], rece2_buf[5] << 8 | rece2_buf[6]);
					break;
				case 2:
					//��ӡ��Ϣ				
					my_printf(USART3,"ָ��2:���Ƶ�������ĳ�����������˶�!\r\n");
					RobotRun(rece2_buf[3],rece2_buf[4]);
					break;
				case 3:
					//��ӡ��Ϣ				
					my_printf(USART3,"ָ��3:���Ƶ�����ֹͣ!\r\n");
					RobotStopOne(rece2_buf[3]);
					break;
				case 4:
					//��ӡ��Ϣ				
					my_printf(USART3,"ָ��4:����������ֹͣ!\r\n");
					RobotStopAll();
					break;
				case 5:
					//��ӡ��Ϣ				
					my_printf(USART3,"ָ��5:����ȫ���ٶ�!\r\n");
					//����ȫ���ٶ�
					AxisNowSpeed = (rece2_buf[3]<<8)|rece2_buf[4];
					my_printf(USART3,"��ǰ�ٶ�:%5.3f\r\n",AxisNowSpeed);
					break;
				case 6:
					//��ӡ��Ϣ				
					for(i=0;i<4;i++)
						u1.buf[i] = rece2_buf[4+i];
					my_printf(USART3,"ָ��6:�����˶���ָ���Ƕ�![%5.2f]\r\n",u1.fv);
					RobotToAngle(rece2_buf[3], u1.fv);
					break;
				case 7:
					//��ӡ��Ϣ				
					my_printf(USART3,"ָ��7:X+5\r\n");
				  AxisPoint.x +=5;
					CalculateReverse(AxisPoint);
					break;
				case 8:
					//��ӡ��Ϣ	
					my_printf(USART3,"ָ��8:X-5\r\n");				
				  AxisPoint.x -=5;
					CalculateReverse(AxisPoint);
					break;
				case 9:
					//��ӡ��Ϣ				
					my_printf(USART3,"ָ��9:Y+5\r\n");				
				  AxisPoint.y +=5;
					CalculateReverse(AxisPoint);
					break;
				case 10:
					//��ӡ��Ϣ				
					my_printf(USART3,"ָ��10:Y-5\r\n");				
				  AxisPoint.y -=5;
					CalculateReverse(AxisPoint);
					break;
				case 11:
					//��ӡ��Ϣ				
					my_printf(USART3,"ָ��11:Z+5\r\n");				
				  AxisPoint.z +=5;
					CalculateReverse(AxisPoint);
					break;
				case 12:
					//��ӡ��Ϣ				
					my_printf(USART3,"ָ��12:Z-5\r\n");				
				  AxisPoint.z -=5;
					CalculateReverse(AxisPoint);
					break;
				default:
					my_printf(USART3,"��Чָ�\r\n");
					break;
			}
		}
		else
		{
			my_printf(USART3,"����ָ�\r\n");
		}
		//��ս��ջ�����
		memset(rece2_buf,0,UART_RX_LEN);
		rece2_index = 0;
	}
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
u16 CRC16(unsigned char *chData, unsigned short uNo)
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

//CRC�Զ�����
//chData:Ŀ������
//len:����CRC�ĳ���
//У��ɹ�:1,ʧ��:0
u8 CRC16Check(unsigned char *chData, unsigned short len)
{
	u16 crc = 0;
	crc = CRC16(chData,len-2);
	if(crc==(chData[len-2]<<8|chData[len-1]))
		return 1;
	else
		return 0;
}










