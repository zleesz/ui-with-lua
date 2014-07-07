/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: ReportTool.cpp
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/

#include "StdAfx.h"
#include "ReportTool.h"
#include <stdio.h>

/********************************** �ꡢ���� **********************************/

/********************************** �������� **********************************/

/************************************ ���� ************************************/

/********************************** ����ʵ�� **********************************/

/*********************************** ��ʵ�� ***********************************/

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