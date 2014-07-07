/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: DetailDlg.h
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 显示异常详细信息的对话框
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __DETAILDLG_H_5746788E_8629_431E_A369_C4EB62CB8338__
#define __DETAILDLG_H_5746788E_8629_431E_A369_C4EB62CB8338__

/**********************************  头文件 ************************************/
#include "resource.h"

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/********************************** 类定义 ***********************************/

class CDetailDlg : public CDialogImpl<CDetailDlg>,public CWinDataExchange<CDetailDlg>
{
public:
	CEdit	m_edit;
	enum { IDD = IDD_DLG_DETAIL };

public:
	CDetailDlg(void);
	~CDetailDlg(void);

	BEGIN_MSG_MAP(CDetailDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()
	
	BEGIN_DDX_MAP(CDetailDlg)
		DDX_CONTROL_HANDLE(IDC_EDIT_CONTEXT, m_edit)
	END_DDX_MAP();
	
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	void ShowContext();
};

#ifdef  __cplusplus
}
#endif
#endif // end of __DETAILDLG_H_5746788E_8629_431E_A369_C4EB62CB8338__


