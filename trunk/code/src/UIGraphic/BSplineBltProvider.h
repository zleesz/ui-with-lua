
/********************************************************************
*
*   Description :   B-Spline的算法实现．
*
********************************************************************/  

#ifndef _BSPLINE_BLT_PROVIDER_H_
#define _BSPLINE_BLT_PROVIDER_H_

#include "./BltProvider.h"

namespace Graphic
{

class BSplineBltProvider : public IBltProvider
{
public:
    BSplineBltProvider();
    virtual ~BSplineBltProvider();
public:


    //拉伸
    virtual Bitmap* Stretch(Bitmap& src,long newWidth,long newHeight);
    virtual void Stretch(Bitmap& src,Bitmap& dest);


protected:

    static float BSpline(const float x);
protected:
    BSplineBltProvider(const BSplineBltProvider& rhs) {return;}
    BSplineBltProvider& operator=(const BSplineBltProvider& rhs) {return *this;}

};


}//namespace BaseGraphic

#endif //_BSPLINE_BLT_PROVIDER_H_