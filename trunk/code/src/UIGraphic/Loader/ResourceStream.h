/********************************************************************
*
*   Description :   ResourceStream
*
********************************************************************/ 

#ifndef _RESOURCE_STREAM_H_
#define _RESOURCE_STREAM_H_

#include "./BaseMemoryStream.h"
#include <string>
#include <Windows.h>

using namespace std;

namespace Stream
{

class ResourceStream : public BaseMemoryStream
{
public:
    ResourceStream(HMODULE hModule, const basic_string<TCHAR> resName, const TCHAR* resType);
    ResourceStream(HMODULE hModule, int resID, const TCHAR* resType);
    virtual ~ResourceStream(void);
protected:
    virtual size_t Write(const void* buffer, size_t size);
protected:
    void Init(HMODULE hModule, const TCHAR* resName, const TCHAR* resType);
protected:
    HRSRC m_hResInfo;
    HGLOBAL m_hGlobal;
};

}//Stream

#endif