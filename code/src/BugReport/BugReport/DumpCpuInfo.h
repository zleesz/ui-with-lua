/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: DumpCpuInfo.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __DUMPCPUINFO_H_56F5D256_5500_4D5D_A672_84D752689D37__
#define __DUMPCPUINFO_H_56F5D256_5500_4D5D_A672_84D752689D37__

/**********************************  ͷ�ļ� ************************************/
#include "dumpinfo.h"

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/********************************** �ඨ�� ***********************************/

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



