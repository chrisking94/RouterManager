#include "StdAfx.h"
#include "CSVarType.h"
#include "StrParallelCmp.h"

CCSVarType::CCSVarType(void):m_type(_void)
{
}

CCSVarType::CCSVarType( char *szType )
{
	m_type = GetTypeByString(szType);
}

CCSVarType::CCSVarType( const TYPE t )
{
	SetType(t);
}

CCSVarType::CCSVarType( const CCSVarType &t )
{
	//类型转换
	m_type = t.m_type;
}


CCSVarType::~CCSVarType(void)
{

}

void CCSVarType::SetType( const TYPE t )
{
	m_type = t;
}

BOOL CCSVarType::ForceChangeType( const TYPE t )
{
	m_type = t;
	return TRUE;
}

CCSVarType::TYPE CCSVarType::GetTypeByString( char * sz )
{
	CStrParallelCmp spc(sz);
	char *sBkup;
	spc.SetList(s_typeList,9);

	int n = spc.Compare(sBkup);

	switch(n)
	{
	case  -1:
		return (_nullType);
	case 0:
		return (_void);
	case 1:
		return (_char);
	case 2:
	case 3:
		return (_double);
	case 4:
	case 5:
	case 7:
		return _int;
	case 6:
		return _ptr;
	case 8:
		return _string;
	}
}

char* CCSVarType::GetTypeString( ) const
{
	return s_typeList[m_type];
}

char * CCSVarType::s_typeList[]=
{
	"void",
	"char",
	"double",
	"float",
	"int",
	"long",
	"ptr",//指针
	"short",
	"string",
};

