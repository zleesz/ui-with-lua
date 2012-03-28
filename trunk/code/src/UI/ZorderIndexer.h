#pragma once
#include <string>
#include "UIControlBase.h"

typedef std::map<std::string, CUIControlBase*> ID2ControlMap, *LPID2ControlMap;
class CZorderIndexer
{
public:
	CZorderIndexer(void);
	virtual ~CZorderIndexer(void);
public:
	void Init(const LPID2ControlMap pMapControl);
private:
	LPID2ControlMap m_pMapID2Control;
};
