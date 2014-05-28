
/********************************************************************
*		
*   Description :  λͼ����㷨��Ĭ��ʵ��,û�����κ��Ż� 
*
********************************************************************/  

#ifndef _DFAULT_MIX_PROVIDER_H_
#define _DFAULT_MIX_PROVIDER_H_

#include "./MixProvider.h"

namespace Graphic
{

class DefaultMixProvider : public IMixProvider
{
public:
    DefaultMixProvider();
    virtual ~DefaultMixProvider();

public:
    //---------------------RGB32-------------------------------
    virtual void DoARGB32Alpha(Bitmap& dest,long destX,long destY,
        Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight);
    virtual void DoARGB32Alpha(Bitmap& dest,long destX,long destY,
        Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight,
        unsigned char alphaValue);  
    virtual void DoXRGB32Alpha(Bitmap& dest,long destX,long destY,
        Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight,
        unsigned char alphaValue) ; 
    virtual void DoRGB32Overlap(Bitmap& dest,long destX,long destY,
        Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight,
        unsigned char alphaValue);  
};

}//namespace Graphic

#endif //_DFAULT_MIX_PROVIDER_H_