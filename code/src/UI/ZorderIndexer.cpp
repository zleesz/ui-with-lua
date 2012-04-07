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

void CZorderIndexer::OnTreeModify(const LPTreeModifyData ptmt)
{
	// resort uiobjects;
	bDirty = TRUE;
}

CUIControlBase* CZorderIndexer::HitTest(int x, int y)
{
	if(NULL == m_pMapID2Control)
		return NULL;
	if(bDirty)
	{
		SortTreeObjectsByZorder();
	}
	for (int i = 0; i < m_vecControl.size(); i++)
	{
		CUIControlBase* pControl = m_vecControl[i];
		if(pControl->OnHitTest(x, y))
		{
			return pControl;
		}
	}
	return NULL;
}

void CZorderIndexer::Render(CDCHandle dc)
{
	if(NULL == m_pMapID2Control)
		return;
	if(bDirty)
	{
		SortTreeObjectsByZorder();
	}
	for (int i = m_vecControl.size() - 1; i >= 0; i--)
	{
		CUIControlBase* pControl = m_vecControl[i];
		if(pControl->GetVisible())
		{
			pControl->Render(dc);
		}
	}
}

// quick sort
void CZorderIndexer::SortTreeObjectsByZorder()
{
	LOG_AUTO();
	if(NULL == m_pMapID2Control)
		return;
	PrintTreeObject();
	ULONG ulMax = 0;
	ID2ControlMap::const_iterator it = m_pMapID2Control->begin();
	for(; it != m_pMapID2Control->end(); it++)
	{
		CUIControlBase* pControl = it->second;
		ULONG ulZorder = pControl->GetZorder();
		// move max zorder object to top;
		if(ulZorder > ulMax)
		{
			ulMax = ulZorder;
			m_vecControl.insert(m_vecControl.begin(), pControl);
		}
		else
		{
			m_vecControl.push_back(pControl);
		}
	}
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
