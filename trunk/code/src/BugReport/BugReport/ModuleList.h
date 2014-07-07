/*******************************************************************************
* 版权所有(C) pyhcx 2009
* 文件名称	: ModuleList.h
* 当前版本	: 1.0.0.1
* 作    者	: 彭勇 (pyhcx@foxmail.com)
* 设计日期	: 2009年3月31日
* 内容摘要	: 
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要

********************************************************************************/
#ifndef __MODULELIST_H_6354A654_2236_4B86_8DF7_4622D909E397__
#define __MODULELIST_H_6354A654_2236_4B86_8DF7_4622D909E397__

/**********************************  头文件 ************************************/
#include <Tlhelp32.h>
#include <list>

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/
typedef std::list<PMODULEENTRY32>			ModuleInfoList ;
typedef std::list<PMODULEENTRY32>::iterator	ModuleInfoIter ;

/********************************** 函数声明 **********************************/

/********************************** 类定义 ***********************************/
class CModuleList
{
private:
	CModuleList(void);

public:
	~CModuleList(void);

	static CModuleList& Instance();

	BOOL UpdateModuleList();
	PMODULEENTRY32 QueryModuleInfoByAddress(DWORD64 dwAddress64);
	ModuleInfoList * GetModuleList();

private:
	void ClearModuleInfo();

	ModuleInfoList	m_lstModuleInfo ;
};


#ifdef  __cplusplus
}
#endif
#endif // end of __MODULELIST_H_6354A654_2236_4B86_8DF7_4622D909E397__



