#ifndef _TDCORE_H
#define _TDCORE_H

#ifndef TD_EXT_CLASS
  #ifdef TD_STATIC
	#ifndef _WIN32
		#define TCHAR char
	#endif
	#define TD_EXT_CLASS
  #else
    #ifdef TDCORE_EXPORTS
      #define TD_EXT_CLASS __declspec(dllexport)
    #else
      #define TD_EXT_CLASS __declspec(dllimport)
    #endif
  #endif
#endif

#ifdef _WIN32
//#include <stdint.h>
typedef unsigned long long uint64_t;
#include <Windows.h>
#endif

#ifndef _WIN32
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef int int32_t;
typedef unsigned int  uint32_t;
typedef struct _GUID {
    uint32_t  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} GUID;
void UuidCreate(GUID *pGuid);

// 必须为int，保持Windows平台一致
typedef int BOOL;

#define _MAX_PATH   260

#endif


#define GET_DATASIZE_FROM_SAVEHEAD(pSaveHead) \
	( (*(int64_t*)&pSaveHead->nResv[1] == 0) ? pSaveHead->nDataSize : *(int64_t*)&pSaveHead->nResv[1])


#endif
