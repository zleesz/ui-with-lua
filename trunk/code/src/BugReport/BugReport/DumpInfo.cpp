/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: DumpInfo.cpp
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/

#include "StdAfx.h"
#include "DumpInfo.h"

/********************************** �ꡢ���� **********************************/

/********************************** �������� **********************************/

/************************************ ���� ************************************/

/********************************** ����ʵ�� **********************************/

/*********************************** ��ʵ�� ***********************************/


CDumpInfo::CDumpInfo(void)
{
	m_achDumpType[0] = 0 ;
}

CDumpInfo::~CDumpInfo(void)
{
}

BOOL CDumpInfo::Dump(HANDLE hDumpFile)
{
	DWORD dwLen = strlen(m_achDumpType);
	WriteFile(hDumpFile,m_achDumpType,dwLen,&dwLen,NULL);

	char * pLine = "\r\n" ;
	dwLen = strlen(pLine);
	WriteFile(hDumpFile,pLine,dwLen,&dwLen,NULL);

	return DumpFile(hDumpFile);
}

BOOL CDumpInfo::Load(HANDLE hDumpFile)
{
	return LoadFile(hDumpFile) ;
}
