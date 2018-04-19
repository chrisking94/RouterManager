#pragma once
#include "CSNameSpace.h"
#include "CSStatement.h"

class CCSNameSpace;

class CCSFunction:
	public CCSNameSpace
	,public CCSStatement
{
public:
	~CCSFunction(void);
	CCSFunction(CCSNameSpace *parentNS,CCSStatDeclare *pdlc);

	BOOL Parse(char *&s);
	CCSVar *Invoke(char *szParams,...);
	BOOL IsMe(char *szName);
private:
	CCSVar m_func;
};

