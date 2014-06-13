#include "StdAfx.h"
#include "ZorderIndexer.h"

CZorderIndexer::CZorderIndexer(void)
	: m_pMapID2Control(NULL), bDirty(TRUE)
{
}

CZorderIndexer::~CZorderIndexer(void)
{
}

void CZorderIndexer::Init(const LPID2ControlMap pMapControl)
{
	ATLASSERT(pMapControl);
	m_pMapID2Control = pMapControl;
}

void CZorderIndexer::OnTreeModify(const LPTreeModifyData /*ptmt*/)
{
	// resort uiobjects;
	bDirty = TRUE;
}

CUIControlBase* CZorderIndexer::HitTest(int x, int y, LONG& nHitTest)
{
	if (NULL == m_pMapID2Control)
		return NULL;
	if (bDirty)
	{
		SortTreeObjectsByZorder();
	}
	for (int i = 0; i < (int)m_vecControl.size(); i++)
	{
		CUIControlBase* pControl = m_vecControl[i];
		if (pControl->GetVisible() && pControl->OnHitTest(x, y, nHitTest))
		{
			return pControl;
		}
	}
	return NULL;
}

void CZorderIndexer::Render(CDCHandle dc)
{
	if (NULL == m_pMapID2Control)
		return;
	if (bDirty)
	{
		SortTreeObjectsByZorder();
	}
	for (int i = (int)m_vecControl.size() - 1; i >= 0; i--)
	{
		CUIControlBase* pControl = m_vecControl[i];
		if (pControl->GetVisible())
		{
			pControl->Render(dc);
		}
	}
}

bool CZorderIndexer::CompareZorder(CUIControlBase* pCtrlA, CUIControlBase* pCtrlB)
{
	return pCtrlA->GetZorder() > pCtrlB->GetZorder();
}

// quick sort
void CZorderIndexer::SortTreeObjectsByZorder()
{
	LOG_AUTO();
	if(NULL == m_pMapID2Control)
		return;
	PrintTreeObject();
	m_vecControl.clear();
	ID2ControlMap::const_iterator it = m_pMapID2Control->begin();
	for(; it != m_pMapID2Control->end(); it++)
	{
		m_vecControl.push_back(it->second);
	}
	std::sort(m_vecControl.begin(), m_vecControl.end(), CZorderIndexer::CompareZorder);
	bDirty = FALSE;
	PrintTreeObject();
}

void CZorderIndexer::PrintTreeObject()
{
#ifdef _DEBUG
	UIControlVec::const_iterator it = m_vecControl.begin();
	for(; it != m_vecControl.end(); it++)
	{
		CUIControlBase* pControl = *it;
		LOG_DEBUG("control=" << pControl << ", id=" << pControl->GetID().c_str() << ", type=" << pControl->GetType() << ", zorder=" << pControl->GetZorder());
	}
#endif
}
