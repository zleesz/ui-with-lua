
/********************************************************************
*
*   Description :  Graphic的基础数据结构 "位图",相当于windows的DIB
*                  这里的位图，是指Package 色的位图
*                  这里的位图也是完整位图,不为超大图优化
*
********************************************************************/  

#ifndef _BITMAP_H_
#define _BITMAP_H_  

namespace Graphic
{

class Bitmap
{
public:
    //创建的时候自己分配内存
    Bitmap(long colorType,unsigned long width,unsigned long height);
    //绑定方式创建，不需要分配pBuffer
    Bitmap(long colorType,unsigned long width,unsigned long height,long scanLineSize,unsigned char* pBuffer);

    virtual ~Bitmap(void);

public:
    unsigned long AddRef();
    unsigned long Release();

public:
    bool IsOwnerBuffer();
    unsigned char* GetBuffer(unsigned long x,unsigned long y) const;
    //得到每个Pixel有多少字节
    unsigned long GetPixelBytes() const; 


public:
    Bitmap* ClipSubBindBitmap(RECT* pClibRect);
    Bitmap* CloneBitmap();
    Bitmap* ChangeColorType(long colorType);
	//预处理32位PNG图片
	bool PretreatmentPNG32();
	void SetPretreatment(bool bPretreatment);
	bool GetPretreatment();

public:
    XLBitmapInfo m_info;

protected:
    long m_ref;
    unsigned long m_pixelBytes;

    bool m_isOwnerBuffer;
    unsigned char* m_buffer;

	bool m_bPretreatment;
};


}//namespace Graphic
#endif //_BITMAP_H_