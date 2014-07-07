/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: BugReport.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __BUGREPORT_H_812607D9_FB87_4F1A_BAA0_6D2F9AE4F14C__
#define __BUGREPORT_H_812607D9_FB87_4F1A_BAA0_6D2F9AE4F14C__

/**********************************  ͷ�ļ� ************************************/

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/********************************** �ඨ�� ***********************************/

class CBugReport
{
private:
	CBugReport(void);

public:
	~CBugReport(void);

	static CBugReport& Instance();

	BOOL SetUnhandleExceptionFilter();
	BOOL ResetUnhandleExceptionFilter();

private:
	static LONG WINAPI UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);
	LONG DumpFile(PEXCEPTION_POINTERS pExceptionInfo);

private:
	void InitPath();

private:
	LPTOP_LEVEL_EXCEPTION_FILTER m_pfnFilter ;
	TCHAR	m_atszReportFile[512] ;
	TCHAR	m_atszReportTool[512] ;
};


#ifdef  __cplusplus
}
#endif
#endif // end of __BUGREPORT_H_812607D9_FB87_4F1A_BAA0_6D2F9AE4F14C__


