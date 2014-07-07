/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: FileVersionInfo.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __FILEVERSIONINFO_H_D0AFC9D4_D555_4F90_A890_FA5059B93DA5__
#define __FILEVERSIONINFO_H_D0AFC9D4_D555_4F90_A890_FA5059B93DA5__

/**********************************  ͷ�ļ� ************************************/

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/********************************** �ඨ�� ***********************************/

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

