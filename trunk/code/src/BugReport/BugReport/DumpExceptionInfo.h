/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: DumpExceptionInfo.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __DUMPEXCEPTIONINFO_H_50CEA412_50D9_42EB_8EF5_960888BA5900__
#define __DUMPEXCEPTIONINFO_H_50CEA412_50D9_42EB_8EF5_960888BA5900__

/**********************************  ͷ�ļ� ************************************/
#include "dumpinfo.h"

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/********************************** �ඨ�� ***********************************/

class CDumpExceptionInfo :
	public CDumpInfo
{
public:
	CDumpExceptionInfo(PEXCEPTION_POINTERS pExceptionInfo);
	~CDumpExceptionInfo(void);

protected:
	BOOL DumpFile(HANDLE hDumpFile);
	BOOL LoadFile(HANDLE hDumpFile);

private:
	PEXCEPTION_POINTERS m_pExceptionInfo ;
};


#ifdef  __cplusplus
}
#endif
#endif // end of __DUMPEXCEPTIONINFO_H_50CEA412_50D9_42EB_8EF5_960888BA5900__



