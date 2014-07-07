/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: BugReport.cpp
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/

#include "stdafx.h"
#include "BugReport.h"
#include "DumpFile.h"
#include "ReportTool.h"

/********************************** 宏、常量 **********************************/

/********************************** 数据类型 **********************************/

/************************************ 变量 ************************************/
extern HINSTANCE g_hInstDll;

/********************************** 函数实现 **********************************/

/*********************************** 类实现 ***********************************/

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