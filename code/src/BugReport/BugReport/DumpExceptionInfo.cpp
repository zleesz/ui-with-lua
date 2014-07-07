/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: DumpExceptionInfo.cpp
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/

#include "StdAfx.h"
#include "DumpExceptionInfo.h"
#include "DataConvert.h"
#include <AtlConv.h>

/********************************** �ꡢ���� **********************************/

/********************************** �������� **********************************/

/************************************ ���� ************************************/

/********************************** ����ʵ�� **********************************/

/*********************************** ��ʵ�� ***********************************/

CDumpExceptionInfo::CDumpExceptionInfo(PEXCEPTION_POINTERS pExceptionInfo)
{
	m_pExceptionInfo = pExceptionInfo ;
	strcpy(m_achDumpType,"[Exception Info]");
}

CDumpExceptionInfo::~CDumpExceptionInfo(void)
{
}

BOOL CDumpExceptionInfo::DumpFile(HANDLE hDumpFile)
{
	USES_CONVERSION;

	PEXCEPTION_RECORD pExceptionRec = m_pExceptionInfo->ExceptionRecord ;

	DWORD64 dwCode64 = (DWORD64)pExceptionRec->ExceptionCode ;
	DWORD64 dwFlag64 = (DWORD64)pExceptionRec->ExceptionFlags ;
	DWORD64 dwAddr64 = (DWORD64)pExceptionRec->ExceptionAddress ;

	TCHAR atszCode[32];
	TCHAR atszFlag[32];
	TCHAR atszAddr[32];

	CDataConvert::AddressToString(dwCode64,atszCode,TRUE,8);
	CDataConvert::AddressToString(dwFlag64,atszFlag,TRUE,8);
	CDataConvert::AddressToString(dwAddr64,atszAddr,TRUE,8);

	TCHAR atszBuf[128];
	
	_stprintf(atszBuf,_T("ExceptionCode : %s\r\nExceptionFlag : %s\r\nExceptionAddr : %s\r\n\r\n"),
		atszCode,atszFlag,atszAddr);

	char * pTemp = T2A(atszBuf);
	DWORD dwLen = strlen(pTemp);

	::WriteFile(hDumpFile,pTemp,dwLen,&dwLen,NULL);

	return TRUE ;
}

BOOL CDumpExceptionInfo::LoadFile(HANDLE hDumpFile)
{
	return FALSE ;
}