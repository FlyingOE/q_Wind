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

#ifdef __cplusplus
}//extern "C"
#endif

/////////////////////////////////////////////////////////////////////////////