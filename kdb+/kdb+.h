// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the KDB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// KDB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef KDB_EXPORTS
#define KDB_API __declspec(dllexport)
#else
#define KDB_API __declspec(dllimport)
#endif

#ifdef WIN32
#define K_DECL	_cdecl
#else
#define K_DECL
#endif

#pragma comment(lib, "q.lib")

#ifdef __cplusplus
extern "C" {
#endif

	KDB_API K K_DECL gb18030_utf8(K strGBK);
	KDB_API K K_DECL utf8_gb18030(K strUTF);

	KDB_API K K_DECL cwd(K/*NOTE: at least one argument required by 2:!*/);

#ifdef __cplusplus
}
#endif