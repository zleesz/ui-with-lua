//
// PNGLoader.cpp 
//
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "./PNGLoader.h"
#include "./BaseStream.h"

using namespace std;
using namespace Stream;
using namespace Graphic;

IMixProvider* PNGLoader::m_pMixProvider = NULL;
PNGLoader::PNGLoader(void)
{
}

PNGLoader::~PNGLoader(void)
{
}

BaseImageLoader* PNGLoader::CreateInstance() 
{
    return new PNGLoader();
}

bool PNGLoader::CheckFileName(const wchar_t* fileName) 
{
    basic_string<TCHAR> strFileName = fileName;
    basic_string<TCHAR>::size_type index = strFileName.find_last_of(_T("."));
    if (index != basic_string<TCHAR>::npos)
    {
        basic_string<TCHAR> strExt = strFileName.substr(index+1);
        if(strExt.length() != 3)
            return false;
        transform(strExt.begin(), strExt.end(), strExt.begin(), tolower);
        if(strExt != _T("png"))
            return false;
    }
    return true;
}

ImageLoaderNode PNGLoader::GetLoaderNode() 
{
    ImageLoaderNode node;
    node.CreateInstance = PNGLoader::CreateInstance;
    node.CheckFilePath = PNGLoader::CheckFileName;
    return node;
}

Bitmap* PNGLoader::LoadImageFromStream(BaseStream* stream,unsigned long resultColorType) 
{
    return LoadImgFromStream(stream, resultColorType);
}

Bitmap* PNGLoader::LoadImageFromStreamReverse(Stream::BaseStream* stream,unsigned long resultColorType)
{
	return LoadImgFromStream(stream, resultColorType, TRUE);
}

Bitmap* PNGLoader::LoadImgFromStream(Stream::BaseStream* stream,unsigned long resultColorType, BOOL bReverse)
{
	png_struct *pPNG;
	png_info *pInfo;
	png_bytepp pRows = NULL;
	Bitmap* pBmp = NULL;

	pPNG = png_create_read_struct(PNG_LIBPNG_VER_STRING, (void *)NULL, NULL, NULL);
	if (pPNG == NULL)
	{
		return NULL;
	}

	pInfo = png_create_info_struct(pPNG);
	if (pInfo == NULL)
	{
		png_destroy_read_struct(&pPNG, (png_infopp)NULL, (png_infopp)NULL);
		return NULL;
	}

	if (setjmp(pPNG->jmpbuf))
	{
		if (pBmp) pBmp->Release();
		if (pRows) delete[] pRows;
		png_destroy_read_struct(&pPNG, &pInfo, (png_infopp)NULL);
		return NULL;
	}

	png_set_read_fn(pPNG, stream, (png_rw_ptr)ReadDataFunc);
	png_set_error_fn(pPNG, NULL,(png_error_ptr)ErrorFunc, NULL);
	png_read_info(pPNG, pInfo);

	if (pInfo->bit_depth == 16)	png_set_strip_16(pPNG);

	int pixelDepth = pInfo->pixel_depth;
	if (pixelDepth >  16) pixelDepth = 24;
	if (pixelDepth == 16) pixelDepth = 8;

	unsigned long pixelFormat;
	if (pInfo->color_type & PNG_COLOR_MASK_ALPHA)
	{
		pixelFormat = GRAPHIC_CT_ARGB32;
	}
	else
	{
		if (!(pInfo->color_type & PNG_COLOR_MASK_COLOR))
		{
			png_set_gray_to_rgb(pPNG);
			png_set_expand(pPNG);
			pixelFormat = GRAPHIC_CT_RGB24;
		}
		else
		{
			if (pInfo->color_type == PNG_COLOR_TYPE_PALETTE)
				png_set_expand(pPNG);
			if (pixelDepth < 8)
				png_set_expand(pPNG);
			pixelFormat = GRAPHIC_CT_RGB24;
		}
	}
	if (pInfo->color_type == PNG_COLOR_TYPE_RGB_ALPHA || //Alpha channel
		(pInfo->color_type == PNG_COLOR_TYPE_GRAY_ALPHA))
	{
		if (pInfo->color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		{
			png_set_gray_to_rgb(pPNG);
			png_set_expand(pPNG);
		}
		png_set_filler(pPNG, 0xFF, PNG_FILLER_AFTER);   
	}
	if (pInfo->color_type & PNG_COLOR_MASK_COLOR)
		png_set_bgr(pPNG);

	png_read_update_info(pPNG, pInfo);
	pixelDepth = pInfo->pixel_depth;
	switch(pixelDepth)
	{
	case 24:
		pixelFormat = GRAPHIC_CT_RGB24;
		break;
	case 32:
		pixelFormat = GRAPHIC_CT_ARGB32;
		break;
	default:
		longjmp(pPNG->jmpbuf, 1);
	}

	pBmp = new Bitmap(pixelFormat,pInfo->width, pInfo->height);
	if (!pBmp)
	{
		longjmp(pPNG->jmpbuf, 1);
	}
	pRows = new png_bytep[pPNG->height];
	for (unsigned int i = 0; i < pPNG->height; i++)
	{
		pRows[i] = pBmp->GetBuffer(0,i);
	}
	png_read_image(pPNG, pRows);
	if (pRows) 
		delete[] pRows;

	png_destroy_read_struct(&pPNG, &pInfo, (png_infopp)NULL);

	//×Ô¶¯×ª»»
	if(pixelFormat == GRAPHIC_CT_RGB24)
	{
		Bitmap* pTrueResult = pBmp->ChangeColorType(GRAPHIC_CT_ARGB32);
		pBmp->Release();
		return pTrueResult;
	}

	if (pixelFormat == GRAPHIC_CT_ARGB32)
	{
		m_pMixProvider->PretreatmentPNG32(pBmp);
	}
	return pBmp;
}

void PNGLoader::ReadDataFunc(png_structp png_ptr, png_bytep data, png_size_t length) 
{
    BaseStream* stream = (BaseStream*)png_ptr->io_ptr;
    if (stream->Read(data, length) != length) 
        png_error(png_ptr, NULL);
}

void PNGLoader::ErrorFunc(png_structp png_ptr, png_const_charp error_msg) 
{
    longjmp(png_ptr->jmpbuf, 1);
}

void PNGLoader::SetMixProvider(IMixProvider * pMixProvider)
{
	m_pMixProvider = pMixProvider;
}


