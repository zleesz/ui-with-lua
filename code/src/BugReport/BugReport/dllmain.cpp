// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "BugReport.h"

HINSTANCE g_hInstDll = NULL ;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	g_hInstDll = (HINSTANCE)hModule ;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


extern "C" __declspec( dllexport )
int BootReport()
{
	CBugReport::Instance().SetUnhandleExceptionFilter();
	return 0;
}

extern "C" __declspec( dllexport )
int StopReport()
{
	CBugReport::Instance().ResetUnhandleExceptionFilter();
	return 0;
}