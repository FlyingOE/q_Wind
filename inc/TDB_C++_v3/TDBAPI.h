
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

/************************************************************************/
	//返回值说明：具体参看TDB_ERROR
	//TDB_SUCCESS: 成功
	//TDB_INVALID_PARAMS: 无效的参数(hTdb无效，pReq为空，pCount为空)
	//TDB_NETWORK_ERROR：网络连接错误
	//TDB_NO_DATA: 没有数据
	//TDB_NETWORK_TIMEOUT:超时
	//TDB_OUT_OF_MEMORY：内存耗尽
/************************************************************************/

/**
	功能：打开一个到TDB服务器的连接，如果连接成功，则返回连接句柄，失败返回NULL。
	* @param[in]   OPEN_SETTINGS*			pSetting;	OPEN_SETTINGS结构体设置
	* @param[out]  TDBDefine_ResLogin*		loginRes;	登录结果
	* @return THANDLE句柄
*/
TDBAPI THANDLE TDB_Open(const OPEN_SETTINGS* pSetting, TDBDefine_ResLogin* loginRes);

/**
	功能：代理打开一个到TDB服务器的连接，如果连接成功，则返回连接句柄，失败返回NULL。
	* @param[in]   OPEN_SETTINGS*			pSetting;		OPEN_SETTINGS设置
	* @param[in]  TDB_PROXY_SETTING*		pProxySetting;	TDB_PROXY_SETTING代理设置
	* @param[out]  TDBDefine_ResLogin*		loginRes;		登录结果
	* @return THANDLE句柄
*/
TDBAPI THANDLE TDB_OpenProxy(const OPEN_SETTINGS* pSetting, TDB_PROXY_SETTING* pProxySetting, TDBDefine_ResLogin* loginRes);

/**
	功能：主动关闭连接，并销毁Handle
	* @param[in]  THANDLE hTdb;		登录返回的句柄
	* @return 关闭结果
*/
TDBAPI int TDB_Close(THANDLE hTdb);

/**
	功能：获取代码表，获得的结果需要用户主动调用 TDB_Free来释放
	* @param[in]  THANDLE			hTdb;		登录返回的句柄
	* @param[in]  char*				szMarket;	市场(格式:Market-Level-Source(SZ-2-0)),为空请求所有授权市场
	* @param[out]  TDBDefine_Code**	pCodeTable; 代码表
	* @param[out]  int*				pCount;		代码表项数
	* @return
*/
TDBAPI int TDB_GetCodeTable(THANDLE hTdb, const char* szMarket, TDBDefine_Code** pCodeTable, int* pCount);

/**
	功能：获取代码的详细信息,如果连接断掉或者未连接，则返回NULL，返回的指针由API用户不能释放之
	* @param[in]  THANDLE hTdb;			登录返回的句柄
	* @param[in]  char*	  szWindCode;	万得代码(000001.SZ)
	* @param[in] char*	  strMarket;	市场(格式:Market-Level-Source(SZ-2-0))
	* @return TDBDefine_Code* 代码信息
*/
TDBAPI const TDBDefine_Code* TDB_GetCodeInfo(THANDLE hTdb, const char* szWindCode, const char* strMarket);

/**
	功能：获取K线，获得的结果需要用户主动调用 TDB_Free来释放
	* @param[in]  THANDLE				hTdb;	登录返回的句柄
	* @param[in]  TDBDefine_ReqKLine*	pReq;	请求结构体(秒线(TickBar)时间间隔为7个自然日，分钟线为30个自然日)
	* @param[out] TDBDefine_KLine**		pData;	K线结果
	* @param[out] int*					pCount;	K线项数
	* @return 请求状态
*/
TDBAPI int TDB_GetKLine(THANDLE hTdb, const TDBDefine_ReqKLine* pReq, TDBDefine_KLine** pData, int* pCount);

/**
	功能：获取Tick数据，获得的结果需要用户主动调用 TDB_Free来释放
	* @param[in]  THANDLE				hTdb;	登录返回的句柄
	* @param[in]  TDBDefine_ReqTick*	pReq;	请求结构体
	* @param[out] TDBDefine_Tick**		pData;	Tick结果
	* @param[out] int*					pCount;	Tick项数
	* @return 请求状态
*/
TDBAPI int TDB_GetTick(THANDLE hTdb, const TDBDefine_ReqTick* pReq, TDBDefine_Tick** pData, int* pCount);

/**
	功能：获取逐笔成交数据，获得的结果需要用户主动调用 TDB_Free来释放
	* @param[in]  THANDLE					hTdb;	登录返回的句柄
	* @param[in]  TDBDefine_ReqTransaction*	pReq;	请求结构体
	* @param[out] TDBDefine_Transaction**	pData;	Transaction结果
	* @param[out] int*						pCount;	Transaction项数
	* @return 请求状态
*/
TDBAPI int TDB_GetTransaction(THANDLE hTdb, const TDBDefine_ReqTransaction* pReq, TDBDefine_Transaction** pData, int* pCount);

/**
	功能：获取逐笔委托数据，获得的结果需要用户主动调用 TDB_Free来释放
	* @param[in]  THANDLE					hTdb;	登录返回的句柄
	* @param[in]  TDBDefine_ReqOrder*		pReq;	请求结构体
	* @param[out] TDBDefine_Order**			pData;	Order结果
	* @param[out] int*						pCount;	Order项数
	* @return 请求状态
*/
TDBAPI int TDB_GetOrder(THANDLE hTdb, const TDBDefine_ReqOrder* pReq, TDBDefine_Order** pData, int* pCount);

/**
	功能：获取委托队列数据，获得的结果需要用户主动调用 TDB_Free来释放
	* @param[in]  THANDLE					hTdb;	登录返回的句柄
	* @param[in]  TDBDefine_ReqOrderQueue*	pReq;	请求结构体
	* @param[out] TDBDefine_OrderQueue**    pData;	OrderQueue结果
	* @param[out] int*						pCount;	OrderQueue项数
	* @return 请求状态
*/
TDBAPI int TDB_GetOrderQueue(THANDLE hTdb, const TDBDefine_ReqOrderQueue* pReq, TDBDefine_OrderQueue** pData, int* pCount);

/**
	功能：上载指标公式
	* @param[in]  THANDLE					hTdb;		登录返回句柄
	* @param[in]  char*						szName;		公式
	* @param[in]  char*						szContent;	公式内容
	* @param[out]  TDBDefine_AddFormulaRes*	pRes;		返回结果
	* @return 请求状态
*/
TDBAPI int TDB_AddFormula(THANDLE hTdb, const char* szName, const char* szContent, TDBDefine_AddFormulaRes* pRes);

/**
	功能：获取服务器上的指标公式，获得的结果需要用户主动调用 TDB_Free来释放
	* @param[in]  THANDLE					hTdb;			登录返回的句柄
	* @param[in]  char*						szFormulaName;	公式名称，为""则请求存在的全部公式
	* @param[out] TDBDefine_FormulaItem**   pFormula;		指标公式结果
	* @param[out] int*						pCount;			指标公式项数
	* @return 请求状态
*/
TDBAPI int TDB_GetFormula(THANDLE hTdb, const char* szFormulaName, TDBDefine_FormulaItem** pFormula, int *pCount);

//获取到结果之后，应调主动用TDB_ReleaseCalcFormula释放内存
/**
	功能：获取指标公式计算结果，应主动调用TDB_ReleaseCalcFormula释放内存
	* @param[in]  THANDLE					hTdb;			登录返回的句柄
	* @param[in]  TDBDefine_ReqCalcFormula*	szFormulaName;	指标公式计算请求
	* @param[out] TDBDefine_CalcFormulaRes* pFormula;		指标公式计算结果
	* @return 请求状态
*/
TDBAPI int TDB_CalcFormula(THANDLE hTdb, const TDBDefine_ReqCalcFormula* pReqCalc, TDBDefine_CalcFormulaRes* pResult);


/**
	功能：删除指标公式
	* @param[in]  THANDLE					hTdb;			登录返回句柄
	* @param[in]  char*						szFormulaName;  公式名
	* @param[out]  TDBDefine_DelFormulaRes*	pDelRes;		返回结果
	* @return 请求状态
*/
TDBAPI int TDB_DeleteFormula(THANDLE hTdb, const char* szFormulaName, TDBDefine_DelFormulaRes* pDelRes);

/**
	功能：释放TDB_CalcFormula调用获取的TDBDefine_CalcFormulaResult结果中的动态内存，pResult必须被 TDB_CalcFormula初始化。
	* @param[in]  TDBDefine_CalcFormulaRes*	pResult; 指标公式结果
	* @return void
*/
TDBAPI void TDB_ReleaseCalcFormula(TDBDefine_CalcFormulaRes* pResult);

/**
	功能：客户调用释放
	* @param[in]  void*	p; 需要释放的指针
	* @return void
*/
TDBAPI void TDB_Free(void* p);

#ifdef __cplusplus
}
#endif

#endif//_TDBAPI_H_
