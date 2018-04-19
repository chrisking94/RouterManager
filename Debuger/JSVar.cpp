#include "StdAfx.h"
#include "JSVar.h"


CJSVar::CJSVar(void)
{
	m_pVar=NULL;
}


CJSVar::~CJSVar(void)
{
}

CString CJSVar::operator=( const CString szVal )
{
	szValue=szValue;

	if(szValue[0]=='"')//×Ö·û´®
	{
		m_type=_String;
		szValue=szValue.Mid(1,szValue.GetLength()-2);
		if(m_pVar) *(CString*)m_pVar=szValue;
	}else
	{
		m_type=_Number;
	}

	return szVal;
}

int CJSVar::operator=( const int szVal )
{
	m_type=_Number;

	return szVal;
}

CJSVar::VarType CJSVar::GetType()
{
	return m_type;
}

void CJSVar::SetPointer( void *p )
{
	m_pVar=p;
}

