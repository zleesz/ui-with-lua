#include "StdAfx.h"
#include "UILuaClass.h"

CUILuaClass::CUILuaClass(void)
{
}

CUILuaClass::~CUILuaClass(void)
{
}

int CUILuaClass::Exit(lua_State* L)
{
	PostQuitMessage(0);
	return 0;
}