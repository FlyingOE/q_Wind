// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

#include "Wind_API.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	BOOL status = TRUE;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//@see https://msdn.microsoft.com/en-us/library/aa370448(v=vs.85).aspx
		::DisableThreadLibraryCalls(hModule);

		status = Wind::prepare();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		status = Wind::finalize();
		break;
	}
	return status;
}

