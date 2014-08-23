#include "StdAfx.h"
#include "UILuaClass.h"

CUILuaClass::CUILuaClass(void)
{
	RegisterGlobalObj();
}

CUILuaClass::~CUILuaClass(void)
{
	UnRegisterGlobalObj();
}

int CUILuaClass::Exit(lua_State* /*L*/)
{
	PostQuitMessage(0);
	return 0;
}