/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: BugReport.cpp
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/

#include "stdafx.h"
#include "BugReport.h"
#include "DumpFile.h"
#include "ReportTool.h"

/********************************** �ꡢ���� **********************************/

/********************************** �������� **********************************/

/************************************ ���� ************************************/
extern HINSTANCE g_hInstDll;

/********************************** ����ʵ�� **********************************/

/*********************************** ��ʵ�� ***********************************/

CBugReport::CBugReport(void)
{
	m_pfnFilter = NULL ;

	InitPath();
}

CBugReport::~CBugReport(void)
{
}

CBugReport& CBugReport::Instance()
{
	static CBugReport bugReport ;
	return bugReport ;
}

void CBugReport::InitPath()
{
	TCHAR atszPath[512] ;
	::GetModuleFileName(g_hInstDll,atszPath,512);

	TCHAR * pTemp = _tcsrchr(atszPath,_T('\\'));
	if(!pTemp) _tcsrchr(atszPath,_T('/'));
	if(pTemp) *pTemp = 0 ;

	_stprintf(m_atszReportFile,_T("%s\\BugDetail.dat"),atszPath);
	_stprintf(m_atszReportTool,_T("%s\\BugReport.exe"),atszPath);
}

BOOL CBugReport::SetUnhandleExceptionFilter()
{
	m_pfnFilter = ::SetUnhandledExceptionFilter(UnhandledExceptionFilter);
	return TRUE ;
}

BOOL CBugReport::ResetUnhandleExceptionFilter()
{
	m_pfnFilter = ::SetUnhandledExceptionFilter(m_pfnFilter);
	return TRUE ;
}

LONG WINAPI CBugReport::UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	return CBugReport::Instance().DumpFile(pExceptionInfo);
}

LONG CBugReport::DumpFile(PEXCEPTION_POINTERS pExceptionInfo)
{
	CDumpFile dump(m_atszReportFile) ;
	if(!dump.DumpInfo(pExceptionInfo))
	{
		return EXCEPTION_EXECUTE_HANDLER;
	}
	dump.Close();

	CReportTool tool(m_atszReportTool) ;
	tool.BootReportTool(m_atszReportFile);

	return EXCEPTION_EXECUTE_HANDLER ;
}