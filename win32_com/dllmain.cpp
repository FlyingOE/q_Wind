// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "win32_com.h"

#include "win32.util/COMWrapper.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	BOOL status = TRUE;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		status = com::InitInstances();
		break;
	case DLL_THREAD_ATTACH:
		status = com::Initialize();
		break;
	case DLL_THREAD_DETACH:
		status = com::Uninitialize();
		break;
	case DLL_PROCESS_DETACH:
		status = com::UninitInstances();
		break;
	}
	return status;
}