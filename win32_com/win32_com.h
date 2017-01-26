// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WIN32_COM_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WIN32_COM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef WIN32_COM_EXPORTS
#define WIN32_COM_API __declspec(dllexport)
#else
#define WIN32_COM_API __declspec(dllimport)
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

	WIN32_COM_API K K_DECL createCOM(
		K progId,		//-11h or 10h
		K serverType	//-4h (@ref CLSCTX_*)
		);
	WIN32_COM_API K K_DECL releaseCOM(K handle);

	WIN32_COM_API K K_DECL invokeCOM(
		K handle,		//-7h
		K getOrPut,		//-5h (@ref DISPATCH_PROPERTYGET | DISPATCH_PROPERTYPUT)
		K property,		//-11h or 10h
		K values		//0h
		);		

	WIN32_COM_API K K_DECL version(K/*NOTE: at least one argument required by 2:!*/);

#ifdef __cplusplus
}
#endif

namespace com {

	bool InitInstances();
	bool UninitInstances();

}//namespace com