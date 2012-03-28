#include "StdAfx.h"
#include "ZorderIndexer.h"

CZorderIndexer::CZorderIndexer(void)
	: m_pMapID2Control(NULL)
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