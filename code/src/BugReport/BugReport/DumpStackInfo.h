/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: DumpStackInfo.h
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __DUMPSTACKINFO_H_B6EA8136_16F6_4022_94AC_0414A2314B65__
#define __DUMPSTACKINFO_H_B6EA8136_16F6_4022_94AC_0414A2314B65__

/**********************************  头文件 ************************************/
#include "dumpinfo.h"

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/********************************** 类定义 ***********************************/

class CDumpStackInfo :
	public CDumpInfo
{
public:
	CDumpStackInfo(PEXCEPTION_POINTERS pExceptionInfo);
	~CDumpStackInfo(void);

protected:
	BOOL DumpFile(HANDLE hDumpFile);
	BOOL LoadFile(HANDLE hDumpFile);

private:
	PEXCEPTION_POINTERS m_pExceptionInfo ;
};


#ifdef  __cplusplus
}
#endif
#endif // end of __DUMPSTACKINFO_H_B6EA8136_16F6_4022_94AC_0414A2314B65__



