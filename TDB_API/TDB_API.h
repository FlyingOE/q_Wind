// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TDB_API_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TDB_API_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef TDB_API_EXPORTS
#define TDB_API __declspec(dllexport)
#else
#define TDB_API __declspec(dllimport)
#endif

// kdb+ 32-bit API
#ifdef WIN32
#define K_DECL	_cdecl
#else
#define K_DECL
#endif

#pragma comment(lib, "TDBAPI.lib")

#ifdef __cplusplus
extern "C" {
#endif

	TDB_API K K_DECL getTimeout(K/*NOTE: at least one argument required by `2:'*/);
	TDB_API K K_DECL setTimeout(
		K timeout,	//-5h or -6h or -7h
		K retries,	//-5h or -6h or -7h
		K delay		//-5h or -6h or -7h
		);

	TDB_API K K_DECL getDataSource(K/*NOTE: at least one argument required by `2:'*/);
	TDB_API K K_DECL setDataSource(
		K source	//-4h or -5h or -6h or -7h
		);

	TDB_API K K_DECL TDB_login(
		K level,	//-4h or -5h or -6h or -7h
		K host,		//-11h or 10h
		K port,		//-5h or -6h or -7h
		K username,	//-11h or 10h
		K password	//-11h or 10h
		);

	TDB_API K K_DECL TDB_logout(
		K tdb		//-7h
		);

	//获取代码表
	TDB_API K K_DECL TDB_codeTable(
		K tdb,		//-7h
		K market	//-11h or 10h
		);

	//获取代码的详细信息
	TDB_API K K_DECL TDB_codeInfo(
		K tdb,		//-7h
		K windCode,	//-11h or 10h
		K market	//-11h or 10h
		);

	//获取快照数据
	TDB_API K K_DECL TDB_tick_fields(
		K type		//-11h
		);
	TDB_API K K_DECL TDB_tick(
		K tdb,			//-7h
		K windCode,		//-11h or 10h
		K indicators,	//-11h or 11h
		K date,			//-14h or -5h or -6h or -7h
		K begin,		//-19h or -18h or -17h or -5h or -6h or -7h
		K end			//-19h or -18h or -17h or -5h or -6h or -7h
		);

	//获取逐笔成交数据
	TDB_API K K_DECL TDB_transaction_fields(K/*NOTE: at least one argument required by `2:'*/);
	TDB_API K K_DECL TDB_transaction(
		K tdb,			//-7h
		K windCode,		//-11h or 10h
		K indicators,	//-11h or 11h
		K date,			//-14h or -5h or -6h or -7h
		K begin,		//-19h or -18h or -17h or -5h or -6h or -7h
		K end			//-19h or -18h or -17h or -5h or -6h or -7h
		);

	//获取逐笔委托数据
	TDB_API K K_DECL TDB_order_fields(K/*NOTE: at least one argument required by `2:'*/);
	TDB_API K K_DECL TDB_order(
		K tdb,			//-7h
		K windCode,		//-11h or 10h
		K indicators,	//-11h or 11h
		K date,			//-14h or -5h or -6h or -7h
		K begin,		//-19h or -18h or -17h or -5h or -6h or -7h
		K end			//-19h or -18h or -17h or -5h or -6h or -7h
		);

	//获取委托队列数据
	TDB_API K K_DECL TDB_orderQueue_fields(K/*NOTE: at least one argument required by `2:'*/);
	TDB_API K K_DECL TDB_orderQueue(
		K tdb,			//-7h
		K windCode,		//-11h or 10h
		K indicators,	//-11h or 11h
		K date,			//-14h or -5h or -6h or -7h
		K begin,		//-19h or -18h or -17h or -5h or -6h or -7h
		K end			//-19h or -18h or -17h or -5h or -6h or -7h
		);

#ifdef __cplusplus
}//extern "C"
#endif

/////////////////////////////////////////////////////////////////////////////
#include <map>

namespace TDB {
	
	// Default settings for Wind TDB's remote calls
	//NOTE: szUser & szPassword in this variable shall _not_ be used!
	extern ::OPEN_SETTINGS SETTINGS;

	// Default data levels (L1/L2) for Wind TDB's remote calls
	extern std::map<::THANDLE, short> LEVELS;

	// Default data source ID for Wind TDB's remote calls
	extern short DATA_SRC;

}//namespace TDB