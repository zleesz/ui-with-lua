#pragma once
#include <atlwin.h>
#include <ExDispId.h>
#include <ExDisp.h>
#include "DocHostUIHandler.h"
#include "DownloadManager.h"

class CUIWebBrowserImpl;

#define DISPID_WEBBROWSER 1
typedef IDispEventImpl<DISPID_WEBBROWSER, CUIWebBrowserImpl, &DIID_DWebBrowserEvents2, &LIBID_SHDocVw, 1, 0> DWebBrowserEventImpl;

class CUIWebBrowserImpl : 
	public CWindowImpl<CUIWebBrowserImpl, CAxWindow>, 
	public CComObjectRootEx<CComSingleThreadModel>,
	public DWebBrowserEventImpl,
	public CDocHostUIHandlerImpl,
	public IOleCommandTarget,
	public IServiceProviderImpl<CUIWebBrowserImpl>,
	public IDownloadManager
{
public:
	CUIWebBrowserImpl();
	~CUIWebBrowserImpl(void);

	BEGIN_COM_MAP(CUIWebBrowserImpl)
		COM_INTERFACE_ENTRY(IDocHostUIHandler)
		COM_INTERFACE_ENTRY(IOleCommandTarget)
		COM_INTERFACE_ENTRY(IServiceProvider)
		COM_INTERFACE_ENTRY(IDownloadManager)
	END_COM_MAP()

	BEGIN_SERVICE_MAP(CUIWebBrowserImpl)
		SERVICE_ENTRY(SID_SDownloadManager)
	END_SERVICE_MAP()

	BEGIN_SINK_MAP(CUIWebBrowserImpl)
		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, OnBeforeNavigate2)
		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_NAVIGATECOMPLETE2, OnNavigatorComplete2)
		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_NAVIGATEERROR, OnNavigateError)
		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_NEWWINDOW2, OnNewWindow2) 
		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_NEWWINDOW3, OnNewWindow3)

		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_TITLECHANGE, OnTitleChange)
		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_STATUSTEXTCHANGE, OnStatusTextChange)
		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_COMMANDSTATECHANGE, OnCommandStateChange)
		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_PROGRESSCHANGE, ProgressChange)

		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_DOWNLOADBEGIN, OnDownloadBegin)
		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_DOWNLOADCOMPLETE, OnDownloadComplete)
		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_WINDOWCLOSING, OnWindowClosing)

		SINK_ENTRY_EX(DISPID_WEBBROWSER, DIID_DWebBrowserEvents2, DISPID_QUIT, OnQuit)
	END_SINK_MAP()

protected:
	// DWebBrowserEvents2
	void __stdcall OnBeforeNavigate2(IDispatch *pDisp,VARIANT *url,VARIANT *Flags, VARIANT *TargetFrameName,VARIANT *PostData,VARIANT *Headers,VARIANT_BOOL *Cancel);
	void __stdcall OnNavigatorComplete2(IDispatch *pDisp, VARIANT *URL);
	void __stdcall OnDocumentComplete(IDispatch* pDisp, VARIANT* URL);
	void __stdcall OnNavigateError(IDispatch *pDisp,VARIANT *URL,VARIANT *TargetFrameName,VARIANT *StatusCode,VARIANT_BOOL *Cancel);
	void __stdcall OnWindowClosing(VARIANT_BOOL IsChildWindow,VARIANT_BOOL *Cancel);

	void __stdcall OnNewWindow2(IDispatch** ppDisp, VARIANT_BOOL* Cancel);
	void __stdcall OnNewWindow3(IDispatch **ppDisp,VARIANT_BOOL *Cancel,DWORD dwFlags,BSTR bstrUrlContext,BSTR bstrUrl);

	void __stdcall OnTitleChange(BSTR Text);
	void __stdcall OnTitleIconChange(BSTR Text);
	void __stdcall OnStatusTextChange(BSTR Text);
	void __stdcall OnCommandStateChange(long Command, VARIANT_BOOL Enable);
	void __stdcall ProgressChange(long Progress, long ProgressMax);

	void __stdcall OnDownloadBegin();
	void __stdcall OnDownloadComplete();

	void __stdcall OnQuit(void);

	//IDocHostUIHandlerDispatch
	STDMETHOD(GetExternal)(IDispatch **ppDispatch);

	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT FAR* ppt, IUnknown FAR* pcmdtReserved, IDispatch FAR* pdispReserved);

	STDMETHOD(GetDropTarget)(IDropTarget* pDropTarget, IDropTarget** ppDropTarget);

	// IOleCommandTarget
	STDMETHOD(QueryStatus)(const GUID *pguidCmdGroup,ULONG cCmds,OLECMD prgCmds[],OLECMDTEXT *pCmdText);

	STDMETHOD(Exec)(const GUID *pguidCmdGroup,DWORD nCmdID,DWORD nCmdexecopt,VARIANT *pvaIn,VARIANT *pvaOut);

	STDMETHOD(GetDefaultDocHostUIHandler)(IDocHostUIHandler** ppHandler);

	// INewWindowManager
	STDMETHOD(EvaluateNewWindow)(LPCWSTR pszUrl,LPCWSTR pszName,LPCWSTR pszUrlContext,LPCWSTR pszFeatures,BOOL fReplace,DWORD dwFlags,DWORD dwUserActionTime);

	// IDownloadManager
	STDMETHOD(Download)( IMoniker *pmk, IBindCtx * pbc,DWORD dwBindVerb, LONG grfBINDF,BINDINFO * pBindInfo,LPCOLESTR pszHeaders,LPCOLESTR pszRedir, UINT uiCP);

protected:
	CComPtr<IDocHostUIHandler> m_spDefaultDocHostUIHandler;
	CComPtr<IOleCommandTarget> m_spDefaultOleCommandTarget;
	CComPtr<IObjectWithSite> m_spImgEmbedDetect;

public:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	BEGIN_MSG_MAP(CUIWebBrowserImpl)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
	END_MSG_MAP()
};
