/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: DumpCpuInfo.cpp
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/

#include "StdAfx.h"
#include "DumpCpuInfo.h"
#include "DataConvert.h"
#include <AtlConv.h>

/********************************** 宏、常量 **********************************/

/********************************** 数据类型 **********************************/

/************************************ 变量 ************************************/

/********************************** 函数实现 **********************************/

/*********************************** 类实现 ***********************************/

CDumpCpuInfo::CDumpCpuInfo(PEXCEPTION_POINTERS pExceptionInfo)
{
	m_pExceptionInfo = pExceptionInfo ;
	strcpy(m_achDumpType,"[CPU Info]");
}

CDumpCpuInfo::~CDumpCpuInfo(void)
{
}

BOOL CDumpCpuInfo::DumpFile(HANDLE hDumpFile)
{
	USES_CONVERSION;

	CONTEXT * pContext = m_pExceptionInfo->ContextRecord ;

	DWORD64 dwEAX = pContext->Eax ;
	DWORD64 dwEBX = pContext->Ebx ;
	DWORD64 dwECX = pContext->Ecx ;
	DWORD64 dwEDX = pContext->Edx ;
	DWORD64 dwEDI = pContext->Edi ;
	DWORD64 dwESI = pContext->Esi ;
	DWORD64 dwESP = pContext->Esp ;
	DWORD64 dwEBP = pContext->Ebp ;
	DWORD64 dwEIP = pContext->Eip ;
	DWORD64 dwEFlags = pContext->EFlags ;
	DWORD64 dwCS = pContext->SegCs ;
	DWORD64 dwDS = pContext->SegDs ;
	DWORD64 dwES = pContext->SegEs ;
	DWORD64 dwFS = pContext->SegFs ;
	DWORD64 dwGS = pContext->SegGs ;
	DWORD64 dwSS = pContext->SegSs ;

	TCHAR atszEAX[32];
	TCHAR atszEBX[32];
	TCHAR atszECX[32];
	TCHAR atszEDX[32];
	TCHAR atszEDI[32];
	TCHAR atszESI[32];
	TCHAR atszEBP[32];
	TCHAR atszESP[32];
	TCHAR atszEIP[32];
	TCHAR atszEFlags[32];
	TCHAR atszCS[32];
	TCHAR atszDS[32];
	TCHAR atszES[32];
	TCHAR atszFS[32];
	TCHAR atszGS[32];
	TCHAR atszSS[32];

	CDataConvert::AddressToString(dwEAX,atszEAX,TRUE,8);
	CDataConvert::AddressToString(dwEBX,atszEBX,TRUE,8);
	CDataConvert::AddressToString(dwECX,atszECX,TRUE,8);
	CDataConvert::AddressToString(dwEDX,atszEDX,TRUE,8);
	CDataConvert::AddressToString(dwEDI,atszEDI,TRUE,8);
	CDataConvert::AddressToString(dwESI,atszESI,TRUE,8);
	CDataConvert::AddressToString(dwEBP,atszEBP,TRUE,8);
	CDataConvert::AddressToString(dwESP,atszESP,TRUE,8);
	CDataConvert::AddressToString(dwEIP,atszEIP,TRUE,8);
	CDataConvert::AddressToString(dwEFlags,atszEFlags,TRUE,8);
	CDataConvert::AddressToString(dwCS,atszCS,TRUE,8);
	CDataConvert::AddressToString(dwDS,atszDS,TRUE,8);
	CDataConvert::AddressToString(dwES,atszES,TRUE,8);
	CDataConvert::AddressToString(dwFS,atszFS,TRUE,8);
	CDataConvert::AddressToString(dwGS,atszGS,TRUE,8);
	CDataConvert::AddressToString(dwSS,atszSS,TRUE,8);

	TCHAR atszBuf[512];
	DWORD dwLen = 0 ;

	_stprintf(atszBuf,_T("EAX = %s , EBX = %s\r\nECX = %s , EDX = %s\r\nEDI = %s , ESI = %s\r\nEBP = %s , ESP = %s\r\nEIP = %s , EFlags = %s\r\nCS  = %s , DS  = %s\r\nES  = %s , FS  = %s\r\nGS  = %s , SS  = %s\r\n\r\n"),
						 atszEAX,atszEBX,atszECX,atszEDX,atszEDI,atszESI,atszEBP,atszESP,atszEIP,atszEFlags,
						 atszCS,atszDS,atszES,atszFS,atszGS,atszSS
						 );

	char * pTemp = T2A(atszBuf);
	dwLen = strlen(pTemp);

	::WriteFile(hDumpFile,pTemp,dwLen,&dwLen,NULL);

	return TRUE ;
}

BOOL CDumpCpuInfo::LoadFile(HANDLE hDumpFile)
{
	return FALSE;
}