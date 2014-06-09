#pragma once
#include "uicontrolbase.h"
#include <uiluax.h>
#include <Util.h>
#include "UITreeContainer.h"

class CUIWebBrowser : 
	public CUIControlBase,
	public IDispatch,
	public IOleClientSite,
	public IOleInPlaceSite,
	public IOleInPlaceFrame,
	public IDocHostUIHandler,
{
public:
	CUIWebBrowser(void) {};
public:
	CUIWebBrowser(CUITreeContainer* pTree, LPXMLDOMNode pNode);
	CUIWebBrowser(CUITreeContainer* pTree);
	virtual ~CUIWebBrowser(void);
public:
	UICONTROL_GETTYPE_METHOD(CT_UIWebBrowser);
	virtual void Render(CDCHandle dc) {};
	virtual void OnInitControl() {};
	virtual void OnDetroy() {};

	static int Navigate(lua_State* L);
public:
	// IUnknown methods
	virtual STDMETHODIMP QueryInterface(REFIID iid,void**ppvObject);
	virtual STDMETHODIMP_(ULONG) AddRef();
	virtual STDMETHODIMP_(ULONG) Release();
	// IDispatch Methods
	HRESULT _stdcall GetTypeInfoCount(unsigned int * pctinfo);
	HRESULT _stdcall GetTypeInfo(unsigned int iTInfo,LCID lcid,ITypeInfo FAR* FAR* ppTInfo);
	HRESULT _stdcall GetIDsOfNames(REFIID riid,OLECHAR FAR* FAR* rgszNames,unsigned int cNames,LCID lcid,DISPID FAR* rgDispId);
	HRESULT _stdcall Invoke(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS FAR* pDispParams,VARIANT FAR* pVarResult,EXCEPINFO FAR* pExcepInfo,unsigned int FAR* puArgErr);
	// IOleClientSite methods
	virtual STDMETHODIMP SaveObject();
	virtual STDMETHODIMP GetMoniker(DWORD dwA,DWORD dwW,IMoniker**pm);
	virtual STDMETHODIMP GetContainer(IOleContainer**pc);
	virtual STDMETHODIMP ShowObject();
	virtual STDMETHODIMP OnShowWindow(BOOL f);
	virtual STDMETHODIMP RequestNewObjectLayout();
	// IOleInPlaceSite methods
	virtual STDMETHODIMP GetWindow(HWND *p);
	virtual STDMETHODIMP ContextSensitiveHelp(BOOL);
	virtual STDMETHODIMP CanInPlaceActivate();
	virtual STDMETHODIMP OnInPlaceActivate();
	virtual STDMETHODIMP OnUIActivate();
	virtual STDMETHODIMP GetWindowContext(IOleInPlaceFrame** ppFrame,IOleInPlaceUIWindow **ppDoc,LPRECT r1,LPRECT r2,LPOLEINPLACEFRAMEINFO o);
	virtual STDMETHODIMP Scroll(SIZE s);
	virtual STDMETHODIMP OnUIDeactivate(int);
	virtual STDMETHODIMP OnInPlaceDeactivate();
	virtual STDMETHODIMP DiscardUndoState();
	virtual STDMETHODIMP DeactivateAndUndo();
	virtual STDMETHODIMP OnPosRectChange(LPCRECT);
	// IOleInPlaceFrame methods
	virtual STDMETHODIMP GetBorder(LPRECT l);
	virtual STDMETHODIMP RequestBorderSpace(LPCBORDERWIDTHS);
	virtual STDMETHODIMP SetBorderSpace(LPCBORDERWIDTHS w);
	virtual STDMETHODIMP SetActiveObject(IOleInPlaceActiveObject*pV,LPCOLESTR s);
	virtual STDMETHODIMP InsertMenus(HMENU h,LPOLEMENUGROUPWIDTHS x);
	virtual STDMETHODIMP SetMenu(HMENU h,HOLEMENU hO,HWND hw);
	virtual STDMETHODIMP RemoveMenus(HMENU h);
	virtual STDMETHODIMP SetStatusText(LPCOLESTR t);
	virtual STDMETHODIMP EnableModeless(BOOL f);
	virtual STDMETHODIMP TranslateAccelerator(LPMSG,WORD);
protected:
	long   _refNum;
private:
	RECT  _rcWebWnd;
	bool  _bInPlaced;
	bool  _bExternalPlace;
	bool  _bCalledCanInPlace;
	bool  _bWebWndInited;
private:
	//÷∏’Î
	IOleObject*			_pOleObj; 
	IOleInPlaceObject*	_pInPlaceObj;
	IStorage*			_pStorage;
	IWebBrowser2*		_pWB2;
	IHTMLDocument2*		_pHtmlDoc2;
	IHTMLDocument3*		_pHtmlDoc3;
	IHTMLWindow2*		_pHtmlWnd2;
	IHTMLEventObj*		_pHtmlEvent;
public:
	BEGIN_LUA_CALL_MAP(CUIWebBrowser)
		LUA_CALL_ENTRY(GetID)
		LUA_CALL_ENTRY(GetVisible)
		LUA_CALL_ENTRY(SetVisible)
		LUA_CALL_ENTRY(GetEnable)
		LUA_CALL_ENTRY(GetOwnerTree)
		LUA_CALL_ENTRY(Navigate)
	END_LUA_CALL_MAP
	LUA_CALL_REGISTER_OBJECT(CUIWebBrowser, UI.WebBrowser);
public:
	LOG_CLS_DEC();
};
