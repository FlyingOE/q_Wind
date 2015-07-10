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

#pragma comment(lib, "TDB_API_Windows_v2.lib")

#ifdef __cplusplus
extern "C" {
#endif

	TDB_API K K_DECL getTimeout(K/*NOTE: at least one argument required by `2:'*/);
	TDB_API K K_DECL setTimeout(
		K timeout,	//-5h or -6h or -7h
		K retries,	//-5h or -6h or -7h
		K delay		//-5h or -6h or -7h
		);

	TDB_API K K_DECL TDB_login(
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

	//获取快照数据(带买卖盘口)
	TDB_API K K_DECL TDB_tickAB_fields(K/*NOTE: at least one argument required by `2:'*/);
	TDB_API K K_DECL TDB_tickAB(
		K tdb,			//-7h
		K windCode,		//-11h or 10h
		K indicators,	//-11h or 11h
		K begin,		//-15h
		K end			//-15h
		);

	//获取逐笔成交数据
	TDB_API K K_DECL TDB_transaction_fields(K/*NOTE: at least one argument required by `2:'*/);
	TDB_API K K_DECL TDB_transaction(
		K tdb,			//-7h
		K windCode,		//-11h or 10h
		K indicators,	//-11h or 11h
		K begin,		//-15h
		K end			//-15h
		);

#ifdef __cplusplus
}//extern "C"
#endif

/////////////////////////////////////////////////////////////////////////////
namespace TDB {
	
	// Default settings for Wind TDB's remote calls
	//NOTE: szUser & szPassword in this variable shall _not_ be used!
	extern ::OPEN_SETTINGS SETTINGS;

}//namespace TDB