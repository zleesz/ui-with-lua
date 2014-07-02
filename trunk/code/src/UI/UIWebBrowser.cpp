#include "StdAfx.h"
#include "UIWebBrowser.h"
#include "UIWindowBase.h"

CUIWebBrowser::~CUIWebBrowser(void)
{
	UnRegisterClass(this);
	if (m_pWebBrowserObj)
	{
		m_pWebBrowserObj->Release();
		m_pWebBrowserObj = NULL;
	}
}

CUIWebBrowser::CUIWebBrowser(CUITreeContainer* pTree)
: CUIControlBase(pTree), m_pWebBrowserObj(NULL)
{
	RegisterClass(this);
	HRESULT hr = CComWebBrowserObj::CreateInstance(&m_pWebBrowserObj);
	m_pWebBrowserObj->AddRef();
	m_pWebBrowserObj->SetWebBrowserEvent(this);
	ATLASSERT(SUCCEEDED(hr) && m_pWebBrowserObj);
}

CUIWebBrowser::CUIWebBrowser(CUITreeContainer* pTree, LPXMLDOMNode pNode)
: CUIControlBase(pTree, pNode), m_pWebBrowserObj(NULL)
{
	RegisterClass(this);
	HRESULT hr = CComWebBrowserObj::CreateInstance(&m_pWebBrowserObj);
	m_pWebBrowserObj->AddRef();
	m_pWebBrowserObj->SetWebBrowserEvent(this);
	ATLASSERT(SUCCEEDED(hr) && m_pWebBrowserObj);
	if(pNode == NULL || pNode->pMapAttr == NULL)
		return;
	LPXMLAttrMap pMapAttr = pNode->pMapAttr;
	if(pMapAttr == NULL)
		return;
	LPXMLChildNodes pChildNodes = pNode->pMapChildNode;
	if(pChildNodes == NULL)
		return;
	XMLChildNodes::const_iterator it = pChildNodes->find("attr");
	if(it == pChildNodes->end())
		return;
	LPXMLDOMNode pAttrNode = it->second;
	if(pAttrNode == NULL)
		return;
	LPXMLChildNodes pAttrChildNodes = pAttrNode->pMapChildNode;
	if(pAttrChildNodes == NULL)
		return;
	XMLChildNodes::const_iterator it2 = pAttrChildNodes->find("url");
	if (it2 != pAttrChildNodes->end())
	{
		std::wstring strUrl;
		Util::UTF8_to_Unicode(it2->second->strUData.c_str(), strUrl);
		m_bstrURL = strUrl.c_str();
	}
}

void CUIWebBrowser::Render(CDCHandle /*dc*/)
{
}

BOOL CUIWebBrowser::GetInitVisible()
{
	BOOL bVisible = TRUE;
	CComVariant vVisible;
	GetAttr("visible", &vVisible);
	if(vVisible.vt == VT_I2)
	{
		bVisible = vVisible.boolVal == VARIANT_TRUE ? TRUE : FALSE;
	}
	return bVisible;
}

void CUIWebBrowser::OnInitControl()
{
	m_pWebBrowserObj->Create(GetOwnerTree()->GetBindWnd()->m_hWnd);
	if (GetInitVisible())
	{
		m_pWebBrowserObj->ShowWindow(SW_SHOWNOACTIVATE);
	}
	const RECT rc = GetObjPos();
	m_pWebBrowserObj->MoveWindow(&rc);
	if (m_bstrURL.Length() > 0)
	{
		m_pWebBrowserObj->Navigate(m_bstrURL);
	}
}

void CUIWebBrowser::OnDetroy()
{
	
}

void CUIWebBrowser::OnBeforeNavigate2(IDispatch *pDisp,VARIANT *url,VARIANT *Flags, VARIANT *TargetFrameName,VARIANT *PostData,VARIANT *Headers,VARIANT_BOOL *Cancel)
{
	LOG_AUTO();
}

void CUIWebBrowser::OnNavigatorComplete2(IDispatch *pDisp, VARIANT *URL)
{
	LOG_AUTO();
}

void CUIWebBrowser::OnDocumentComplete(IDispatch* pDisp, VARIANT* URL)
{
	LOG_AUTO();
}

void CUIWebBrowser::OnNavigateError(IDispatch *pDisp,VARIANT *URL,VARIANT *TargetFrameName,VARIANT *StatusCode,VARIANT_BOOL *Cancel)
{
	LOG_AUTO();
}

void CUIWebBrowser::OnWindowClosing(VARIANT_BOOL IsChildWindow,VARIANT_BOOL *Cancel)
{
	LOG_AUTO();
}

void CUIWebBrowser::OnNewWindow2(IDispatch** ppDisp, VARIANT_BOOL* Cancel)
{
	LOG_AUTO();
}

void CUIWebBrowser::OnNewWindow3(IDispatch **ppDisp,VARIANT_BOOL *Cancel,DWORD dwFlags,BSTR bstrUrlContext,BSTR bstrUrl)
{
	LOG_AUTO();
}

void CUIWebBrowser::OnTitleChange(BSTR Text)
{
	LOG_AUTO();
}

void CUIWebBrowser::OnTitleIconChange(BSTR Text)
{
	LOG_AUTO();
}

void CUIWebBrowser::OnStatusTextChange(BSTR Text)
{
	LOG_AUTO();
}

void CUIWebBrowser::OnCommandStateChange(long Command, VARIANT_BOOL Enable)
{
	LOG_AUTO();
}

void CUIWebBrowser::ProgressChange(long Progress, long ProgressMax)
{
	LOG_AUTO();
}

void CUIWebBrowser::OnDownloadBegin()
{
	LOG_AUTO();
}

void CUIWebBrowser::OnDownloadComplete()
{
	LOG_AUTO();
}

void CUIWebBrowser::OnQuit(void)
{
	LOG_AUTO();
}

BOOL CUIWebBrowser::GetVisible()
{
	return m_pWebBrowserObj->IsWindowVisible();
}

void CUIWebBrowser::SetVisible(BOOL bVisible)
{
	m_pWebBrowserObj->ShowWindow(bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
}

void CUIWebBrowser::OnPosChange(int left, int top, int right, int bottom, int /*oldLeft*/, int /*oldTop*/, int /*oldRight*/, int /*oldBottom*/)
{
	m_pWebBrowserObj->MoveWindow(left, top, right - left, bottom - top);
}

void CUIWebBrowser::SetAttr(const std::string& /*strName*/, const std::string& /*strValue*/)
{

}

int CUIWebBrowser::Navigate(lua_State* L)
{
	LOG_AUTO();
	CUIWebBrowser** ppThis = (CUIWebBrowser**)luaL_checkudata(L, -1, GetRigisterClassName());
	CUIWebBrowser* pThis = *ppThis;
	if (!pThis)
	{
		ATLASSERT(pThis);
		return 0;
	}
	const char* pURL = lua_tostring(L, 2);
	if (pURL == NULL)
	{
		return 0;
	}
	std::wstring strURL;
	Util::UTF8_to_Unicode(pURL, strURL);
	pThis->m_bstrURL = strURL.c_str();
	pThis->m_pWebBrowserObj->Navigate(pThis->m_bstrURL);
	return 0;
}