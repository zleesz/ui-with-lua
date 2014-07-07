/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: DumpCpuInfo.h
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __DUMPCPUINFO_H_56F5D256_5500_4D5D_A672_84D752689D37__
#define __DUMPCPUINFO_H_56F5D256_5500_4D5D_A672_84D752689D37__

/**********************************  头文件 ************************************/
#include "dumpinfo.h"

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/********************************** 类定义 ***********************************/

class CDumpCpuInfo :
	public CDumpInfo
{
public:
	CDumpCpuInfo(PEXCEPTION_POINTERS pExceptionInfo);
	~CDumpCpuInfo(void);

protected:
	BOOL DumpFile(HANDLE hDumpFile);
	BOOL LoadFile(HANDLE hDumpFile);

private:
	PEXCEPTION_POINTERS m_pExceptionInfo ;
};


#ifdef  __cplusplus
}
#endif
#endif // end of __DUMPCPUINFO_H_56F5D256_5500_4D5D_A672_84D752689D37__



