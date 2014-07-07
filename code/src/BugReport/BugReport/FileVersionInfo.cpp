/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: FileVersionInfo.cpp
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#include "StdAfx.h"
#include "FileVersionInfo.h"

#pragma comment(lib,"Version.lib")

/********************************** �ꡢ���� **********************************/

/********************************** �������� **********************************/

/************************************ ���� ************************************/

/********************************** ����ʵ�� **********************************/

/*********************************** ��ʵ�� ***********************************/

CFileVersionInfo::CFileVersionInfo(LPCTSTR ptszFileName)
{
	if(ptszFileName)
	{
		_tcscpy(m_atszFileName,ptszFileName);
	}
	else
	{
		m_atszFileName[0] = 0 ;
	}

	m_pbyVersionData = NULL ;
	m_bInitSuccess = InitVersionInfo();
}

CFileVersionInfo::~CFileVersionInfo(void)
{
	if(m_pbyVersionData == NULL)
	{
		delete [] m_pbyVersionData ;
		m_pbyVersionData = NULL ;
	}
}

BOOL CFileVersionInfo::InitVersionInfo()
{  
	int nVersionLen = ::GetFileVersionInfoSize(m_atszFileName,NULL);   
	if(nVersionLen <= 0)   
	{   
		return   FALSE;   
	}

	m_pbyVersionData = new BYTE[nVersionLen];
	if(!m_pbyVersionData)
	{
		return FALSE ;
	}
    
	if(!::GetFileVersionInfo(m_atszFileName,NULL,nVersionLen,m_pbyVersionData))
	{
		delete[] m_pbyVersionData;
		m_pbyVersionData = NULL ;
		return FALSE;   
	}  

	UINT uTranslateBytes;   
	DWORD * pdwData = NULL ;
	
	if(!::VerQueryValue(m_pbyVersionData,_T("\\VarFileInfo\\Translation"),(LPVOID*)&pdwData,&uTranslateBytes) || !pdwData)
	{
		delete[] m_pbyVersionData;
		m_pbyVersionData = NULL ;
		return FALSE;
	}

	m_wLanguage = (WORD)((*pdwData) & 0xFFFF);
	m_wCodePage = (WORD)(((*pdwData)>>16) & 0xFFFF);

	return TRUE ;
}

BOOL CFileVersionInfo::GetPrivateData(LPTSTR ptszName,DWORD dwLen,LPCTSTR ptszString)
{
	if(!m_bInitSuccess) return FALSE ;

	TCHAR atszSubBlock[256];
	_stprintf(atszSubBlock,_T("\\StringFileInfo\\%04x%04x\\%s"),m_wLanguage,m_wCodePage,ptszString);

	TCHAR * ptszBuffer = NULL ;
	UINT dwBufferLen = 0 ;
	if(!::VerQueryValue(m_pbyVersionData,atszSubBlock,(LPVOID*)&ptszBuffer,&dwBufferLen) || !ptszBuffer)
	{
		return FALSE ;
	}

	if(dwLen <= _tcslen(ptszBuffer))
	{
		return FALSE ;
	}

	_tcscpy(ptszName,ptszBuffer);

	return TRUE ;
}

BOOL CFileVersionInfo::GetComments(LPTSTR ptszName,DWORD dwLen)
{
	return GetPrivateData(ptszName,dwLen,_T("Comments"));
}

BOOL CFileVersionInfo::GetInternalName(LPTSTR ptszName,DWORD dwLen)
{
	return GetPrivateData(ptszName,dwLen,_T("InternalName"));
}

BOOL CFileVersionInfo::GetProductName(LPTSTR ptszName,DWORD dwLen)
{
	return GetPrivateData(ptszName,dwLen,_T("ProductName"));
}

BOOL CFileVersionInfo::GetCompanyName(LPTSTR ptszName,DWORD dwLen)
{
	return GetPrivateData(ptszName,dwLen,_T("CompanyName"));
}

BOOL CFileVersionInfo::GetLegalCopyright(LPTSTR ptszName,DWORD dwLen)
{
	return GetPrivateData(ptszName,dwLen,_T("LegalCopyright"));
}

BOOL CFileVersionInfo::GetProductVersion(LPTSTR ptszName,DWORD dwLen)
{
	return GetPrivateData(ptszName,dwLen,_T("ProductVersion"));
}

BOOL CFileVersionInfo::GetFileDescription(LPTSTR ptszName,DWORD dwLen)
{
	return GetPrivateData(ptszName,dwLen,_T("FileDescription"));
}

BOOL CFileVersionInfo::GetLegalTrademarks(LPTSTR ptszName,DWORD dwLen)
{
	return GetPrivateData(ptszName,dwLen,_T("LegalTrademarks"));
}

BOOL CFileVersionInfo::GetPrivateBuild(LPTSTR ptszName,DWORD dwLen)
{
	return GetPrivateData(ptszName,dwLen,_T("PrivateBuild"));
}

BOOL CFileVersionInfo::GetFileVersion(LPTSTR ptszName,DWORD dwLen)
{
	return GetPrivateData(ptszName,dwLen,_T("FileVersion"));
}

BOOL CFileVersionInfo::GetOriginalFilename(LPTSTR ptszName,DWORD dwLen)
{
	return GetPrivateData(ptszName,dwLen,_T("OriginalFilename"));
}

BOOL CFileVersionInfo::GetSpecialBuild(LPTSTR ptszName,DWORD dwLen)
{
	return GetPrivateData(ptszName,dwLen,_T("SpecialBuild"));
}

