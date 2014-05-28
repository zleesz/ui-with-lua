
/********************************************************************
*				
*   Description :  位图拉伸算法。注意"Blt"取自著名的BitBlt函数
*
********************************************************************/  

#ifndef _BLT_PROVIDER_H_
#define _BLT_PROVIDER_H_

#include "./Bitmap.h"

namespace Graphic
{

class IBltProvider
{
public:
    //拉伸,产生一张新位图
    virtual Bitmap* Stretch(Bitmap& src,long newWidth,long newHeight) = 0;
    //拉伸到dest位图上
    virtual void Stretch(Bitmap& src,Bitmap& dest) = 0;
};

}//namespace Graphic

#endif //_BLT_PROVIDER_H_