/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: DumpModuleInfo.cpp
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/

#include "StdAfx.h"
#include "DumpModuleInfo.h"
#include "ModuleList.h"
#include "DataConvert.h"
#include <AtlConv.h>
#include "FileVersionInfo.h"

/********************************** �ꡢ���� **********************************/

/********************************** �������� **********************************/

/************************************ ���� ************************************/

/********************************** ����ʵ�� **********************************/

/*********************************** ��ʵ�� ***********************************/

CDumpModuleInfo::CDumpModuleInfo(void)
{
	strcpy(m_achDumpType,"[Module list Info]");
}

CDumpModuleInfo::~CDumpModuleInfo(void)
{
}

BOOL CDumpModuleInfo::DumpFile(HANDLE hDumpFile)
{
	ModuleInfoList * pModuleList = CModuleList::Instance().GetModuleList();

	TCHAR atszBuf[512];
	DWORD dwLen = 0 ;

	USES_CONVERSION ;

	for(ModuleInfoIter iter=pModuleList->begin();iter!=pModuleList->end();iter++)
	{
		TCHAR atszAddr[32];
		CDataConvert::AddressToString((DWORD64)((*iter)->modBaseAddr),atszAddr,TRUE,8);

		CFileVersionInfo FileVerion((*iter)->szExePath);

		TCHAR atszFileInfo[64];
		if(!FileVerion.GetProductVersion(atszFileInfo,64))
		{
			_stprintf(atszBuf,_T("[%s] %s\r\n"),atszAddr,(*iter)->szExePath);
		}
		else
		{
			_stprintf(atszBuf,_T("[%s] %s (%s)\r\n"),atszAddr,(*iter)->szExePath,atszFileInfo);
		}

		char * pTemp = T2A(atszBuf);
		dwLen = strlen(pTemp);

		::WriteFile(hDumpFile,pTemp,dwLen,&dwLen,NULL);
	}

	char * pLine = "\r\n" ;
	dwLen = strlen(pLine);
	::WriteFile(hDumpFile,pLine,dwLen,&dwLen,NULL);

	return TRUE ;
}

BOOL CDumpModuleInfo::LoadFile(HANDLE hDumpFile)
{
	return TRUE ;
}