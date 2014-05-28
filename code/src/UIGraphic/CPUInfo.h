/********************************************************************
*
*   Description :   CPUInfo
*
********************************************************************/ 

#ifndef _CPUINFO_H_
#define _CPUINFO_H_

namespace System
{

class CPUInfo
{
public:
    CPUInfo(void);
public:
    static CPUInfo& Instance();
protected:
    void CPUInfoInit();
public:
    bool MMXSupport;
    bool SSESupport;
    bool SSE2Support;

};

}//System

#endif