#include "StdAfx.h"
#include "JSStatement.h"


CJSStatement::CJSStatement(void):TYPE(_default)
{
	
}

CJSStatement::CJSStatement( CJSNameSpace *pNS ,SType t):TYPE(t)
{
	m_pNameSpace=pNS;
}


CJSStatement::~CJSStatement(void)
{
	
}

void CJSStatement::Execute()
{

}

