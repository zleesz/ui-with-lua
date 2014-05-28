//
// BaseImageLoader.cpp 
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "./BaseImageLoader.h"

#include "./FileStream.h"
#include "./ResourceStream.h"

using namespace std;
using namespace Stream;
using namespace Graphic;

BaseImageLoader::BaseImageLoader(void) : m_ref(1)
{
}

BaseImageLoader::~BaseImageLoader(void)
{
}

Bitmap* BaseImageLoader::LoadImageFromFile(const wchar_t* fileName,unsigned long resultColorType)
{
    FileStream stream(fileName, _T("rb"));
    if (stream.Valid)
    {
        return LoadImageFromStream(&stream,resultColorType);
    }
    else
    {
        return NULL;
    }
}

Bitmap* BaseImageLoader::LoadImageFromFileReverse(const wchar_t* fileName,unsigned long resultColorType)
{
	FileStream stream(fileName, _T("rb"));
	if (stream.Valid)
	{
		return LoadImageFromStreamReverse(&stream,resultColorType);
	}
	else
	{
		return NULL;
	}
}


Bitmap* BaseImageLoader::LoadImageFromResName(HMODULE hModule, const basic_string<TCHAR>& resName,unsigned long resultColorType)
{
    ResourceStream stream(hModule, resName, RT_RCDATA);
    if (stream.Valid)
    {
        return LoadImageFromStream(&stream,resultColorType);
    }
    else
    {
        return NULL;
    }    
}

Bitmap* BaseImageLoader::LoadImageFromResID(HMODULE hModule, int resID,unsigned long resultColorType)
{
    ResourceStream stream(hModule, resID, RT_RCDATA);
    if (stream.Valid)
    {
        return LoadImageFromStream(&stream,resultColorType);
    }
    else
    {
        return NULL;
    }
}

int BaseImageLoader::AddRef() 
{
    m_ref ++;
    return m_ref;
}

int BaseImageLoader::Release() 
{
    m_ref--;
    int tmp = m_ref;
    if(m_ref == 0)
    {
        delete this;
    }
    return tmp;
}

BaseImageLoader& BaseImageLoader::operator=(const BaseImageLoader& rhs) 
{
    return *this;
}

BaseImageLoader::BaseImageLoader(const BaseImageLoader& rhs) 
{
    return;
}
