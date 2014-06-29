#pragma once
#include "uicontrolbase.h"
#include <uiluax.h>
#include <Util.h>
#include "UITreeContainer.h"
#include "UIWebBrowserImpl.h"

class CUIWebBrowser : 
	public CUIControlBase,
	public IWebBrowserEvent
{
public:
	CUIWebBrowser(void) {};
public:
	CUIWebBrowser(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	CUIWebBrowser(CUITreeContainer* pTree);
	virtual ~CUIWebBrowser(void);
public:
	UICONTROL_GETTYPE_METHOD(CT_UIWebBrowser);
	virtual void Render(CDCHandle dc);
	virtual void OnInitControl();
	virtual void OnDetroy();

	virtual void OnBeforeNavigate2(IDispatch *pDisp,VARIANT *url,VARIANT *Flags, VARIANT *TargetFrameName,VARIANT *PostData,VARIANT *Headers,VARIANT_BOOL *Cancel);
	virtual void OnNavigatorComplete2(IDispatch *pDisp, VARIANT *URL);
	virtual void OnDocumentComplete(IDispatch* pDisp, VARIANT* URL);
	virtual void OnNavigateError(IDispatch *pDisp,VARIANT *URL,VARIANT *TargetFrameName,VARIANT *StatusCode,VARIANT_BOOL *Cancel);
	virtual void OnWindowClosing(VARIANT_BOOL IsChildWindow,VARIANT_BOOL *Cancel);
	virtual void OnNewWindow2(IDispatch** ppDisp, VARIANT_BOOL* Cancel);
	virtual void OnNewWindow3(IDispatch **ppDisp,VARIANT_BOOL *Cancel,DWORD dwFlags,BSTR bstrUrlContext,BSTR bstrUrl);
	virtual void OnTitleChange(BSTR Text);
	virtual void OnTitleIconChange(BSTR Text);
	virtual void OnStatusTextChange(BSTR Text);
	virtual void OnCommandStateChange(long Command, VARIANT_BOOL Enable);
	virtual void ProgressChange(long Progress, long ProgressMax);
	virtual void OnDownloadBegin();
	virtual void OnDownloadComplete();
	virtual void OnQuit(void);

	virtual BOOL GetVisible();
	virtual void SetVisible(BOOL bVisible);
	virtual void OnPosChange(int left, int top, int right, int bottom, int oldLeft, int oldTop, int oldRight, int oldBottom);
	virtual void SetAttr(const std::string& strName, const std::string& strValue);
private:
	BOOL GetInitVisible();
public:
	static int Navigate(lua_State* L);
private:
	CComWebBrowserObj*	m_pWebBrowserObj;
	CComBSTR			m_bstrURL;
public:
	BEGIN_LUA_CALL_MAP(CUIWebBrowser)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY2(GetVisible, CUIControlBase::GetVisible)
		LUA_CALL_ENTRY2(SetVisible, CUIControlBase::SetVisible)
		LUA_CALL_ENTRY(GetEnable)
		LUA_CALL_ENTRY(GetOwnerTree)
		LUA_CALL_ENTRY(Navigate)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIWebBrowser, UI.WebBrowser);
public:
	LOG_CLS_DEC();
};
