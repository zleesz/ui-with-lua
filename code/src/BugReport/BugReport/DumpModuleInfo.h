/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: DumpModuleInfo.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __DUMPMODULEINFO_H_75E42ED4_2DE6_438D_B9DC_7F8169C53FEF__
#define __DUMPMODULEINFO_H_75E42ED4_2DE6_438D_B9DC_7F8169C53FEF__

/**********************************  ͷ�ļ� ************************************/
#include "dumpinfo.h"

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/********************************** �ඨ�� ***********************************/

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



