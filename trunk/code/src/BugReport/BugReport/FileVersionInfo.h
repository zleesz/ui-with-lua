/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: FileVersionInfo.h
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __FILEVERSIONINFO_H_D0AFC9D4_D555_4F90_A890_FA5059B93DA5__
#define __FILEVERSIONINFO_H_D0AFC9D4_D555_4F90_A890_FA5059B93DA5__

/**********************************  头文件 ************************************/

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/********************************** 类定义 ***********************************/

class CFileVersionInfo
{
public:
	CFileVersionInfo(LPCTSTR ptszFileName);
	~CFileVersionInfo(void);

	BOOL GetComments(LPTSTR ptszName,DWORD dwLen);
	BOOL GetInternalName(LPTSTR ptszName,DWORD dwLen);
	BOOL GetProductName(LPTSTR ptszName,DWORD dwLen);
	BOOL GetCompanyName(LPTSTR ptszName,DWORD dwLen);
	BOOL GetLegalCopyright(LPTSTR ptszName,DWORD dwLen);
	BOOL GetProductVersion(LPTSTR ptszName,DWORD dwLen);
	BOOL GetFileDescription(LPTSTR ptszName,DWORD dwLen);
	BOOL GetLegalTrademarks(LPTSTR ptszName,DWORD dwLen);
	BOOL GetPrivateBuild(LPTSTR ptszName,DWORD dwLen);
	BOOL GetFileVersion(LPTSTR ptszName,DWORD dwLen);
	BOOL GetOriginalFilename(LPTSTR ptszName,DWORD dwLen);
	BOOL GetSpecialBuild(LPTSTR ptszName,DWORD dwLen);

private:
	BOOL InitVersionInfo();
	BOOL GetPrivateData(LPTSTR ptszName,DWORD dwLen,LPCTSTR ptszString);

private:
	TCHAR	m_atszFileName[512];
	BOOL	m_bInitSuccess ;

	BYTE *	m_pbyVersionData ;
	WORD	m_wLanguage ;
	WORD	m_wCodePage ;
};


#ifdef  __cplusplus
}
#endif
#endif // end of __FILEVERSIONINFO_H_D0AFC9D4_D555_4F90_A890_FA5059B93DA5__

