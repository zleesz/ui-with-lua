/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: DumpFile.cpp
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/

#include "StdAfx.h"
#include "DumpFile.h"

#include "ModuleList.h"
#include "DumpSystemInfo.h"
#include "DumpExceptionInfo.h"
#include "DumpStackInfo.h"
#include "DumpModuleInfo.h"
#include "DumpCpuInfo.h"

/********************************** �ꡢ���� **********************************/

/********************************** �������� **********************************/

/************************************ ���� ************************************/

/********************************** ����ʵ�� **********************************/

/*********************************** ��ʵ�� ***********************************/

CDumpFile::CDumpFile(LPCTSTR ptszFile)
{
	m_hDumpFile = ::CreateFile(ptszFile,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
}

CDumpFile::~CDumpFile(void)
{
	Close();
}

BOOL CDumpFile::DumpInfo(PEXCEPTION_POINTERS pExceptionInfo)
{
	if(m_hDumpFile==INVALID_HANDLE_VALUE) return FALSE ;

	CModuleList::Instance().UpdateModuleList();

	CDumpSystemInfo dumpSystem ;
	dumpSystem.Dump(m_hDumpFile);

	CDumpExceptionInfo dumpException(pExceptionInfo); ;
	dumpException.Dump(m_hDumpFile);

	CDumpCpuInfo dumpCPU(pExceptionInfo);
	dumpCPU.Dump(m_hDumpFile);

	CDumpStackInfo dumpStack(pExceptionInfo);
	dumpStack.Dump(m_hDumpFile);

	CDumpModuleInfo dumpModule ;
	dumpModule.Dump(m_hDumpFile);

	return TRUE ;
}

void CDumpFile::Close()
{
	if(m_hDumpFile!=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDumpFile);
		m_hDumpFile = INVALID_HANDLE_VALUE ;
	}
}