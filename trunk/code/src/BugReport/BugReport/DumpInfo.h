/*******************************************************************************
* ��Ȩ����(C) pyhcx 2009
* �ļ�����	: DumpInfo.h
* ��ǰ�汾	: 1.0.0.1
* ��    ��	: ���� (pyhcx@foxmail.com)
* �������	: 2009��3��31��
* ����ժҪ	: 
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ

********************************************************************************/
#ifndef __DUMPINFO_H_F866BA7F_6100_499B_B2C4_1EEB06ED2855__
#define __DUMPINFO_H_F866BA7F_6100_499B_B2C4_1EEB06ED2855__

/**********************************  ͷ�ļ� ************************************/

#ifdef  __cplusplus
extern "C" {
#endif

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/********************************** �ඨ�� ***********************************/

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


