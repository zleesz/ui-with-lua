#include "StdAfx.h"
#include "UIWebBrowserImpl.h"
#include <atltypes.h>

CUIWebBrowserImpl::CUIWebBrowserImpl() : 
	m_pWebBrowserEvent(NULL), m_hParentWnd(NULL)
{
}

CUIWebBrowserImpl::~CUIWebBrowserImpl(void)
{
	m_pWebBrowserEvent = NULL;
	m_hParentWnd =NULL;
}

void CUIWebBrowserImpl::SetWebBrowserEvent(IWebBrowserEvent* pWebBrowserEvent)
{
	m_pWebBrowserEvent = pWebBrowserEvent;
}

void CUIWebBrowserImpl::Navigate(BSTR bstrURL)
{
	CComPtr<IWebBrowser2> spWebBrowser;
	if (SUCCEEDED(GetBrowser(&spWebBrowser)) && spWebBrowser)
	{
		CComVariant vtURL(bstrURL);
		spWebBrowser->Navigate2(&vtURL, 0, NULL, NULL, NULL);
	}
}

HRESULT CUIWebBrowserImpl::GetBrowser(IWebBrowser2** ppWebBrowser)
{
	ATLASSERT( ppWebBrowser ); 
	if ( ppWebBrowser == NULL ) 
	{
		return E_POINTER; 
	}

	*ppWebBrowser = NULL;

	CAxWindow wnd = m_hWnd;
	CComPtr<IWebBrowser2> spWebBrowser;
	HRESULT hRet = wnd.QueryControl(IID_IWebBrowser2, (void**)&spWebBrowser);
	ATLASSERT(SUCCEEDED(hRet));
	return spWebBrowser.CopyTo(ppWebBrowser);
}

HWND CUIWebBrowserImpl::Create(HWND hParentWnd)
{
	m_hParentWnd = hParentWnd;
	CRect rcParent;
	::GetWindowRect(m_hParentWnd, &rcParent);

	CRect rect(0, 0, rcParent.Width(), rcParent.Height());
	return CWindowImpl<CUIWebBrowserImpl, CAxWindow>::Create(NULL, &rect, NULL,  WS_CLIPSIBLINGS|WS_POPUP,WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE);
}

void __stdcall CUIWebBrowserImpl::OnBeforeNavigate2(IDispatch *pDisp,VARIANT *url,VARIANT *Flags, VARIANT *TargetFrameName,VARIANT *PostData,VARIANT *Headers,VARIANT_BOOL *Cancel)
{
	m_pWebBrowserEvent->OnBeforeNavigate2(pDisp, url, Flags, TargetFrameName, PostData, Headers, Cancel);
}

void __stdcall CUIWebBrowserImpl::OnNavigatorComplete2(IDispatch *pDisp, VARIANT *URL)
{
	m_pWebBrowserEvent->OnNavigatorComplete2(pDisp, URL);
}

void __stdcall CUIWebBrowserImpl::OnDocumentComplete(IDispatch* pDisp, VARIANT* URL)
{
	m_pWebBrowserEvent->OnDocumentComplete(pDisp, URL);
}

void __stdcall CUIWebBrowserImpl::OnNavigateError(IDispatch *pDisp,VARIANT *URL,VARIANT *TargetFrameName,VARIANT *StatusCode,VARIANT_BOOL *Cancel)
{
	m_pWebBrowserEvent->OnNavigateError(pDisp, URL, TargetFrameName, StatusCode, Cancel);
}

void __stdcall CUIWebBrowserImpl::OnWindowClosing(VARIANT_BOOL IsChildWindow,VARIANT_BOOL *Cancel)
{
	m_pWebBrowserEvent->OnWindowClosing(IsChildWindow, Cancel);
}

void __stdcall CUIWebBrowserImpl::OnNewWindow2(IDispatch** ppDisp, VARIANT_BOOL* Cancel)
{
	m_pWebBrowserEvent->OnNewWindow2(ppDisp, Cancel);
}

void __stdcall CUIWebBrowserImpl::OnNewWindow3(IDispatch **ppDisp,VARIANT_BOOL *Cancel,DWORD dwFlags,BSTR bstrUrlContext,BSTR bstrUrl)
{
	m_pWebBrowserEvent->OnNewWindow3(ppDisp, Cancel, dwFlags, bstrUrlContext, bstrUrl);
}

void __stdcall CUIWebBrowserImpl::OnTitleChange(BSTR Text)
{
	m_pWebBrowserEvent->OnTitleChange(Text);
}

void __stdcall CUIWebBrowserImpl::OnTitleIconChange(BSTR Text)
{
	m_pWebBrowserEvent->OnTitleIconChange(Text);
}

void __stdcall CUIWebBrowserImpl::OnStatusTextChange(BSTR Text)
{
	m_pWebBrowserEvent->OnStatusTextChange(Text);
}

void __stdcall CUIWebBrowserImpl::OnCommandStateChange(long Command, VARIANT_BOOL Enable)
{
	m_pWebBrowserEvent->OnCommandStateChange(Command, Enable);
}

void __stdcall CUIWebBrowserImpl::ProgressChange(long Progress, long ProgressMax)
{
	m_pWebBrowserEvent->ProgressChange(Progress, ProgressMax);
}


void __stdcall CUIWebBrowserImpl::OnDownloadBegin()
{
	m_pWebBrowserEvent->OnDownloadBegin();
}

void __stdcall CUIWebBrowserImpl::OnDownloadComplete()
{
	m_pWebBrowserEvent->OnDownloadComplete();
}

void __stdcall CUIWebBrowserImpl::OnQuit(void)
{
	m_pWebBrowserEvent->OnQuit();
}

STDMETHODIMP CUIWebBrowserImpl::GetExternal(IDispatch **ppDispatch)
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowserImpl::ShowContextMenu(DWORD dwID, POINT FAR* ppt, IUnknown FAR* pcmdtReserved, IDispatch FAR* pdispReserved)
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowserImpl::GetDropTarget(IDropTarget* pDropTarget, IDropTarget** ppDropTarget)
{
	return S_OK;
}

// IOleCommandTarget
STDMETHODIMP CUIWebBrowserImpl::QueryStatus(const GUID *pguidCmdGroup,ULONG cCmds,OLECMD prgCmds[],OLECMDTEXT *pCmdText)
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowserImpl::Exec(const GUID *pguidCmdGroup,DWORD nCmdID,DWORD nCmdexecopt,VARIANT *pvaIn,VARIANT *pvaOut)
{
	return S_OK;
}

STDMETHODIMP CUIWebBrowserImpl::GetDefaultDocHostUIHandler(IDocHostUIHandler** ppHandler)
{
	return S_OK;
}

// IDownloadManager
STDMETHODIMP CUIWebBrowserImpl::Download(IMoniker *pmk, IBindCtx * pbc,DWORD dwBindVerb, LONG grfBINDF,BINDINFO * pBindInfo,LPCOLESTR pszHeaders,LPCOLESTR pszRedir, UINT uiCP)
{
	return S_OK;
}

LRESULT CUIWebBrowserImpl::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SetParent(m_hParentWnd);

	LPOLESTR lpCLSID = NULL;
	::StringFromCLSID(CLSID_WebBrowser, &lpCLSID);
	CreateControl(lpCLSID);
	::CoTaskMemFree(lpCLSID);
	CComPtr<IObjectWithSite> spOws;
	HRESULT hr = QueryHost(IID_IObjectWithSite, (void**)&spOws);
	if (SUCCEEDED(hr) && spOws)
	{
		CComPtr<IServiceProvider> spSP;
		if (SUCCEEDED(QueryInterface(IID_IServiceProvider, (void**)&spSP) && spSP))
		{
			spOws->SetSite(spSP);
		}
	}
	return 0;
}

LRESULT CUIWebBrowserImpl::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}