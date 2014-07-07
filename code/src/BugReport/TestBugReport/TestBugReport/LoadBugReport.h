#pragma once

#include <Windows.h>

class CLoadBugReport
{
private:
	CLoadBugReport()
	{
		m_hInstDll = NULL ;
	}

	typedef int (* PFN_BootReport)() ;
	typedef int (* PFN_StopReport)() ;

public:
	~CLoadBugReport()
	{
		if(m_hInstDll!=NULL)
		{
			FreeLibrary(m_hInstDll);
			m_hInstDll = NULL ;
		}
	}

	static CLoadBugReport& Instance()
	{
		static CLoadBugReport report ;
		return report ;
	}

	BOOL LoadLibrary(LPCTSTR ptszDllPath=NULL)
	{
		if(m_hInstDll!=NULL) return FALSE ;

		if(ptszDllPath!=NULL)
		{
			m_hInstDll = ::LoadLibrary(ptszDllPath);
		}
		else
		{
			TCHAR atszPath[512];
			::GetModuleFileName(NULL,atszPath,512);

			TCHAR * pTemp = _tcsrchr(atszPath,_T('\\'));
			if(!pTemp) pTemp = _tcsrchr(atszPath,_T('/'));
			if(pTemp) *pTemp = 0 ;

			_tcscat(atszPath,_T("\\BugReport.dll"));

			m_hInstDll = ::LoadLibrary(atszPath);
		}

		if(m_hInstDll==NULL) return FALSE ;

		m_pfnBootReport = (PFN_BootReport)::GetProcAddress(m_hInstDll,"BootReport");
		m_pfnStopReport = (PFN_StopReport)::GetProcAddress(m_hInstDll,"StopReport");

		if(m_pfnBootReport==NULL || m_pfnStopReport==NULL)
		{
			FreeLibrary(m_hInstDll);
			m_hInstDll = NULL ;

			return FALSE ;
		}

		return TRUE ;
	}

	BOOL BootReport()
	{
		if(m_hInstDll==NULL || m_pfnBootReport==NULL) return FALSE ;

		if(0 != m_pfnBootReport()) return FALSE ;

		return TRUE ;
	}

	BOOL StopReport()
	{
		if(m_hInstDll==NULL || m_pfnStopReport==NULL) return FALSE ;

		if(0 != m_pfnStopReport()) return FALSE ;

		return TRUE ;
	}

private:
	HINSTANCE		m_hInstDll ;
	PFN_BootReport	m_pfnBootReport ;
	PFN_StopReport	m_pfnStopReport ;
};