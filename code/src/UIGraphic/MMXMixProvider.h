
/********************************************************************
*				
*   Description :  使用MMX优化的MixProvider
*
********************************************************************/  

#ifndef _MMX_MIX_PROVIDER_H_
#define _MMX_MIX_PROVIDER_H_

#include "./MixProvider.h"

namespace Graphic
{

class MMXMixProvider : public IMixProvider
{
public:
    MMXMixProvider();
    virtual ~MMXMixProvider();

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

}//namespace BaseGraphic

#endif //_MMX_MIX_PROVIDER_H_