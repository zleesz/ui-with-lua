#include "StdAfx.h"
#include "UIWebBrowserImpl.h"

CUIWebBrowserImpl::CUIWebBrowserImpl(void)
{
}

CUIWebBrowserImpl::~CUIWebBrowserImpl(void)
{
}

void __stdcall CUIWebBrowserImpl::OnBeforeNavigate2(IDispatch *pDisp,VARIANT *url,VARIANT *Flags, VARIANT *TargetFrameName,VARIANT *PostData,VARIANT *Headers,VARIANT_BOOL *Cancel)
{
}

void __stdcall CUIWebBrowserImpl::OnNavigatorComplete2(IDispatch *pDisp, VARIANT *URL)
{
}

void __stdcall CUIWebBrowserImpl::OnDocumentComplete(IDispatch* pDisp, VARIANT* URL)
{
}

void __stdcall CUIWebBrowserImpl::OnNavigateError(IDispatch *pDisp,VARIANT *URL,VARIANT *TargetFrameName,VARIANT *StatusCode,VARIANT_BOOL *Cancel)
{
}

void __stdcall CUIWebBrowserImpl::OnWindowClosing(VARIANT_BOOL IsChildWindow,VARIANT_BOOL *Cancel)
{
}

void __stdcall CUIWebBrowserImpl::OnNewWindow2(IDispatch** ppDisp, VARIANT_BOOL* Cancel)
{
}

void __stdcall CUIWebBrowserImpl::OnNewWindow3(IDispatch **ppDisp,VARIANT_BOOL *Cancel,DWORD dwFlags,BSTR bstrUrlContext,BSTR bstrUrl)
{
}

void __stdcall CUIWebBrowserImpl::OnTitleChange(BSTR Text)
{
}

void __stdcall CUIWebBrowserImpl::OnTitleIconChange(BSTR Text)
{
}

void __stdcall CUIWebBrowserImpl::OnStatusTextChange(BSTR Text)
{
}

void __stdcall CUIWebBrowserImpl::OnCommandStateChange(long Command, VARIANT_BOOL Enable)
{
}

void __stdcall CUIWebBrowserImpl::ProgressChange(long Progress, long ProgressMax)
{
}


void __stdcall CUIWebBrowserImpl::OnDownloadBegin()
{
}

void __stdcall CUIWebBrowserImpl::OnDownloadComplete()
{
}

void __stdcall CUIWebBrowserImpl::OnQuit(void)
{
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

// INewWindowManager
STDMETHODIMP CUIWebBrowserImpl::EvaluateNewWindow(LPCWSTR pszUrl,LPCWSTR pszName,LPCWSTR pszUrlContext,LPCWSTR pszFeatures,BOOL fReplace,DWORD dwFlags,DWORD dwUserActionTime)
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
	return 0;
}

LRESULT CUIWebBrowserImpl::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}