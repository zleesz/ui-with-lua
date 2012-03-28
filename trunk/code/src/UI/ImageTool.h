#pragma once
#include "ximage.h"
#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)

class CImageTool
{
public:
	static void GetVLeftPos(CxImage* pImage, int& left1, int& left2);
	static void GetHTopPos(CxImage* pImage, int& top1, int& top2);
};
