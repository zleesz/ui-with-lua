/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: ModuleList.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __MODULELIST_H_6354A654_2236_4B86_8DF7_4622D909E397__
#define __MODULELIST_H_6354A654_2236_4B86_8DF7_4622D909E397__

/**********************************  ͷ�ļ� ************************************/
#include <Tlhelp32.h>
#include <list>

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/
typedef std::list<PMODULEENTRY32>			ModuleInfoList ;
typedef std::list<PMODULEENTRY32>::iterator	ModuleInfoIter ;

/********************************** �������� **********************************/

/********************************** �ඨ�� ***********************************/
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



