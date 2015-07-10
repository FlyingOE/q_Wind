/**
* @defgroup  MAINFRAME 系统资源区
* @author 
* @version 1.0
* @date 2010-2013
* @warning 版权所有，翻录必究
* @{
* Tips:
*/

#ifndef _TDBAPI_H_
#define _TDBAPI_H_

#include "TDBAPIStruct.h"

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) || defined(_WINDOWS)
#define __PLATFORM_WINDOWS__
#else
#define __PLATFORM_LINUX__
#endif

#ifdef __PLATFORM_WINDOWS__
    #ifdef TDB_API_EXPORT
        #define TDBAPI __declspec(dllexport)
    #else    
        #define TDBAPI __declspec(dllimport)
    #endif
#else
    #define TDBAPI
#endif


#ifdef __cplusplus
extern "C"
{
#endif

//打开一个到TDB服务器的连接，如果连接成功，则返回连接句柄，失败返回NULL。
//loginRes中返回市场，动态数据日期，
TDBAPI THANDLE TDB_Open(const OPEN_SETTINGS* pSetting, TDBDefine_ResLogin* loginRes);

//通过代理打开连接，错误代码和TDB_Open一样
TDBAPI THANDLE TDB_OpenProxy(const OPEN_SETTINGS* pSetting, TDB_PROXY_SETTING* pProxySetting, TDBDefine_ResLogin* loginRes);

//主动关闭连接，并销毁Handle
TDBAPI int TDB_Close(THANDLE hTdb);

//获取代码表
//返回值：
//TDB_SUCCESS: 成功
//TDB_INVALID_PARAMS: 无效的参数（hTdb无效，pCount为空）
//TDB_NETWORK_ERROR：网络连接错误
//若szMarket为空，则返回所有市场的代码
//若pCodeTable为空，pCount不为空，则返回
//*pCodeTable所指向的内存需要用户显式的调用 TDB_Free 释放
TDBAPI int TDB_GetCodeTable(THANDLE hTdb, const char* szMarket, TDBDefine_Code** pCodeTable, int* pCount);


//获取K线
//返回值：
//TDB_SUCCESS: 成功
//TDB_INVALID_PARAMS: 无效的参数(hTdb无效，pReq为空，pCount为空)
//TDB_NETWORK_ERROR：网络连接错误
//TDB_NO_DATA: 没有数据
//TDB_NETWORK_TIMEOUT:超时
//TDB_OUT_OF_MEMORY：内存耗尽
//若pData为空，pCount不为空，则返回记录条数
//*pData 指向的内存块需要用户显式的调用TDB_Free 释放
TDBAPI int TDB_GetKLine(THANDLE hTdb, const TDBDefine_ReqKLine* pReq, TDBDefine_KLine** pData, int* pCount);


//获取快照数据(带买卖盘口)，获得的结果需要用户主动调用 TDB_Free 来释放
TDBAPI int TDB_GetTickAB(THANDLE hTdb, const TDBDefine_ReqTick* pReq, TDBDefine_TickAB** pData, int* pCount);

//获取快照数据(不带买卖盘口)，获得的结果需要用户主动调用 TDB_Free 来释放
TDBAPI int TDB_GetTick(THANDLE hTdb, const TDBDefine_ReqTick* pReq, TDBDefine_Tick** pData, int* pCount);


//获取期货行情(不带买卖盘口)，获得的结果需要用户主动调用 TDB_Free 来释放
TDBAPI int TDB_GetFuture(THANDLE hTdb, const TDBDefine_ReqFuture* pReq, TDBDefine_Future** pData, int* pCount);

//获取期货行情，获得的结果需要用户主动调用 TDB_Free 来释放
TDBAPI int TDB_GetFutureAB(THANDLE hTdb, const TDBDefine_ReqFuture* pReq, TDBDefine_FutureAB** pData, int* pCount);

//获取代码的详细信息,如果连接断掉或者未连接，则返回NULL，返回的指针由API用户不能释放之
TDBAPI const TDBDefine_Code* TDB_GetCodeInfo(THANDLE hTdb, const char* szWindCode);

//获取逐笔成交数据，获得的结果需要用户主动调用 TDB_Free来释放
TDBAPI int TDB_GetTransaction(THANDLE hTdb, const TDBDefine_ReqTransaction* pReq, TDBDefine_Transaction** pData, int* pCount);

//获取逐笔委托数据，获得的结果需要用户主动调用 TDB_Free 来释放
TDBAPI int TDB_GetOrder(THANDLE hTdb, const TDBDefine_ReqOrder* pReq, TDBDefine_Order** pData, int* pCount);

//获取委托队列数据，获得的结果需要用户主动调用 TDB_Free 来释放
TDBAPI int TDB_GetOrderQueue(THANDLE hTdb, const TDBDefine_ReqOrderQueue* pReq, TDBDefine_OrderQueue** pData, int* pCount);

//上载指标公式
TDBAPI int TDB_AddFormula(THANDLE hTdb, const char* szName, const char* szContent, TDBDefine_AddFormulaRes* pRes);

//获取服务器上的指标公式，获得的结果需要用户主动调用 TDB_Free 来释放，如果为空或者""，则获取所有的指标公式
TDBAPI int TDB_GetFormula(THANDLE hTdb, const char* szFormulaName, TDBDefine_FormulaItem** pFormula, int *pCount);

//计算指标公式，并获取结果，pResult不可为空，并且CalcFormulaResult内部的动态内存字段由API分配。
//获取到结果之后，应调主动用TDB_ReleaseCalcFormula释放内存
TDBAPI int TDB_CalcFormula(THANDLE hTdb, const TDBDefine_ReqCalcFormula* pReqCalc, TDBDefine_CalcFormulaRes* pResult);

//释放TDB_CalcFormula调用获取的TDBDefine_CalcFormulaResult结果中的动态内存，pResult必须被 TDB_CalcFormula初始化。
TDBAPI void TDB_ReleaseCalcFormula(TDBDefine_CalcFormulaRes* pResult);


TDBAPI void TDB_Free(void* p);

//删除指标公式
TDBAPI int TDB_DeleteFormula(THANDLE hTdb, const char* szFormulaName, TDBDefine_DelFormulaRes* pDelRes);

////如果当前正在传输数据，则终止之
//TDBAPI void TDB_AbortRecv(THANDLE hTdb);


//清除指标公式结果，释放内存
//TDBAPI int TDB_ReleaseEZFResult(THANDLE hTdb, TDBDefine_EZFormula_ResultData* pRetData);


#ifdef __cplusplus
}
#endif

#endif//_TDBAPI_H_
