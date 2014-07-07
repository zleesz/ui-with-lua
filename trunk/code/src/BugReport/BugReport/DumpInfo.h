/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: DumpInfo.h
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __DUMPINFO_H_F866BA7F_6100_499B_B2C4_1EEB06ED2855__
#define __DUMPINFO_H_F866BA7F_6100_499B_B2C4_1EEB06ED2855__

/**********************************  头文件 ************************************/

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/********************************** 类定义 ***********************************/

class CDumpInfo
{
public:
	CDumpInfo(void);
	~CDumpInfo(void);

	BOOL Dump(HANDLE hDumpFile);
	BOOL Load(HANDLE hDumpFile);

protected:
	virtual BOOL DumpFile(HANDLE hDumpFile)=0;
	virtual BOOL LoadFile(HANDLE hDumpFile)=0;

	char	m_achDumpType[64];
};


#ifdef  __cplusplus
}
#endif
#endif // end of __DUMPINFO_H_F866BA7F_6100_499B_B2C4_1EEB06ED2855__


