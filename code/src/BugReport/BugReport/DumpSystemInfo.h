/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: DumpSystemInfo.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __DUMPSYSTEMINFO_H_34E838FC_3621_4EBF_8074_408FD931E59E__
#define __DUMPSYSTEMINFO_H_34E838FC_3621_4EBF_8074_408FD931E59E__

/**********************************  ͷ�ļ� ************************************/
#include "dumpinfo.h"

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/********************************** �ඨ�� ***********************************/
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



