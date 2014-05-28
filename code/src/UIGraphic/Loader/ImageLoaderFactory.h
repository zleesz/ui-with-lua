/********************************************************************
*
*   Description :   ImageLoaderFactory
*
********************************************************************/ 

#ifndef _IMAGE_LOADER_FACTORY_H_
#define _IMAGE_LOADER_FACTORY_H_

#include <string>
#include <vector>

#include "./BaseImageLoader.h"
#include "../Bitmap.h"

namespace Graphic
{

class ImageLoaderFactory
{
protected:
    ImageLoaderFactory(void);
public:
    virtual ~ImageLoaderFactory(void);
public:
    int RegisterLoader(ImageLoaderNode node);
	Bitmap* LoadImageFromMemory(void *buffer, long size, unsigned long colorType) ;
    Bitmap* LoadImageFromFile(const wchar_t* fileName,unsigned long colorType);
	Bitmap* LoadImageFromFileReverse(const wchar_t* fileName,unsigned long colorType);
    BaseImageLoader* GetLoader(const wchar_t* fileName);

public:
    static ImageLoaderFactory& Instance();
protected:
    std::vector<ImageLoaderNode> m_nodeList;
protected:
    ImageLoaderFactory& operator= (const ImageLoaderFactory& rhs)
    {
        return *this;
    }
    ImageLoaderFactory(const ImageLoaderFactory& rhs)
    {
        return;
    }
};

}//Graphic

#endif