/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: DumpInfo.cpp
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/

#include "StdAfx.h"
#include "DumpInfo.h"

/********************************** 宏、常量 **********************************/

/********************************** 数据类型 **********************************/

/************************************ 变量 ************************************/

/********************************** 函数实现 **********************************/

/*********************************** 类实现 ***********************************/


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
