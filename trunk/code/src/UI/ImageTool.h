#pragma once
#include "ximage.h"
#define WIDTHBYTES(bits) ((DWORD)(((bits)+31) & (~31)) / 8)

class CImageTool
{
public:
	CImageTool(void);
	~CImageTool(void);
};
