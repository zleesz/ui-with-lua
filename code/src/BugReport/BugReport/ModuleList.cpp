/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: ModuleList.cpp
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/

#include "StdAfx.h"
#include "ModuleList.h"

/********************************** �ꡢ���� **********************************/

/********************************** �������� **********************************/

/************************************ ���� ************************************/

/********************************** ����ʵ�� **********************************/

/*********************************** ��ʵ�� ***********************************/


CModuleList::CModuleList(void)
{
}

CModuleList::~CModuleList(void)
{
}

CModuleList& CModuleList::Instance()
{
	static CModuleList mlst ;
	return mlst ;
}

BOOL CModuleList::UpdateModuleList()
{
	ClearModuleInfo();

	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ::GetCurrentProcessId());

	MODULEENTRY32 me32 ;
	me32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hModuleSnap, &me32)) 
	{ 
		do 
		{
			PMODULEENTRY32 pInfo = new MODULEENTRY32 ;
			memcpy(pInfo,&me32,sizeof(MODULEENTRY32));

			m_lstModuleInfo.push_back(pInfo);
		} 
		while (Module32Next(hModuleSnap, &me32)); 
	}

	CloseHandle(hModuleSnap);

	return TRUE ;
}

PMODULEENTRY32 CModuleList::QueryModuleInfoByAddress(DWORD64 dwAddress64)
{
	ModuleInfoIter iter1 = m_lstModuleInfo.begin();
	ModuleInfoIter iter2 = m_lstModuleInfo.end();
	ModuleInfoIter iter ;

	for(iter=iter1;iter!=iter2;iter++)
	{
		DWORD64 dwBaseAddr = (DWORD64)((*iter)->modBaseAddr) ;
		DWORD64 dwEndAddr  = (DWORD64)((*iter)->modBaseSize) + dwBaseAddr ;
		if(dwAddress64 >= dwBaseAddr && dwAddress64 < dwEndAddr)
		{
			return (*iter);
		}
	}

	return NULL ;
}

void CModuleList::ClearModuleInfo()
{
	ModuleInfoIter iter1 = m_lstModuleInfo.begin();
	ModuleInfoIter iter2 = m_lstModuleInfo.end();
	ModuleInfoIter iter ;

	for(iter=iter1;iter!=iter2;iter++)
	{
		delete (*iter) ;
	}

	m_lstModuleInfo.clear();
}

ModuleInfoList * CModuleList::GetModuleList()
{
	return &m_lstModuleInfo ;
}
