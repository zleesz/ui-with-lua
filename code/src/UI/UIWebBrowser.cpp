#include "StdAfx.h"
#include "UIWebBrowser.h"

CUIWebBrowser::~CUIWebBrowser(void)
{
}

CUIWebBrowser::CUIWebBrowser(CUITreeContainer* pTree) :
	_refNum(0),
	_bInPlaced(false),
	_bExternalPlace(false),
	_bCalledCanInPlace(false),
	_bWebWndInited(false),
	_pOleObj(NULL), 
	_pInPlaceObj(NULL), 
	_pStorage(NULL), 
	_pWB2(NULL), 
	_pHtmlDoc2(NULL), 
	_pHtmlDoc3(NULL), 
	_pHtmlWnd2(NULL), 
	_pHtmlEvent(NULL)
{
	::memset((PVOID)&_rcWebWnd, 0, sizeof(_rcWebWnd));
	::OleInitialize(0);
	StgCreateDocfile(0, STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT | STGM_CREATE, 0, &_pStorage);
	OleCreate(CLSID_WebBrowser, IID_IOleObject, OLERENDER_DRAW, 0, this, _pStorage, (void**)&_pOleObj);
	_pOleObj->QueryInterface(IID_IOleInPlaceObject, (LPVOID*)&_pInPlaceObj);
}

CUIWebBrowser::CUIWebBrowser(CUITreeContainer* pTree, LPXMLDOMNode pNode)
{

}

STDMETHODIMP CUIWebBrowser::QueryInterface(REFIID iid,void**ppvObject)
{
	*ppvObject = 0;
    if (iid == IID_IOleClientSite)
        *ppvObject = (IOleClientSite*)this;
    if (iid == IID_IUnknown)
        *ppvObject = this;
    if (iid == IID_IDispatch)
        *ppvObject = (IDispatch*)this;
    if (_bExternalPlace == false)
    {
        if (iid == IID_IOleInPlaceSite)
            *ppvObject = (IOleInPlaceSite*)this;
        if (iid == IID_IOleInPlaceFrame)
            *ppvObject = (IOleInPlaceFrame*)this;
        if (iid == IID_IOleInPlaceUIWindow)
            *ppvObject = (IOleInPlaceUIWindow*)this;
    }
    if (iid == DIID_DWebBrowserEvents2)
        *ppvObject = (DWebBrowserEvents2 *)this;
    if (iid == IID_IDocHostUIHandler)
        *ppvObject = (IDocHostUIHandler*)this;
    if (*ppvObject)
    {
        AddRef();
        return S_OK;
    }
    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CUIWebBrowser::AddRef()
{
	return ::InterlockedIncrement( &_refNum );
}

STDMETHODIMP_(ULONG) CUIWebBrowser::Release()
{
	return ::InterlockedDecrement( &_refNum );
}

HRESULT _stdcall CUIWebBrowser::GetTypeInfoCount(
    unsigned int * pctinfo) 
{
    return E_NOTIMPL;
}

HRESULT _stdcall CUIWebBrowser::GetTypeInfo(
    unsigned int iTInfo,
    LCID lcid,
    ITypeInfo FAR* FAR* ppTInfo) 
{
    return E_NOTIMPL;
}

HRESULT _stdcall CUIWebBrowser::GetIDsOfNames(REFIID riid, 
  OLECHAR FAR* FAR* rgszNames, 
  unsigned int cNames, 
  LCID lcid, 
  DISPID FAR* rgDispId )
{
    return E_NOTIMPL;
}

HRESULT _stdcall CUIWebBrowser::Invoke(
    DISPID dispIdMember,
    REFIID riid,
    LCID lcid,
    WORD wFlags,
    DISPPARAMS* pDispParams,
    VARIANT* pVarResult,
    EXCEPINFO* pExcepInfo,
    unsigned int* puArgErr)
{
        /*走私货,留在以后讲,是关于DWebBrowserEvents2让人激动的实现,而且简单.
    // DWebBrowserEvents2
    if( dispIdMember == DISPID_DOCUMENTCOMPLETE)
    {
        DocumentComplete(pDispParams->rgvarg[1].pdispVal,pDispParams->rgvarg[0].pvarVal);
        return S_OK;
    }
    if( dispIdMember == DISPID_BEFORENAVIGATE2)
    {
        BeforeNavigate2( pDispParams->rgvarg[6].pdispVal,
                         pDispParams->rgvarg[5].pvarVal,
                         pDispParams->rgvarg[4].pvarVal,
                         pDispParams->rgvarg[3].pvarVal,
                         pDispParams->rgvarg[2].pvarVal,
                         pDispParams->rgvarg[1].pvarVal,
                         pDispParams->rgvarg[0].pboolVal);
        return S_OK;
    }
        */
    return E_NOTIMPL;
}

STDMETHODIMP CUIWebBrowser::SaveObject()
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::GetMoniker(DWORD dwA,DWORD dwW,IMoniker**pm)
{
	*pm = 0;
	return E_NOTIMPL;
}

STDMETHODIMP CUIWebBrowser::GetContainer(IOleContainer**pc)
{
	*pc = 0;
	return E_FAIL;
}

STDMETHODIMP CUIWebBrowser::ShowObject()
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::OnShowWindow(BOOL f)
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::RequestNewObjectLayout()
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::GetWindow(HWND *p)
{
	*p = GetHWND();
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::ContextSensitiveHelp(BOOL)
{
	return E_NOTIMPL;
}

STDMETHODIMP CUIWebBrowser::CanInPlaceActivate()//If this function return S_FALSE, AX cannot activate in place!
{
	if ( _bInPlaced )//Does WebBrowser Control already in placed?
	{
		_bCalledCanInPlace = true;
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CUIWebBrowser::OnInPlaceActivate()
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::OnUIActivate()
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::GetWindowContext(IOleInPlaceFrame** ppFrame,IOleInPlaceUIWindow **ppDoc,LPRECT r1,LPRECT r2,LPOLEINPLACEFRAMEINFO o)
{

	*ppFrame = (IOleInPlaceFrame*)this;
	AddRef();
	*ppDoc = NULL;
	::GetClientRect(  GetHWND() ,&_rcWebWnd );
	*r1 = _rcWebWnd;
	*r2 = _rcWebWnd;
	o->cb = sizeof(OLEINPLACEFRAMEINFO);
	o->fMDIApp = false;
	o->hwndFrame = GetParent( GetHWND() );
	o->haccel = 0;
	o->cAccelEntries = 0;

	return S_OK;
}

STDMETHODIMP CUIWebBrowser::Scroll(SIZE s)
{
	return E_NOTIMPL;
}

STDMETHODIMP WebBrowser::OnUIDeactivate(int)
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::OnInPlaceDeactivate()
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::DiscardUndoState()
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::DeactivateAndUndo()
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::OnPosRectChange(LPCRECT)
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::CanInPlaceActivate()//If this function return S_FALSE, AX cannot activate in place!
{
	if ( _bInPlaced )//Does WebBrowser Control already in placed?
	{
		_bCalledCanInPlace = true;
		return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CUIWebBrowser::GetWindowContext(IOleInPlaceFrame** ppFrame,IOleInPlaceUIWindow **ppDoc,LPRECT r1,LPRECT r2,LPOLEINPLACEFRAMEINFO o)
{
	//因为IOleInPlaceFrame接口已经被我们的WebBrowser实现
	//所以直接设置为this
	*ppFrame = (IOleInPlaceFrame*)this;
	AddRef();
	*ppDoc = NULL;
	// r1, r2设置为框架的大小, 让WB充满整个窗口
	::GetClientRect(  GetHWND() ,&_rcWebWnd );
	*r1 = _rcWebWnd;
	*r2 = _rcWebWnd;
	//我们没有这方面的要求,所以仅仅初始化.
	o->cb = sizeof(OLEINPLACEFRAMEINFO);
	o->fMDIApp = false;
	o->hwndFrame = GetParent( GetHWND() );
	o->haccel = 0;
	o->cAccelEntries = 0;

	return S_OK;
}

STDMETHODIMP CUIWebBrowser::GetBorder(LPRECT l)
{
	::GetClientRect(  GetHWND() ,&_rcWebWnd );
	*l = _rcWebWnd;
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::RequestBorderSpace(LPCBORDERWIDTHS b)
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::SetBorderSpace(LPCBORDERWIDTHS b)
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::SetActiveObject(IOleInPlaceActiveObject*pV,LPCOLESTR s)
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowser::SetStatusText(LPCOLESTR t)
{
	return E_NOTIMPL;
}

STDMETHODIMP CUIWebBrowser::EnableModeless(BOOL f)
{
	return E_NOTIMPL;
}

STDMETHODIMP CUIWebBrowser::TranslateAccelerator(LPMSG,WORD)
{
	return E_NOTIMPL;
}

HRESULT _stdcall CUIWebBrowser::RemoveMenus(HMENU h)
{
	return E_NOTIMPL;
}

HRESULT _stdcall CUIWebBrowser::InsertMenus(HMENU h,LPOLEMENUGROUPWIDTHS x)
{
	return E_NOTIMPL;
}

HRESULT _stdcall CUIWebBrowser::SetMenu(HMENU h,HOLEMENU hO,HWND hw)
{
	return E_NOTIMPL;
}

void CUIWebBrowser::DocumentComplete( IDispatch *pDisp,VARIANT *URL)
{
	//老天保佑,多好的函数啊.
	return ;
}

void CUIWebBrowser::BeforeNavigate2( IDispatch *pDisp,VARIANT *&url,VARIANT *&Flags,VARIANT *&TargetFrameName,VARIANT *&PostData,VARIANT *&Headers,VARIANT_BOOL *&Cancel)
{
	PCWSTR pcwApp = L"app:";
	if( url->vt != VT_BSTR )
		return;
	if( 0 == _wcsnicmp( pcwApp, url->bstrVal,wcslen(pcwApp)) )
	{
		*Cancel = VARIANT_TRUE;
		_OnHtmlCmd( url->bstrVal+wcslen(pcwApp) );
		return;
	}
	*Cancel = VARIANT_FALSE;
}
HRESULT CUIWebBrowser::ShowContextMenu(
									 DWORD dwID,
									 POINT *ppt,
									 IUnknown *pcmdtReserved,
									 IDispatch *pdispReserved){return E_NOTIMPL;}
HRESULT CUIWebBrowser:: GetHostInfo( 
								 DOCHOSTUIINFO *pInfo){return E_NOTIMPL;}
HRESULT CUIWebBrowser:: ShowUI( 
							DWORD dwID,
							IOleInPlaceActiveObject *pActiveObject,
							IOleCommandTarget *pCommandTarget,
							IOleInPlaceFrame *pFrame,
							IOleInPlaceUIWindow *pDoc){return E_NOTIMPL;}
HRESULT CUIWebBrowser:: HideUI( void){return E_NOTIMPL;}
HRESULT CUIWebBrowser:: UpdateUI( void){return E_NOTIMPL;}
//HRESULT CUIWebBrowser:: EnableModeless( 
//  BOOL fEnable){return E_NOTIMPL;}
HRESULT CUIWebBrowser:: OnDocWindowActivate( 
	BOOL fActivate){return E_NOTIMPL;}
HRESULT CUIWebBrowser:: OnFrameWindowActivate( 
	BOOL fActivate){return E_NOTIMPL;}
HRESULT CUIWebBrowser:: ResizeBorder( 
								  LPCRECT prcBorder,
								  IOleInPlaceUIWindow *pUIWindow,
								  BOOL fRameWindow){return E_NOTIMPL;}
HRESULT CUIWebBrowser:: TranslateAccelerator( 
	LPMSG lpMsg,
	const GUID *pguidCmdGroup,
	DWORD nCmdID){return E_NOTIMPL;}
HRESULT CUIWebBrowser:: GetOptionKeyPath( 
									  LPOLESTR *pchKey,
									  DWORD dw){return E_NOTIMPL;}
HRESULT CUIWebBrowser:: GetDropTarget( 
								   IDropTarget *pDropTarget,
								   IDropTarget **ppDropTarget)
{
	return E_NOTIMPL;//使用默认拖拽
}
HRESULT CUIWebBrowser:: GetExternal( IDispatch **ppDispatch)
{
	return E_NOTIMPL;
}
HRESULT CUIWebBrowser:: TranslateUrl( 
								  DWORD dwTranslate,
								  OLECHAR *pchURLIn,
								  OLECHAR **ppchURLOut){return E_NOTIMPL;}
HRESULT CUIWebBrowser:: FilterDataObject( 
									  IDataObject *pDO,
									  IDataObject **ppDORet){return E_NOTIMPL;}

IWebBrowser2* 
CUIWebBrowser::GetWebBrowser2()
{
	if( _pWB2 != NULL )
		return _pWB2;
	NULLTEST_SE( _pOleObj,L"Ole对象为空");
	HRTEST_SE( _pOleObj->QueryInterface(IID_IWebBrowser2,(void**)&_pWB2),L"QueryInterface IID_IWebBrowser2 失败");
	return _pWB2;
RETURN:
	return NULL;
}
IHTMLDocument2*    
CUIWebBrowser::GetHTMLDocument2()
{
	if( _pHtmlDoc2 != NULL )
		return _pHtmlDoc2;
	IWebBrowser2* pWB2 = NULL;
	NULLTEST(pWB2 = GetWebBrowser2());//GetWebBrowser2已经将错误原因交给LastError.
	IDispatch* pDp =  NULL;
	HRTEST_SE(pWB2->get_Document(&pDp),L"DWebBrowser2::get_Document 错误");
	HRTEST_SE(pDp->QueryInterface(IID_IHTMLDocument2,(void**)&_pHtmlDoc2),L"QueryInterface IID_IHTMLDocument2 失败");
	return _pHtmlDoc2;
RETURN:
	return NULL;
}
IHTMLDocument3*    
CUIWebBrowser::GetHTMLDocument3()
{
	if( _pHtmlDoc3 != NULL )
		return _pHtmlDoc3;
	IWebBrowser2* pWB2 = NULL;
	NULLTEST(pWB2 = GetWebBrowser2());//GetWebBrowser2已经将错误原因交给LastError.
	IDispatch* pDp =  NULL;
	HRTEST_SE(pWB2->get_Document(&pDp),L"DWebBrowser2::get_Document 错误");
	HRTEST_SE(pDp->QueryInterface(IID_IHTMLDocument3,(void**)&_pHtmlDoc3),L"QueryInterface IID_IHTMLDocument3 失败");
	return _pHtmlDoc3;
RETURN:
	return NULL;
}
IHTMLWindow2*
CUIWebBrowser::GetHTMLWindow2()
{
	if( _pHtmlWnd2 != NULL)
		return _pHtmlWnd2;
	IHTMLDocument2*  pHD2 = GetHTMLDocument2();
	NULLTEST( pHD2 );
	HRTEST_SE( pHD2->get_parentWindow(&_pHtmlWnd2),L"IHTMLWindow2::get_parentWindow 错误" );
	return _pHtmlWnd2;
RETURN:
	return NULL;
}
IHTMLEventObj*   
CUIWebBrowser::GetHTMLEventObject()
{
	if( _pHtmlEvent != NULL )
		return _pHtmlEvent;
	IHTMLWindow2* pHW2;
	NULLTEST( pHW2 = GetHTMLWindow2() );
	HRTEST_SE( pHW2->get_event(&_pHtmlEvent),L"IHTMLWindow2::get_event 错误");
	return _pHtmlEvent;
RETURN:
	return NULL;
}
BOOL       
CUIWebBrowser::SetWebRect(LPRECT lprc)
{
    BOOL bRet = FALSE;
    if( false == _bInPlaced )//尚未OpenWebBrowser操作,直接写入_rcWebWnd
    {
       _rcWebWnd = *lprc;
    }
    else//已经打开WebBrowser,通过 IOleInPlaceObject::SetObjectRects 调整大小
    {
        SIZEL size;
        size.cx = RECTWIDTH(*lprc);
        size.cy = RECTHEIGHT(*lprc);
        IOleObject* pOleObj;
        NULLTEST( pOleObj= _GetOleObject());
        HRTEST_E( pOleObj->SetExtent(  1,&size ),L"SetExtent 错误");
        IOleInPlaceObject* pInPlace;
        NULLTEST( pInPlace = _GetInPlaceObject());
        HRTEST_E( pInPlace->SetObjectRects(lprc,lprc),L"SetObjectRects 错误");
        _rcWebWnd = *lprc;
    }
    bRet = TRUE;
RETURN:
    return bRet;
}

BOOL CUIWebBrowser::OpenWebBrowser()
{
    BOOL bRet = FALSE;
    NULLTEST_E( _GetOleObject(),L"ActiveX对象为空" );//对于本身的实现函数,其自身承担错误录入工作
    
    if( (RECTWIDTH(_rcWebWnd) && RECTHEIGHT(_rcWebWnd)) == 0 )
        ::GetClientRect( GetHWND() ,&_rcWebWnd);//设置WebBrowser的大小为窗口的客户区大小.
    
    if( _bInPlaced == false )// Activate In Place
    {
        _bInPlaced = true;//_bInPlaced must be set as true, before INPLACEACTIVATE, otherwise, once DoVerb, it would return error;
        _bExternalPlace = 0;//lParam;
    
        HRTEST_E( _GetOleObject()->DoVerb(OLEIVERB_INPLACEACTIVATE,0,this,0, GetHWND()  ,&_rcWebWnd),L"关于INPLACE的DoVerb错误");
        _bInPlaced = true;
        
        
        //* 挂接DWebBrwoser2Event
        IConnectionPointContainer* pCPC = NULL;
        IConnectionPoint*          pCP  = NULL;
        HRTEST_E( GetWebBrowser2()->QueryInterface(IID_IConnectionPointContainer,(void**)&pCPC),L"枚举IConnectionPointContainer接口失败");
        HRTEST_E( pCPC->FindConnectionPoint( DIID_DWebBrowserEvents2,&pCP),L"FindConnectionPoint失败");
        DWORD dwCookie = 0;
        HRTEST_E( pCP->Advise( (IUnknown*)(void*)this,&dwCookie),L"IConnectionPoint::Advise失败");
    }
    bRet = TRUE;
RETURN:
    return bRet;
}

BOOL CUIWebBrowser::OpenURL(VARIANT* pVarUrl)
{
    BOOL bRet = FALSE;
    HRTEST_E( GetWebBrowser2()->Navigate2( pVarUrl,0,0,0,0),L"GetWebBrowser2 失败");
    bRet = TRUE;
RETURN:
    return bRet;
}

CUIWebBrowser::Navigate(BSTR bstrURL)
{
	BOOL bRet = FALSE;
	//GetWebBrowser2 返回IWebBrowser2
	//他的实现是这样的:
	//if( _pWB2 != NULL )//_pWB2是WebBrowser的私有IWebBrowser2指针
	//   return _pWB2;//如果_pWB2已经不是NULL,即已经获取过内容,则直接返回
	////否则使用WB控件对象枚举IWebBrowser2指针
	//_pOleObj->QueryInterface(IID_IWebBrowser2,(void**)&_pWB2);
	//return _pWB2;
	GetWebBrowser2()->Navigate2( pVarUrl,0,0,0,0);//打开网页
}

CUIWebBrowser::CUIWebBrowser(CUITreeContainer* pTree)
{
	BOOL bRet = FALSE;
	// RECTWIDTH 和 RECTHEIGHT 是计算RECT长宽的宏函数
	// _rcWebWnd 是WebBrowser的私有RECT成员,就是GetHWND()的大小.
	// 下面的语句就是关联两者
	// GetHWND()获取容器的框架窗口句柄,就是我们创建的窗口,关于这个函数,下面会单独讲
	if( (RECTWIDTH(_rcWebWnd) && RECTHEIGHT(_rcWebWnd)) == 0 )
		::GetClientRect( GetHWND() ,&_rcWebWnd);//设置WebBrowser的大小为窗口的客户区大小.

	// _bInPlaced 是WebBrowser的一个私有BOOL成员,初始为false
	// 一旦执行了OLEIVERB_INPLACEACTIVATE (就是下面的操作)后,立即设置为true,防止控件被多次放置在容器中
	// _GetOleObject()是WebBrowser的一个保护函数成员,他只是简单的返回_pOleObj
	// _pOleObj就是WB控件本身. 考虑到WebBrowser是一个基本类,以后必然被其他类继承,所以使用_GetOleObject()来返回_pOleObject,来实现防止_pOleObject本身不被修改的意外.
	if( _bInPlaced == false )// Activate In Place
	{
		_bInPlaced = true;//_bInPlaced must be set as true, before INPLACEACTIVATE, otherwise, once DoVerb, it would return error;
		_bExternalPlace = 0;//lParam;
		_GetOleObject()->DoVerb(OLEIVERB_INPLACEACTIVATE,0,this,0, GetHWND(),&_rcWebWnd);
		_bInPlaced = true;
	}
	bRet = TRUE;
	return bRet;
}

int CUIWebBrowser::Navigate(lua_State* L)
{
	LOG_AUTO();
}