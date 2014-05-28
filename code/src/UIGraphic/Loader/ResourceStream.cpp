//
// ResourceStream.cpp 
//
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "./ResourceStream.h"

using namespace Stream;

ResourceStream::ResourceStream(HMODULE hModule, const basic_string<TCHAR> resName, const TCHAR* resType) :
    m_hResInfo(0),
    m_hGlobal(0)
{
    Init(hModule, resName.c_str(), resType);
}

ResourceStream::ResourceStream(HMODULE hModule, int resID, const TCHAR* resType) :
    m_hResInfo(0),
    m_hGlobal(0)
{
    Init(hModule, MAKEINTRESOURCE(resID), resType);
}

ResourceStream::~ResourceStream(void)
{
    if (m_hGlobal)
    {
        UnlockResource(m_hGlobal);
        FreeResource(m_hGlobal);
    }
}

size_t ResourceStream::Write(const void* buffer, size_t size)
{
    //read only
    return -1;
}

void ResourceStream::Init(HMODULE hModule, const TCHAR* resName, const TCHAR* resType)
{
    Valid = false;
    m_hResInfo = FindResource(hModule, resName, resType);
    if (m_hResInfo)
    {
        m_hGlobal = LoadResource(hModule, m_hResInfo);
        if (m_hGlobal)
        {
            SetPointer(LockResource(m_hGlobal), SizeofResource(hModule, m_hResInfo));
            Valid = true;
            return;
        }
    }
    return;
}