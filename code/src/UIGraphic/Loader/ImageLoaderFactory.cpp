//
// ImageLoaderFactory.cpp 
//
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./ImageLoaderFactory.h"
#include "MemoryStream.h"

using namespace std;
using namespace Graphic;

ImageLoaderFactory::ImageLoaderFactory(void)
{
}

ImageLoaderFactory::~ImageLoaderFactory(void)
{
}

int ImageLoaderFactory::RegisterLoader(ImageLoaderNode node) 
{
    m_nodeList.push_back(node);
    return (int)m_nodeList.size();
}

Bitmap* ImageLoaderFactory::LoadImageFromMemory(void *buffer, long size, unsigned long colorType) 
{
    BaseImageLoader* pLoader = GetLoader(L"test.png");
    if(pLoader != 0)
    {
		Stream::MemoryStream stream(buffer, size);
        Bitmap* result = pLoader->LoadImageFromStream(&stream,colorType);
        pLoader->Release();
        return result;
    }
    else
    {
        return 0;
    }
}

Bitmap* ImageLoaderFactory::LoadImageFromFile(const wchar_t* fileName,unsigned long colorType) 
{
    BaseImageLoader* pLoader = GetLoader(fileName);
    if(pLoader != 0)
    {
        Bitmap* result = pLoader->LoadImageFromFile(fileName,colorType);
        pLoader->Release();
        return result;
    }
    else
    {
        return 0;
    }
}

Bitmap* ImageLoaderFactory::LoadImageFromFileReverse(const wchar_t* fileName,unsigned long colorType)
{
	BaseImageLoader* pLoader = GetLoader(fileName);
	if(pLoader != 0)
	{
		Bitmap* result = pLoader->LoadImageFromFileReverse(fileName,colorType);
		pLoader->Release();
		return result;
	}
	else
	{
		return 0;
	}

}


BaseImageLoader* ImageLoaderFactory::GetLoader(const wchar_t* fileName) 
{
    vector<ImageLoaderNode>::iterator iterEnd, iter;
    iterEnd = m_nodeList.end();
    for(iter = m_nodeList.begin(); iter != iterEnd; ++iter)
    {
        if(iter->CheckFilePath(fileName))
        {
            BaseImageLoader* result;
            result = iter->CreateInstance();
            return result;
        }
    }
    return 0;
}

ImageLoaderFactory& ImageLoaderFactory::Instance()
{
    static ImageLoaderFactory instance;
    return instance;
}