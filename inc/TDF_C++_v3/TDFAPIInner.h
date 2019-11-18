#ifndef __TDF_API_INNER_H__
#define __TDF_API_INNER_H__
#pragma pack(push)
#pragma pack(1)

#include "TDFAPIStruct.h"
#include "TDFDataDefine.h"

//环境设置,设置该类型后，回调返回原始结构体
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
	ORI_SNAPSHOT = 0,//快照(基础快照和委托队列都有变化)
	ORI_ORDERQUEUE = 1,//委托队列，港股经纪商队列(仅委托队列变化)
};

//原始数据结构
struct TDF_ORIDATA_MSG
{
	int						nCodeDate;			//代码表日期
	char					marketKey[12];		//markeyKey,TW-1-0,TWO-1-0
	TDMarketData_Packet     marketData;         //原始数据结构
	ORI_MSG_DATA_TYPE		eMsgDataType;		//原始结构数据类型，快照+委托队列或仅委托队列
	int						nSide;				//委托队列方向，卖:0x01，买:0x02，可同时存在
};

#pragma  pack(pop)
#endif