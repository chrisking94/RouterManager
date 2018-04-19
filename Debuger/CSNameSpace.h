#pragma once
#include "ListEx.h"
#include "CSStatement.h"
#include "CSVar.h"
#include "CSVarManager.h"
#include "CSTypeManager.h"
#include "CSCodeManager.h"

class CCSStatement;

using namespace CM;

class CCSNameSpace:
	public CCSVarManager,
	public CCSTypeManager
{
public:
	~CCSNameSpace(void);
	CCSNameSpace(CCSNameSpace *parentNS);

	BOOL Parse(char *&s);
	void AppendStat(CCSStatement *pStat);
	SRCode Execute(CCSVar *&pVarRet);

	CCSNameSpace *GetTopNamespace();//����CScript��Namespace
protected:
	CListEx<CCSStatement*> m_lstStat;
	CCSNameSpace *m_parentNameSpace;
	int m_statCount;//����δ���뵽�б�����
};

