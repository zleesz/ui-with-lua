
/********************************************************************
*
*   Description :  Graphic�Ļ������ݽṹ "λͼ",�൱��windows��DIB
*                  �����λͼ����ָPackage ɫ��λͼ
*                  �����λͼҲ������λͼ,��Ϊ����ͼ�Ż�
*
********************************************************************/  

#ifndef _BITMAP_H_
#define _BITMAP_H_  

namespace Graphic
{

class Bitmap
{
public:
    //������ʱ���Լ������ڴ�
    Bitmap(long colorType,unsigned long width,unsigned long height);
    //�󶨷�ʽ����������Ҫ����pBuffer
    Bitmap(long colorType,unsigned long width,unsigned long height,long scanLineSize,unsigned char* pBuffer);

    virtual ~Bitmap(void);

public:
    unsigned long AddRef();
    unsigned long Release();

public:
    bool IsOwnerBuffer();
    unsigned char* GetBuffer(unsigned long x,unsigned long y) const;
    //�õ�ÿ��Pixel�ж����ֽ�
    unsigned long GetPixelBytes() const; 


public:
    Bitmap* ClipSubBindBitmap(RECT* pClibRect);
    Bitmap* CloneBitmap();
    Bitmap* ChangeColorType(long colorType);
	//Ԥ����32λPNGͼƬ
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