/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: DumpSystemInfo.h
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __DUMPSYSTEMINFO_H_34E838FC_3621_4EBF_8074_408FD931E59E__
#define __DUMPSYSTEMINFO_H_34E838FC_3621_4EBF_8074_408FD931E59E__

/**********************************  头文件 ************************************/
#include "dumpinfo.h"

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/********************************** 类定义 ***********************************/
class CDumpSystemInfo :	public CDumpInfo
{
public:
	CDumpSystemInfo(void);
	~CDumpSystemInfo(void);

protected:
	BOOL DumpFile(HANDLE hDumpFile);
	BOOL LoadFile(HANDLE hDumpFile);

private:
	BOOL GetOSVersionString(LPTSTR ptszOSVer,DWORD dwLen);
};


#ifdef  __cplusplus
}
#endif
#endif // end of __DUMPSYSTEMINFO_H_34E838FC_3621_4EBF_8074_408FD931E59E__



