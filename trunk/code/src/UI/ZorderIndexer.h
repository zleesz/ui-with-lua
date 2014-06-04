#pragma once
#include <string>
#include "UIControlBase.h"

typedef std::map<std::string, CUIControlBase*> ID2ControlMap, *LPID2ControlMap;
typedef enum emunTreeModifyType
{
	TMT_DEFAULT = 0,
	TMT_ADD,
	TMT_DELETE,
}TreeModifyType;

typedef struct tagTreeModifyData
{
	TreeModifyType t;
	CUIControlBase* pControl;
}TreeModifyData, *LPTreeModifyData;

class CZorderIndexer
{
public:
	CZorderIndexer(void);
	virtual ~CZorderIndexer(void);
public:
	void Init(const LPID2ControlMap pMapControl);
	void Render(CDCHandle dc);
	void OnTreeModify(const LPTreeModifyData ptmt);
	CUIControlBase* HitTest(int x, int y);
private:
	static bool CompareZorder(CUIControlBase* pCtrlA, CUIControlBase* pCtrlB);
	void SortTreeObjectsByZorder();
	void PrintTreeObject();
	BOOL bDirty;
private:
	LPID2ControlMap m_pMapID2Control;
	typedef std::vector<CUIControlBase*> UIControlVec;
	UIControlVec m_vecControl;
public:
	LOG_CLS_DEC();
};
