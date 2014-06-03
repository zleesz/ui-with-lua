// UIManager.h : Declaration of the CUIManager

#pragma once
#include "resource.h"       // main symbols

#include "UI.h"
#include "XmlParser.h"
//#include <Util.h>

// CUIManager

class ATL_NO_VTABLE CUIManager : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CUIManager, &CLSID_UIManager>,
	public IDispatchImpl<IUIManager, &IID_IUIManager, &LIBID_UILib, /*wMajor =*/ 0xff, /*wMinor =*/ 0xff>
{
public:
	CUIManager()
	{
	}

// DECLARE_REGISTRY_RESOURCEID(IDR_UIMANAGER)
DECLARE_NO_REGISTRY()


BEGIN_COM_MAP(CUIManager)
	COM_INTERFACE_ENTRY(IUIManager)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();

public:
	STDMETHOD(LoadSkin)(BSTR bstrPath);

private:
	CComBSTR m_bstrPath;
	CUIXmlParser m_XmlParser;

private:
	void InitLog4CPlus();
public:
	LOG_CLS_DEC();
};

OBJECT_ENTRY_AUTO(__uuidof(UIManager), CUIManager)
