/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: DumpStackInfo.cpp
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/

#include "StdAfx.h"
#include <dbghelp.h>
#include "DumpStackInfo.h"
#include "ModuleList.h"
#include "DataConvert.h"
#include <AtlConv.h>

/********************************** 宏、常量 **********************************/

/********************************** 数据类型 **********************************/

/************************************ 变量 ************************************/

/********************************** 函数实现 **********************************/

/*********************************** 类实现 ***********************************/

CDumpStackInfo::CDumpStackInfo(PEXCEPTION_POINTERS pExceptionInfo)
{
	m_pExceptionInfo = pExceptionInfo ;
	strcpy(m_achDumpType,"[Stack Info]");
}

CDumpStackInfo::~CDumpStackInfo(void)
{
}

BOOL CDumpStackInfo::DumpFile(HANDLE hDumpFile)
{
	CONTEXT * pContext = m_pExceptionInfo->ContextRecord;

	HANDLE hCurrentProcess = ::GetCurrentProcess();
	HANDLE hCurrentThread  = ::GetCurrentThread();

	STACKFRAME64 sStackFrame;
	memset(&sStackFrame,0,sizeof(STACKFRAME64));

	sStackFrame.AddrPC.Offset	= pContext->Eip;
	sStackFrame.AddrPC.Mode		= AddrModeFlat;
	sStackFrame.AddrStack.Offset= pContext->Esp;
	sStackFrame.AddrStack.Mode	= AddrModeFlat;
	sStackFrame.AddrFrame.Offset= pContext->Ebp;
	sStackFrame.AddrFrame.Mode	= AddrModeFlat;

	USES_CONVERSION ;

	TCHAR atszCallbackBuffer[1024];
	DWORD dwLen = 0;

	SymInitialize(GetCurrentProcess(),NULL,TRUE);

	while(StackWalk64(MACHINE_TYPE,hCurrentProcess,hCurrentThread,&sStackFrame,pContext,0,0,0,0))
	{
		if( sStackFrame.AddrFrame.Offset == 0 )
		{
		   break;
		}

		BYTE abySymBuffer[sizeof(SYMBOL_INFO) + 512];
		PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)abySymBuffer;

		pSymbol->SizeOfStruct = sizeof(abySymBuffer);
		pSymbol->MaxNameLen = 512;

		// 得到PC对应的函数名(pSymbol->Name).
		if(!SymFromAddr(hCurrentProcess,sStackFrame.AddrPC.Offset,0,pSymbol))
		{
			pSymbol->Name[0] = 0 ;
		}

		PMODULEENTRY32 pModuleInfo = CModuleList::Instance().QueryModuleInfoByAddress(sStackFrame.AddrPC.Offset);
		if(!pModuleInfo)
		{
			TCHAR atszAddr[32];
			CDataConvert::AddressToString(sStackFrame.AddrPC.Offset,atszAddr,TRUE,8);

			_stprintf(atszCallbackBuffer,_T("[%s]  UnkownModule  %s\r\n"),atszAddr,A2T(pSymbol->Name));
		}
		else
		{
			TCHAR atszBaseAddr[32];
			CDataConvert::AddressToString((DWORD64)(pModuleInfo->modBaseAddr),atszBaseAddr,TRUE,8);

			TCHAR atszOffsetAddr[32];
			CDataConvert::AddressToString(sStackFrame.AddrPC.Offset-(DWORD64)(pModuleInfo->modBaseAddr),atszOffsetAddr,FALSE,0);

			_stprintf(atszCallbackBuffer,_T("[%s][%s]  %s  %s\r\n"),atszBaseAddr,atszOffsetAddr,pModuleInfo->szModule,	A2T(pSymbol->Name));
		}

		char * pTemp = T2A(atszCallbackBuffer);
		dwLen = strlen(pTemp);
		::WriteFile(hDumpFile,pTemp,dwLen,&dwLen,NULL);
	}

	SymCleanup(GetCurrentProcess());

	char * pLine = "\r\n" ;
	dwLen = strlen(pLine);
	::WriteFile(hDumpFile,pLine,dwLen,&dwLen,NULL);

	return TRUE ;
}

BOOL CDumpStackInfo::LoadFile(HANDLE hDumpFile)
{
	return TRUE ;
}