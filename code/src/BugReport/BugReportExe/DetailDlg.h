/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: DetailDlg.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: ��ʾ�쳣��ϸ��Ϣ�ĶԻ���
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __DETAILDLG_H_5746788E_8629_431E_A369_C4EB62CB8338__
#define __DETAILDLG_H_5746788E_8629_431E_A369_C4EB62CB8338__

/**********************************  ͷ�ļ� ************************************/
#include "resource.h"

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/********************************** �ඨ�� ***********************************/

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


