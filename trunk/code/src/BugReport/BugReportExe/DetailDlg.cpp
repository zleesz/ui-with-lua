/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: DetailDlg.cpp
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 显示异常详细信息的对话框
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#include "StdAfx.h"
#include "DetailDlg.h"
#include <AtlConv.h>

/********************************** 宏、常量 **********************************/

/********************************** 数据类型 **********************************/

/************************************ 变量 ************************************/
extern TCHAR g_ptszBugDetail[512] ;

/********************************** 函数实现 **********************************/

/*********************************** 类实现 ***********************************/

CDetailDlg::CDetailDlg(void)
{
}

CDetailDlg::~CDetailDlg(void)
{
}

LRESULT CDetailDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	DoDataExchange(false);
	ShowContext();

	return TRUE;
}

LRESULT CDetailDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0 ;
}

LRESULT CDetailDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0 ;
}

void CDetailDlg::ShowContext()
{
	HANDLE hFile = CreateFile(g_ptszBugDetail,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
	{
		TCHAR p[512];
		_stprintf(p,_T("打开文件 %s 失败，Err=%d"),g_ptszBugDetail,GetLastError());
		m_edit.AppendText(p);
		return ;
	}

	char achBuffer[1025];
	DWORD dwLen = 1024 ;

	USES_CONVERSION;

	while(::ReadFile(hFile,achBuffer,1024,&dwLen,NULL) && dwLen!=0)
	{
		achBuffer[dwLen] = 0 ;

		m_edit.AppendText(A2T(achBuffer));
	}

	CloseHandle(hFile);
}