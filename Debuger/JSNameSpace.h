#pragma once
#include "JSVar.h"
#include "ListEx.h"
#include "JSStatement.h"

class CJSStatement;

class CJSNameSpace
{
public:
	CJSNameSpace(void);
	~CJSNameSpace(void);
	CJSNameSpace(CString szCode);

	void Execute();
	int AddVar(const CJSVar &var);
	CJSVar* AppendNewVar();
	CJSVar* GetVar(CString szName);
	CJSVar* GetVar(int nIndex);
	void Parse(CString szSegment);//解析段落
	void SetParent(CJSNameSpace *parent);
private:
	CListEx<CJSVar> m_lstVar;
	CListEx<CJSStatement> m_lstStatement;//语句列表
	CJSNameSpace* m_parent;//父命名空间
};

