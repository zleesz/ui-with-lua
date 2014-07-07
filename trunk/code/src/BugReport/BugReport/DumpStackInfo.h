/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: DumpStackInfo.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __DUMPSTACKINFO_H_B6EA8136_16F6_4022_94AC_0414A2314B65__
#define __DUMPSTACKINFO_H_B6EA8136_16F6_4022_94AC_0414A2314B65__

/**********************************  ͷ�ļ� ************************************/
#include "dumpinfo.h"

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/********************************** �ඨ�� ***********************************/

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



