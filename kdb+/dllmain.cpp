// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"


void activateMemoryLeakChecks();


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		activateMemoryLeakChecks();
		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
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