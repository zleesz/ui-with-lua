/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: DetailDlg.cpp
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: ��ʾ�쳣��ϸ��Ϣ�ĶԻ���
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#include "StdAfx.h"
#include "DetailDlg.h"
#include <AtlConv.h>

/********************************** �ꡢ���� **********************************/

/********************************** �������� **********************************/

/************************************ ���� ************************************/
extern TCHAR g_ptszBugDetail[512] ;

/********************************** ����ʵ�� **********************************/

/*********************************** ��ʵ�� ***********************************/

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
		_stprintf(p,_T("���ļ� %s ʧ�ܣ�Err=%d"),g_ptszBugDetail,GetLastError());
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