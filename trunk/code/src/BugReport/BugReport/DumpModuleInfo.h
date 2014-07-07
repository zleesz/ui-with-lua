/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: DumpModuleInfo.h
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __DUMPMODULEINFO_H_75E42ED4_2DE6_438D_B9DC_7F8169C53FEF__
#define __DUMPMODULEINFO_H_75E42ED4_2DE6_438D_B9DC_7F8169C53FEF__

/**********************************  头文件 ************************************/
#include "dumpinfo.h"

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/********************************** 类定义 ***********************************/

class CDumpModuleInfo :
	public CDumpInfo
{
public:
	CDumpModuleInfo(void);
	~CDumpModuleInfo(void);

protected:
	BOOL DumpFile(HANDLE hDumpFile);
	BOOL LoadFile(HANDLE hDumpFile);
};


#ifdef  __cplusplus
}
#endif
#endif // end of __DUMPMODULEINFO_H_75E42ED4_2DE6_438D_B9DC_7F8169C53FEF__



