/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: ReportTool.cpp
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/

#include "StdAfx.h"
#include "ReportTool.h"
#include <stdio.h>

/********************************** 宏、常量 **********************************/

/********************************** 数据类型 **********************************/

/************************************ 变量 ************************************/

/********************************** 函数实现 **********************************/

/*********************************** 类实现 ***********************************/

CReportTool::CReportTool(LPCTSTR ptszTool)
{
	if(ptszTool)
	{
		_tcscpy(m_atszTool,ptszTool);
	}
	else
	{
		m_atszTool[0] = 0 ;
	}
}

CReportTool::~CReportTool(void)
{
}

void CReportTool::BootReportTool(LPCTSTR ptszParam)
{
	if(ptszParam==NULL) return ;

	STARTUPINFO si ;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) ); 
    si.cb = sizeof(si); 
    ZeroMemory( &pi, sizeof(pi) );

	TCHAR atszCmdline[1024];
	_stprintf(atszCmdline,_T("\"%s\" %s"),m_atszTool,ptszParam);

	if(CreateProcess(NULL,atszCmdline,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}
}