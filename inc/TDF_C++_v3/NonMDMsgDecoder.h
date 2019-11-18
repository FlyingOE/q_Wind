#pragma pack(push)
#pragma pack(1)

#include <string.h>
#include "TDFAPIStruct.h"

#ifndef UINT32
#define UINT32 unsigned int
#endif

#if defined(WIN32) || defined(WIN64) || defined(_WINDOWS)
#ifdef TDF_API_EXPORT
#define NonMDMMsg __declspec(dllexport) 
#else	
#define NonMDMMsg __declspec(dllimport)
#endif
#else
#define NonMDMMsg __attribute((visibility("default")))
#endif

struct _NonMD_Msg_Def;

#ifdef __cplusplus
extern "C" {
#endif

	class MsgDecoder
	{
	public:
		MsgDecoder()
		{
			pMsg = NULL;
		}

		NonMDMMsg int GetMsgType();

		NonMDMMsg bool Init(char* _pMsg, int size);

		NonMDMMsg int GetInt32Field(int index);

		NonMDMMsg __int64 GetInt64Field(int index);

		NonMDMMsg unsigned __int64 GetUInt64Field(int index);

		NonMDMMsg UINT32 GetUInt32Field(int index);

		NonMDMMsg float GetFloatField(int index);

		NonMDMMsg double GetDoubleField(int index);

		NonMDMMsg char GetCharField(int index);

		NonMDMMsg char* GetStringField(int index);

	private:
		_NonMD_Msg_Def* pDef;
		char* pMsg;
	};

#ifdef __cplusplus
}
#endif

#pragma pack(pop)