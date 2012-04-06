#pragma once
#include "ximage.h"
#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)

typedef std::vector<int> VecLinePos;
class CImageTool
{
public:
	// get two poses by RGB(ff, 0, ff) color
	static void GetVLeftPos(CxImage* pImage, int& left1, int& left2);
	static void GetHTopPos(CxImage* pImage, int& top1, int& top2);
	// get all poses by RGB(ff, 0, ff) color
	static void GetHLeftPos(CxImage* pImage, VecLinePos& vecLinePos);
	static void GetVLeftPos(CxImage* pImage, VecLinePos& vecLinePos);
};
