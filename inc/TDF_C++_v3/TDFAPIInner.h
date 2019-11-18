#ifndef __TDF_API_INNER_H__
#define __TDF_API_INNER_H__
#pragma pack(push)
#pragma pack(1)

#include "TDFAPIStruct.h"
#include "TDFDataDefine.h"

//��������,���ø����ͺ󣬻ص�����ԭʼ�ṹ��
enum TDF_ENVIRON_SETTING_INNER
{
	TDF_ENVIRON_ORIGI_MODE = 50,
};

enum TDF_MSG_ID_INNER
{
	MSG_DATA_ORIGINAL = 99,
};

enum ORI_MSG_DATA_TYPE
{
	ORI_SNAPSHOT = 0,//����(�������պ�ί�ж��ж��б仯)
	ORI_ORDERQUEUE = 1,//ί�ж��У��۹ɾ����̶���(��ί�ж��б仯)
};

//ԭʼ���ݽṹ
struct TDF_ORIDATA_MSG
{
	int						nCodeDate;			//���������
	char					marketKey[12];		//markeyKey,TW-1-0,TWO-1-0
	TDMarketData_Packet     marketData;         //ԭʼ���ݽṹ
	ORI_MSG_DATA_TYPE		eMsgDataType;		//ԭʼ�ṹ�������ͣ�����+ί�ж��л��ί�ж���
	int						nSide;				//ί�ж��з�����:0x01����:0x02����ͬʱ����
};

#pragma  pack(pop)
#endif