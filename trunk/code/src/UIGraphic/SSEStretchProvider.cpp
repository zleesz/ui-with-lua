#include "stdafx.h"
#include <assert.h>
#include "./SSEStretchProvider.h"

using namespace Graphic;

SSEStretchProvider::SSEStretchProvider(void)
{
}

SSEStretchProvider::~SSEStretchProvider(void)
{
}

Bitmap* SSEStretchProvider::Stretch(Bitmap& src, long newWidth, long newHeight)
{
    assert (src.m_info.ColorType == GRAPHIC_CT_ARGB32);
    Bitmap* pBmp = new Bitmap(src.m_info.ColorType, newWidth, newHeight);
    Stretch(src, *pBmp);
    return pBmp;
}

void SSEStretchProvider::Stretch(Bitmap& src, Bitmap& dest)
{
    assert (src.m_info.ColorType == GRAPHIC_CT_ARGB32);
    assert (dest.m_info.ColorType == GRAPHIC_CT_ARGB32);
    unsigned long xrIntFloat16 = (src.m_info.Width << 16) / dest.m_info.Width;
    unsigned long yrIntFloat16 = (src.m_info.Height << 16) / dest.m_info.Height;
    unsigned long dstWidth = dest.m_info.Width;
    unsigned long srcy16 = 0;
        DWORD* pDstLine = (DWORD*)dest.GetBuffer(0, 0);
            for (unsigned long y = 0; y < dest.m_info.Height; ++y)
            {
                DWORD* pSrcLine = ((DWORD*)(src.GetBuffer(0, 0) + src.m_info.ScanLineLength * (srcy16 >> 16)));
                __asm
                {
                    mov esi, pSrcLine
                    mov edi, pDstLine
                    mov edx, xrIntFloat16
                    mov ecx, dstWidth
                    xor eax, eax
                    and ecx, (not 3)
                    test ecx, ecx
                    jle ENDLOOP
                    lea edi, [edi+ecx*4]
                    neg ecx
WRITELOOP:
                    mov ebx, eax
                    shr eax, 16
                    movd mm0, [esi+eax*4]
                    mov eax, ebx
                    lea ebx, [eax+edx]
                    shr ebx, 16
                    punpckldq mm0, [esi+ebx*4]
                    lea eax, [eax+edx*2]
                    movntq qword ptr [edi+ecx*4], mm0
                    mov ebx, eax
                    shr eax, 16
                    movd mm1, [esi+eax*4]
                    mov eax, ebx
                    lea ebx, [eax+edx]    
                    shr ebx, 16
                    punpckldq mm1, [esi+ebx*4]
                    lea eax, [eax+edx*2]
                    movntq qword ptr [edi+ecx*4+8], mm1
                    add ecx, 4
                    jnz WRITELOOP
ENDLOOP:
                    mov ebx, eax
                    mov ecx, dstWidth
                    and ecx, 3
                    test ecx, ecx
                    jle END
                    lea edi, [edi+ecx*4]
                    neg ecx
BORDER:
                    mov eax, ebx
                    shr eax, 16
                    mov eax, [esi+eax*4]
                    mov [edi+ecx*4], eax
                    add ebx, edx
                    inc ecx
                    jnz BORDER
END:
                    sfence
                    emms
                }
                srcy16 += yrIntFloat16;
                pDstLine = (DWORD*)((unsigned char*)pDstLine + dest.m_info.ScanLineLength);
            }
}