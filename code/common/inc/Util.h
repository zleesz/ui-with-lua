#pragma once
#include <uilog4cplus.h>
#include <string>
#include <expat.h>
#include <memory>
#include <map>
#include <vector>

namespace Util
{
	static bool BSTRToString( BSTR src, std::string& dest)
	{
		if(!src)
			return false;
		int  iLen = (int)wcslen(src);
		if(iLen > 0)
		{
			char* szdest = new  char[iLen * 4];
			if(NULL == szdest)
				return false;
			ZeroMemory(szdest, iLen * 4);			
			int nLen = WideCharToMultiByte(CP_UTF8, NULL, src, iLen, szdest, iLen * 4, 0, 0);
			szdest[nLen] = '\0'; 
			dest = szdest;
			delete [] szdest ;
			return true;
		}
		return false;
	}
	static bool StringToWideString(const char* src, std::wstring& wstr)
	{
		if(!src)
			return false;
		int iLen = (int)strlen(src);
		wchar_t* szm = new wchar_t[iLen * 4];
		ZeroMemory(szm, iLen * 4);
		int nLen = MultiByteToWideChar(CP_UTF8, 0, src,iLen, szm, iLen*4); 
		szm [nLen] = '\0';
		wstr = szm;
		delete [] szm;
		return false;
	}
	static HRESULT CreateInstanceFromHandle(HMODULE hModuleHandle, REFCLSID rclsid, REFIID riid, void** ppv)
	{
		if(hModuleHandle==NULL) return E_FAIL;

		HRESULT hr = E_FAIL;

		typedef HRESULT (STDAPICALLTYPE *_pfnDllGetClassObject)(REFCLSID , REFIID , LPVOID*);
		_pfnDllGetClassObject pfnDllGetClassObject = (_pfnDllGetClassObject)
			GetProcAddress(hModuleHandle, "DllGetClassObject");
		if(pfnDllGetClassObject == NULL)
		{
			return  hr;
		}

		CComPtr<IClassFactory> spCF;
		hr = (*pfnDllGetClassObject)(rclsid, IID_IClassFactory, (LPVOID*) &spCF);
		if (hr != S_OK)
		{
			return hr;
		}

		// Create instance
		hr = spCF->CreateInstance(NULL, riid, ppv);	
		return hr;
	}
};