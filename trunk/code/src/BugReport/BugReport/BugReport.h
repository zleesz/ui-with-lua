/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: BugReport.h
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __BUGREPORT_H_812607D9_FB87_4F1A_BAA0_6D2F9AE4F14C__
#define __BUGREPORT_H_812607D9_FB87_4F1A_BAA0_6D2F9AE4F14C__

/**********************************  头文件 ************************************/

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/********************************** 类定义 ***********************************/

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


