// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WIND_API_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WIND_API_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef WIND_API_EXPORTS
#define WIND_API __declspec(dllexport)
#else
#define WIND_API __declspec(dllimport)
#endif

// kdb+ 32-bit API
#ifdef WIN32
#define K_DECL	_cdecl
#else
#define K_DECL
#endif

#pragma comment(lib, "WindQuantData.lib")

#ifdef __cplusplus
extern "C" {
#endif

	WIND_API K K_DECL getTimeout(K/*NOTE: at least one argument required by 2:!*/);
	WIND_API K K_DECL setTimeout(K timeout);

	WIND_API K K_DECL Wind_login(
		K username,	//-11h or 10h
		K password	//-11h or 10h
		);

	WIND_API K K_DECL Wind_logout(K/*dummy*/);

	WIND_API K K_DECL Wind_wsd(
		K windCode,		//-11h or 10h
		K indicators,	//-11h or 11h
		K beginDate,	//-14h
		K endDate,		//-14h
		K params		//0h=() or 11h!0h
		);

	WIND_API K K_DECL Wind_wss(
		K windCodes,	//-11h or 11h or 10h or 0h=(10h;...)
		K indicators,	//-11h or 11h
		K params		//0h=() or 11h!0h
		);

	WIND_API K K_DECL Wind_wsi(
		K windCode,		//-11h or 10h
		K indicators,	//-11h or 11h
		K beginTime,	//-14h or -15h
		K endTime,		//-14h or -15h
		K params		//0h=() or 11h!0h
		);

	WIND_API K K_DECL Wind_wst(
		K windCode,		//-11h or 10h
		K indicators,	//-11h or 11h
		K beginTime,	//-14h or -15h
		K endTime,		//-14h or -15h
		K params		//0h=() or 11h!0h
		);

	WIND_API K K_DECL Wind_wset(
		K reportName,	//-11h or 10h
		K params		//0h=() or 11h!0h
		);

	WIND_API K K_DECL Wind_wsq(
		K windCodes,	//-11h or 11h or 10h or 0h=(10h;...)
		K indicators,	//-11h or 11h
		K params,		//0h=() or 11h!0h
		K callback		//-11h
		);

	WIND_API K K_DECL Wind_cr(K qid);
	WIND_API K K_DECL Wind_car(K/*dummy*/);

	WIND_API K K_DECL Wind_tdays(
		K beginDate,	//-14h
		K endDate,		//-14h
		K params		//0h=() or 11h!0h
		);

	WIND_API K K_DECL Wind_tdaysoff(
		K beginDate,	//-14h
		K offset,		//-5h or -6h or -7h or 10h
		K params		//0h=() or 11h!0h
		);

	WIND_API K K_DECL Wind_tdayscnt(
		K beginDate,	//-14h
		K endDate,		//-14h
		K params		//0h=() or 11h!0h
		);

	WIND_API K K_DECL Wind_wupf(
		K portfolioName,	//-11h or 10h
		K tradeDates,	//-14h or 14h
		K windCodes,	//-11h or 11h or 10h or 0h=(10h;...)
		K quantities,	//-5h or 5h or -6h or 6h or -7h or 7h
		K costPrices,	//-8h or 8h or -9h or 9h
		K params		//0h=() or 11h!0h
		);

#ifdef __cplusplus
}//extern "C"
#endif

/////////////////////////////////////////////////////////////////////////////
#include <chrono>	//C++11

namespace Wind {
	// Timeout for Wind's asynchronous calls
	extern std::chrono::milliseconds ASYNC_TIMEOUT;

	// Initializations - only WinSock for now...
	BOOL prepare();
	// Finalizations - only WinSock2 for now...
	BOOL finalize();

	namespace pubsub {
		bool prepare();
		bool finalize();
	}//namespace Wind::pubsub

}//namespace Wind