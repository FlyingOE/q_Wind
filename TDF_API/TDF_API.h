// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TDF_API_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TDF_API_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef TDF_API_EXPORTS
#define TDF_API __declspec(dllexport)
#else
#define TDF_API __declspec(dllimport)
#endif

// kdb+ 32-bit API
#ifdef WIN32
#define K_DECL	_cdecl
#else
#define K_DECL
#endif

#pragma comment(lib, "TDFAPI30.lib")

#ifdef __cplusplus
extern "C" {
#endif

	TDF_API K K_DECL setTimeout(
		K hbInterval,	//-5h or -6h or -7h
		K hbMissing,	//-5h or -6h or -7h
		K openTimeout	//-5h or -6h or -7h
		);

	TDF_API K K_DECL TDF_subscribe(
		K servers,		//99h: `host`port`username`password!(-11h or 10h;5h or 6h or 7h;-11h or 10h;-11h or 10h)
		K markets,		//-11h or 11h
		K windCodes,	//-11h or 11h
		K msgTypes,		//-11h or 11h
		K startTime		//-19h
		);

	TDF_API K K_DECL TDF_unsubscribe(
		K tdf		//-7h
		);

	//获取代码表（在已经收到MSG_SYS_CODETABLE_RESULT 消息之后，可以获得代码表）
	TDF_API K K_DECL TDF_codeTable(
		K tdf,		//-7h
		K market	//-11h or 10h
		);

	//从万得代码来获取详细的期权代码信息
	TDF_API K K_DECL TDF_optionCodeInfo(
		K tdf,		//-7h
		K windCode	//-11h or 10h
		);

	TDF_API K K_DECL TDF_index_fields(K/*NOTE: at least one argument required by `2:'*/);
	TDF_API K K_DECL TDF_market_fields(K/*NOTE: at least one argument required by `2:'*/);
	TDF_API K K_DECL TDF_future_fields(K/*NOTE: at least one argument required by `2:'*/);

#ifdef __cplusplus
}//extern "C"
#endif

/////////////////////////////////////////////////////////////////////////////
namespace TDF {

	// Default settings for Wind TDF's remote calls
	//NOTE: szUser & szPwd in this variable shall _not_ be used!
	extern ::TDF_OPEN_SETTING_EXT SETTINGS;

	// Initializations - only WinSock for now...
	BOOL prepare();
	// Finalizations - only WinSock2 for now...
	BOOL finalize();

}//namespace TDF