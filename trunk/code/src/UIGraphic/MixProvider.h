
/********************************************************************
*		
*   Description :  λͼ����㷨���ṩ�� 
*
********************************************************************/  

#ifndef _MIX_PROVIDER_H_
#define _MIX_PROVIDER_H_

#include "./Bitmap.h"

namespace Graphic
{

class IMixProvider
{
public:
    //�޸�Alpha�㷨�Ĺ���ģʽ
    virtual int SetAlphaMixSetting(unsigned long Setting)
    {
        m_setting = Setting;
        return 0;
    }

			//Ԥ����PNG32
	void PretreatmentPNG32(Bitmap* png32)
	{
		bool bPretreatment = png32->GetPretreatment();
		if (bPretreatment == true)
			return ;

		long nScanLine = png32->m_info.ScanLineLength;

		unsigned char* pBuffer = png32->GetBuffer(0,0);
		for (unsigned int i=0; i<png32->m_info.Height; i++)
		{
			DWORD * pNowBuffer = (DWORD*)pBuffer;
			for (unsigned int j=0; j<png32->m_info.Width; j++)
			{
				unsigned char * pSrcByte = (unsigned char*)(pNowBuffer + j);
				//unsigned char * pDestByte = (unsigned char*)(pNowBuffer + j);

				//ALPHA���Ԥ����
				pSrcByte[0] = pSrcByte[0] * pSrcByte[3] / 255;
				pSrcByte[1] = pSrcByte[1] * pSrcByte[3] / 255;
				pSrcByte[2] = pSrcByte[2] * pSrcByte[3] / 255;

				//pDestByte[0] = pSrcByte[0];
				//pDestByte[1] = pSrcByte[1];
				//pDestByte[2] = pSrcByte[2];
				//pDestByte[3] = pSrcByte[3];
			}
			pBuffer += nScanLine;
		}
		png32->SetPretreatment(true);
	}

    //����һЩ��Alpha��ͷ�ĺ�����Ϊ�˷���ʹ�õ�Frindly�ӿ�
    //��ͼ���,�����С��ͬ����ȡ��С�ķ�Χ 
    void Alpha(Bitmap& dest,Bitmap& src,unsigned char alphaValue)
    {
        return Alpha(dest,0,0,src,0,0,src.m_info.Width,src.m_info.Height,alphaValue);
    }

    //��src��ϵ�dest��destX,destYλ��
    void Alpha(Bitmap& dest,long destX,long destY,Bitmap& src,unsigned char alphaValue)
    {
        return Alpha(dest,destX,destY,src,0,0,src.m_info.Width,src.m_info.Height,alphaValue);
    }

    //��src�ü���srcWidth,srcHeight����ϵ�dest��destX,destYλ��
    void Alpha(Bitmap& dest,long destX,long destY,Bitmap& src,long srcWidth,long srcHeight,unsigned char alphaValue)
    {
        return Alpha(dest,destX,destY,src,0,0,srcWidth,srcHeight,alphaValue);
    }

     //��src��srcTop,srcLeft,srcWidth,srcHeight��ʾ�ľ���λ�õ�ͼ��ϵ�dest��destX,destYλ��
    void Alpha(Bitmap& dest,long destX,long destY,
                       Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight,
                       unsigned char alphaValue)
    {
        assert(src.m_info.ColorType == dest.m_info.ColorType);
        
        if(src.m_info.ColorType == GRAPHIC_CT_ARGB32)
        {
            switch(m_setting)
            {
            case GRAPHIC_BLEND_ARGB:
                DoARGB32Alpha(dest,destX,destY,src,srcTop,srcLeft,srcWidth,srcHeight);
                break;
            case GRAPHIC_BLEND_ARGB_ALPHA:
                return DoARGB32Alpha(dest,destX,destY,src,srcTop,srcLeft,srcWidth,srcHeight,alphaValue);
                break;
            case GRAPHIC_BLEND_XRGB_ALPHA:
                return DoXRGB32Alpha(dest,destX,destY,src,srcTop,srcLeft,srcWidth,srcHeight,alphaValue);
                break;
            case GRAPHIC_BLEND_XXXX_SRCCOPY:
                return DoRGB32Overlap(dest,destX,destY,src,srcTop,srcLeft,srcWidth,srcHeight,alphaValue);
                break;
            default:
                assert(0);
            }
        }
        else 
        {
            assert(0);
        }
    }

protected:
    //���к���������ʵ���˾���Ļ���㷨
    virtual void DoARGB32Alpha(Bitmap& dest,long destX,long destY,
         Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight) = 0;

    virtual void DoARGB32Alpha(Bitmap& dest,long destX,long destY,
         Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight,
        unsigned char alphaValue) = 0;  


    virtual void DoXRGB32Alpha(Bitmap& dest,long destX,long destY,
         Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight,
        unsigned char alphaValue) = 0; 


    virtual void DoRGB32Overlap(Bitmap& dest,long destX,long destY,
         Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight,
        unsigned char alphaValue) = 0;  



protected:
    inline void GetRealRect(Bitmap& dest,long destX,long destY,
                            Bitmap& src,long srcTop,long srcLeft,long srcWidth,long srcHeight,
                            long& destStartX,long& destStartY,
                            long& srcStartX,long& srcStartY,
                            long& rowLen,long& lines)
    {
        //����src,dest  Bitmap���ཻ����,���вü�
        if(destX >= (long)dest.m_info.Width || destY >= (long)dest.m_info.Height)
            return;

        if(destX + srcWidth < 0 || destY + srcHeight <0)
            return;

        if(destX >= 0)
        {
            destStartX = destX;
            srcStartX = 0;
        }
        else
        {
            destStartX = 0;
            srcStartX = -destX;
        }

        if(destY >= 0)
        {
            destStartY = destY;
            srcStartY = 0;
        }
        else
        {
            destStartY = 0;
            srcStartY = - destY;
        }

        rowLen = (srcWidth-srcStartX > (long)dest.m_info.Width-destStartX) ? (long)dest.m_info.Width-destStartX : srcWidth-srcStartX;
        lines = (srcHeight-srcStartY > (long)dest.m_info.Height-destStartY) ? (long)dest.m_info.Height-destStartY:srcHeight-srcStartY;
    }

protected:
    unsigned long m_setting;
};


}//namespace Graphic

#endif //_MIX_PROVIDER_H_