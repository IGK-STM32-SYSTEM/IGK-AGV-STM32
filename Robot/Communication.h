#ifndef  _COMMUNICATION_H_
#define  _COMMUNICATION_H_
#include "steppercontrol.h"

//��������ָ��
void AnalysisRevice(void);
u16 CRC16(unsigned char *chData, unsigned short uNo);
//CRC�Զ�����
//chData:Ŀ������
//len:����CRC�ĳ���
//У��ɹ�:1,ʧ��:0
u8 CRC16Check(unsigned char *chData, unsigned short len);














#endif
