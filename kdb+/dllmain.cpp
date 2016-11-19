// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

void activateMemoryLeakChecks();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	BOOL status = TRUE;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//@ref https://msdn.microsoft.com/en-us/library/aa370448(v=vs.85).aspx
		::DisableThreadLibraryCalls(hModule);

		activateMemoryLeakChecks();
		break;

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return status;
}

//@ref http://pages.cs.wisc.edu/~tlabonne/memleak.html
#include "CrtDbg.h"
void activateMemoryLeakChecks() {
#	ifndef NDEBUG
	int flags = ::_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flags |= _CRTDBG_LEAK_CHECK_DF;
	::_CrtSetDbgFlag(flags);
#	endif
}