/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: DumpSystemInfo.cpp
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/

#include "StdAfx.h"
#include "DumpSystemInfo.h"
#include <AtlConv.h>

/********************************** 宏、常量 **********************************/

/********************************** 数据类型 **********************************/

/************************************ 变量 ************************************/

/********************************** 函数实现 **********************************/

/*********************************** 类实现 ***********************************/

CDumpSystemInfo::CDumpSystemInfo(void)
{
	strcpy(m_achDumpType,"[System Info]");
}

CDumpSystemInfo::~CDumpSystemInfo(void)
{
}

BOOL CDumpSystemInfo::DumpFile(HANDLE hDumpFile)
{
	TCHAR ptszOSVer[256];
	if(!GetOSVersionString(ptszOSVer,256))
	{
		return FALSE ;
	}

	_tcscat(ptszOSVer,_T(".\r\n\r\n"));

	USES_CONVERSION ;

	char * pTemp = T2A(ptszOSVer);
	DWORD dwLen = strlen(pTemp);

	::WriteFile(hDumpFile,pTemp,dwLen,&dwLen,NULL);

	return TRUE ;
}

BOOL CDumpSystemInfo::LoadFile(HANDLE hDumpFile)
{
	return TRUE ;
}

BOOL CDumpSystemInfo::GetOSVersionString(LPTSTR ptszOSVer,DWORD dwLen)
{
	if(!ptszOSVer) return FALSE ;

	typedef void (WINAPI *PFN_GetSysInfo)(LPSYSTEM_INFO);

	TCHAR atszOSVer[256];
	TCHAR atszTemp[128];

	OSVERSIONINFOEX OSVerInfo;
	SYSTEM_INFO		SysInfo;
	PFN_GetSysInfo	pfnGetSystemInfo ;
	BOOL			bOsVersionInfoEx;

	atszOSVer[0] = 0 ;
	atszTemp[0] = 0 ;
	ZeroMemory(&SysInfo,sizeof(SYSTEM_INFO));
	ZeroMemory(&OSVerInfo,sizeof(OSVERSIONINFOEX));

	// 获取 OSVERSIONINFOEX 信息
	OSVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if(!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *)&OSVerInfo)))
	{
		OSVerInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (!GetVersionEx((OSVERSIONINFO *)&OSVerInfo)) 
		{
			return FALSE;
		}
	}

	// 获取 SYSTEM_INFO 信息
	pfnGetSystemInfo = (PFN_GetSysInfo)GetProcAddress(GetModuleHandle(_T("kernel32.dll")),"GetNativeSystemInfo");
	if(NULL != pfnGetSystemInfo)
	{
		pfnGetSystemInfo(&SysInfo);
	}
	else 
	{
		GetSystemInfo(&SysInfo);
	}

	// 判断信息表示的系统类型
	switch (OSVerInfo.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT:
		{
			// OS主类型
			if(OSVerInfo.dwMajorVersion == 6 && OSVerInfo.dwMinorVersion == 0)
			{
				if(OSVerInfo.wProductType == VER_NT_WORKSTATION)
				{
					_tcscpy(atszOSVer,_T("Windows Vista "));
				}
				else 
				{
					_tcscpy(atszOSVer,_T("Windows Server \"Longhorn\" "));
				}
			}
			else if(OSVerInfo.dwMajorVersion == 5 && OSVerInfo.dwMinorVersion == 2)
			{
				if(GetSystemMetrics(SM_SERVERR2))
				{
					_tcscpy(atszOSVer,_T("Microsoft Windows Server 2003 \"R2\" "));
				}
				else if(OSVerInfo.wProductType == VER_NT_WORKSTATION && SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				{
					_tcscpy(atszOSVer,_T("Microsoft Windows XP Professional x64 Edition "));
				}
				else 
				{
					_tcscpy(atszOSVer,_T("Microsoft Windows Server 2003 "));
				}
			}
			else if(OSVerInfo.dwMajorVersion == 5 && OSVerInfo.dwMinorVersion == 1)
			{
				_tcscpy(atszOSVer,_T("Microsoft Windows XP "));
			}
			else if(OSVerInfo.dwMajorVersion == 5 && OSVerInfo.dwMinorVersion == 0)
			{
				_tcscpy(atszOSVer,_T("Microsoft Windows 2000 "));
			}
			else if(OSVerInfo.dwMajorVersion <= 4)
			{
				_tcscpy(atszOSVer,_T("Microsoft Windows NT "));
			}
			else
			{
				_tcscpy(atszOSVer,_T("Unknown System Version"));
				break;
			}

			// OS扩展类型
			if(bOsVersionInfoEx)
			{
				if(OSVerInfo.wProductType == VER_NT_WORKSTATION && SysInfo.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_AMD64)
				{
					if(OSVerInfo.dwMajorVersion == 4)
					{
						_tcscat(atszOSVer,_T("Workstation 4.0 "));
					}
					else if(OSVerInfo.wSuiteMask & VER_SUITE_PERSONAL)
					{
						_tcscat(atszOSVer,_T("Home Edition "));
					}
					else 
					{
						_tcscat(atszOSVer,_T("Professional "));
					}
				}
				else if(OSVerInfo.wProductType == VER_NT_SERVER || OSVerInfo.wProductType == VER_NT_DOMAIN_CONTROLLER)
				{
					if(OSVerInfo.dwMajorVersion==5 && OSVerInfo.dwMinorVersion==2)
					{
						if (SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
						{
							if(OSVerInfo.wSuiteMask & VER_SUITE_DATACENTER )
							{
								_tcscat(atszOSVer,_T("Datacenter Edition for Itanium-based Systems"));
							}
							else if(OSVerInfo.wSuiteMask & VER_SUITE_ENTERPRISE )
							{
								_tcscat(atszOSVer,_T("Enterprise Edition for Itanium-based Systems"));
							}
						}
						else if(SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
						{
							if(OSVerInfo.wSuiteMask & VER_SUITE_DATACENTER)
							{
								_tcscat(atszOSVer,_T("Datacenter x64 Edition "));
							}
							else if(OSVerInfo.wSuiteMask & VER_SUITE_ENTERPRISE)
							{
								_tcscat(atszOSVer,_T("Enterprise x64 Edition "));
							}
							else 
							{
								_tcscat(atszOSVer,_T("Standard x64 Edition "));
							}
						}
						else 
						{
							if(OSVerInfo.wSuiteMask & VER_SUITE_DATACENTER)
							{
								_tcscat(atszOSVer,_T("Datacenter Edition "));
							}
							else if(OSVerInfo.wSuiteMask & VER_SUITE_ENTERPRISE)
							{
								_tcscat(atszOSVer,_T("Enterprise Edition "));
							}
							else if (OSVerInfo.wSuiteMask & VER_SUITE_BLADE)
							{
								_tcscat(atszOSVer,_T("Web Edition "));
							}
							else 
							{
								_tcscat(atszOSVer,_T("Standard Edition "));
							}
						}
					}
					else if(OSVerInfo.dwMajorVersion==5 && OSVerInfo.dwMinorVersion==0)
					{
						if(OSVerInfo.wSuiteMask & VER_SUITE_DATACENTER)
						{
							_tcscat(atszOSVer,_T("Datacenter Server "));
						}
						else if(OSVerInfo.wSuiteMask & VER_SUITE_ENTERPRISE)
						{
							_tcscat(atszOSVer,_T("Advanced Server "));
						}
						else
						{
							_tcscat(atszOSVer,_T("Server "));
						}
					}
					else
					{
						if(OSVerInfo.wSuiteMask & VER_SUITE_ENTERPRISE)
						{
							_tcscat(atszOSVer,_T("Server 4.0, Enterprise Edition "));
						}
						else
						{
							_tcscat(atszOSVer,_T("Server 4.0 "));
						}
					}
				}
			}
			else
			{
				HKEY hKey;
				TCHAR szProductType[128];
				DWORD dwBufLen = 128 * sizeof(TCHAR);

				if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"), 0, KEY_QUERY_VALUE, &hKey))
				{
					break ;
				}

				if(ERROR_SUCCESS == RegQueryValueEx(hKey,_T("ProductType"),NULL,NULL,(LPBYTE)szProductType,&dwBufLen) 
					|| dwBufLen > 128 * sizeof(TCHAR))
				{
					RegCloseKey(hKey);
					break ;
				}

				RegCloseKey(hKey);

				if(_tcsicmp(_T("WINNT"),szProductType) == 0)
				{
					_tcscat(atszOSVer,_T("Workstation "));
				}
				else if(_tcsicmp(_T("LANMANNT"),szProductType) == 0)
				{
					_tcscat(atszOSVer,_T("Server "));
				}
				else if(_tcsicmp(_T("SERVERNT"),szProductType) == 0)
				{
					_tcscat(atszOSVer,_T("Advanced Server "));
				}

				_stprintf(atszTemp,_T("%d.%d "),OSVerInfo.dwMajorVersion,OSVerInfo.dwMinorVersion);
				_tcscat(atszOSVer,atszTemp);
			}

			// 显示 Service Pack & Build 号.
			if(OSVerInfo.dwMajorVersion == 4 && 0 == _tcsicmp(OSVerInfo.szCSDVersion,_T("Service Pack 6")))
			{ 
				HKEY hKey;
				LONG lRet;

				lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Hotfix\\Q246009"),0,KEY_QUERY_VALUE,&hKey);
				if(lRet == ERROR_SUCCESS)
				{
					_stprintf(atszTemp,_T("Service Pack 6a (Build %d.%d.%d)"), 
						OSVerInfo.dwMajorVersion,OSVerInfo.dwMinorVersion,OSVerInfo.dwBuildNumber&0xFFFF);
					_tcscat(atszOSVer,atszTemp);
				}
				else 
				{
					_stprintf(atszTemp,_T("%s (Build %d.%d.%d)"), 
						OSVerInfo.szCSDVersion,OSVerInfo.dwMajorVersion,OSVerInfo.dwMinorVersion,OSVerInfo.dwBuildNumber&0xFFFF);
					_tcscat(atszOSVer,atszTemp);
				}

				RegCloseKey( hKey );
			}
			else 
			{
				_stprintf(atszTemp,_T("%s (Build %d.%d.%d)"), 
					OSVerInfo.szCSDVersion,OSVerInfo.dwMajorVersion,OSVerInfo.dwMinorVersion,OSVerInfo.dwBuildNumber&0xFFFF);
				_tcscat(atszOSVer,atszTemp);
			}
		}
		break;
	case VER_PLATFORM_WIN32_WINDOWS:
		{
			if(OSVerInfo.dwMajorVersion == 4)
			{
				if(OSVerInfo.dwMinorVersion == 0)
				{
					if(OSVerInfo.szCSDVersion[1]==_T('C') || OSVerInfo.szCSDVersion[1]==_T('B'))
					{
						_tcscpy(atszOSVer,_T("Microsoft Windows 95 OSR2"));
					}
					else
					{
						_tcscpy(atszOSVer,_T("Microsoft Windows 95"));
					}
				}
				else if(OSVerInfo.dwMinorVersion == 10)
				{
					if(OSVerInfo.szCSDVersion[1]==_T('A') || OSVerInfo.szCSDVersion[1]==_T('B'))
					{
						_tcscpy(atszOSVer,_T("Microsoft Windows 98 SE"));
					}
					else
					{
						_tcscpy(atszOSVer,_T("Microsoft Windows 98"));
					}
				}
				else if(OSVerInfo.dwMinorVersion == 90)
				{
					_tcscpy(atszOSVer,_T("Microsoft Windows ME"));
				}
				else
				{
					_tcscpy(atszOSVer,_T("Unknown System Version"));
				}
			}
			else
			{
				_tcscpy(atszOSVer,_T("Unknown System Version"));
			}
		}
		break;
	case VER_PLATFORM_WIN32s:
		{
			_tcscpy(atszOSVer,_T("Microsoft Win32s"));
		}
		break;
	default:
		{
			_tcscpy(atszOSVer,_T("Unknown System Version"));
		}
	}

	if(dwLen <= _tcslen(atszOSVer)) return FALSE ;

	_tcscpy(ptszOSVer,atszOSVer);

	return TRUE ;
}