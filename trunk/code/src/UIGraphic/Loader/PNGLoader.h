/********************************************************************
*
*   Description :   PNGLoader
*
********************************************************************/ 

#ifndef _PNG_LOADER_H_
#define _PNG_LOADER_H_

#include <string>
#include "png.h"

#include "./BaseImageLoader.h"
#include "../MixProvider.h"

namespace Graphic
{

class PNGLoader : public BaseImageLoader
{
public:
    PNGLoader(void);
    virtual ~PNGLoader(void);
public:
    static BaseImageLoader* CreateInstance();
    static bool CheckFileName(const wchar_t* fileName);
    static ImageLoaderNode GetLoaderNode();
	static void SetMixProvider(IMixProvider * pMixProvider);

public:
    virtual Bitmap* LoadImageFromStream(Stream::BaseStream* stream,unsigned long resultColorType);
	virtual Bitmap* LoadImageFromStreamReverse(Stream::BaseStream* stream,unsigned long resultColorType);

private:
	Bitmap* LoadImgFromStream(Stream::BaseStream* stream,unsigned long resultColorType, BOOL bReverse = FALSE);

protected:
    static void ReadDataFunc(png_structp png_ptr, png_bytep data, png_size_t length);
    static void ErrorFunc(png_structp png_ptr, png_const_charp error_msg);

	static IMixProvider*	m_pMixProvider;
};

}//Graphic

#endif
