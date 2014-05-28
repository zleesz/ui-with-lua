/********************************************************************
*
*   Description :   BaseImageLoader
*
********************************************************************/ 

#ifndef _BASE_IMAGE_LOADER_H_
#define _BASE_IMAGE_LOADER_H_


#include <string>
#include <tchar.h>
#include <wchar.h>
#include <algorithm>
#include "./BaseStream.h"

#include "../Bitmap.h"

namespace Graphic
{

class BaseImageLoader
{
protected:
    BaseImageLoader(void);
    virtual ~BaseImageLoader(void);
public:
    virtual Bitmap* LoadImageFromStream(Stream::BaseStream* stream,unsigned long resultColorType) = 0;
	virtual Bitmap* LoadImageFromStreamReverse(Stream::BaseStream* stream,unsigned long resultColorType) = 0;

    Bitmap* LoadImageFromFile(const wchar_t* fileName,unsigned long resultColorType);
	Bitmap* LoadImageFromFileReverse(const wchar_t* fileName,unsigned long resultColorType);
    Bitmap* LoadImageFromResName(HMODULE hModule, const std::basic_string<TCHAR>& resName,unsigned long resultColorType);
    Bitmap* LoadImageFromResID(HMODULE hModule, int resID,unsigned long resultColorType);
public:
    int AddRef();
    int Release();
protected:
    int m_ref;

protected:
    BaseImageLoader& operator=(const BaseImageLoader& rhs);
    BaseImageLoader(const BaseImageLoader& rhs);
};

typedef bool (*FunCheckImageFilePath)(const wchar_t* filePath);
typedef BaseImageLoader* (*FunCreateImageLoaderInstance)();

typedef struct tagLoaderNode
{
    FunCheckImageFilePath CheckFilePath;
    FunCreateImageLoaderInstance CreateInstance;
} ImageLoaderNode;

}//Graphic

#endif