#pragma once
#include <MsHtmHst.h>

class ATL_NO_VTABLE CDocHostUIHandlerImpl : public IDocHostUIHandler
{
public:
	STDMETHOD(GetDefaultDocHostUIHandler)(IDocHostUIHandler** ppHandler);

	STDMETHOD(ShowContextMenu)(DWORD dwID, POINT FAR* ppt, IUnknown FAR* pcmdtReserved, IDispatch FAR* pdispReserved)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->ShowContextMenu(dwID, ppt, pcmdtReserved, pdispReserved);
		}

		return E_NOTIMPL;
	}

	STDMETHOD(ShowUI)(DWORD dwID, IOleInPlaceActiveObject FAR* pActiveObject,
		IOleCommandTarget FAR* pCommandTarget,
		IOleInPlaceFrame  FAR* pFrame,
		IOleInPlaceUIWindow FAR* pDoc)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->ShowUI(dwID, pActiveObject, pCommandTarget, pFrame, pDoc);
		}

		return E_NOTIMPL;
	}

	STDMETHOD(GetHostInfo)(DOCHOSTUIINFO FAR *pInfo)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			HRESULT hr = spHandler->GetHostInfo(pInfo);
			return hr;
		}
		return E_NOTIMPL;
	}

	STDMETHOD(HideUI)(void)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->HideUI();
		}

		return E_NOTIMPL;
	}

	STDMETHOD(UpdateUI)(void)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->UpdateUI();
		}

		return E_NOTIMPL;
	}

	STDMETHOD(EnableModeless)(BOOL fEnable)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->EnableModeless(fEnable);
		}

		return E_NOTIMPL;
	}

	STDMETHOD(OnDocWindowActivate)(BOOL fActivate)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->OnDocWindowActivate(fActivate);
		}

		return E_NOTIMPL;
	}

	STDMETHOD(OnFrameWindowActivate)(BOOL fActivate)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->OnFrameWindowActivate(fActivate);
		}

		return E_NOTIMPL;
	}

	STDMETHOD(ResizeBorder)(LPCRECT prcBorder, IOleInPlaceUIWindow FAR* pUIWindow, BOOL fFrameWindow)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->ResizeBorder(prcBorder, pUIWindow, fFrameWindow);
		}

		return E_NOTIMPL;
	}

	STDMETHOD(TranslateAccelerator)(LPMSG lpMsg, const GUID FAR* pguidCmdGroup, DWORD nCmdID)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->TranslateAccelerator(lpMsg, pguidCmdGroup, nCmdID);
		}

		return E_NOTIMPL;
	}

	STDMETHOD(GetOptionKeyPath)(LPOLESTR FAR* pchKey, DWORD dw)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->GetOptionKeyPath(pchKey, dw);
		}

		return E_NOTIMPL;
	}

	STDMETHOD(GetDropTarget)(IDropTarget* pDropTarget, IDropTarget** ppDropTarget)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->GetDropTarget(pDropTarget, ppDropTarget);
		}

		return E_NOTIMPL;
	}

	STDMETHOD(GetExternal)(IDispatch** ppDispatch)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->GetExternal(ppDispatch);
		}

		return E_NOTIMPL;
	}

	STDMETHOD(TranslateUrl)(DWORD dwTranslate, OLECHAR* pchURLIn, OLECHAR** ppchURLOut)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->TranslateUrl(dwTranslate, pchURLIn, ppchURLOut);
		}

		return E_NOTIMPL;
	}

	STDMETHOD(FilterDataObject)(IDataObject* pDO, IDataObject** ppDORet)
	{
		CComPtr<IDocHostUIHandler> spHandler = NULL;
		GetDefaultDocHostUIHandler(&spHandler);
		if (spHandler)
		{
			return spHandler->FilterDataObject(pDO, ppDORet);
		}

		return E_NOTIMPL;
	}
};