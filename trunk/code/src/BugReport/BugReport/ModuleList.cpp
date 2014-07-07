/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: ModuleList.cpp
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/

#include "StdAfx.h"
#include "ModuleList.h"

/********************************** 宏、常量 **********************************/

/********************************** 数据类型 **********************************/

/************************************ 变量 ************************************/

/********************************** 函数实现 **********************************/

/*********************************** 类实现 ***********************************/


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
