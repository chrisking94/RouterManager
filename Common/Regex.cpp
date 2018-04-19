#include "StdAfx.h"
#include "Regex.h"
#include <afxpriv.h>


CRegex::CRegex(void)
{
}

CRegex::CRegex( CString szSourceString ):
m_szSourceString(szSourceString)
{
}


CRegex::~CRegex(void)
{
}

void CRegex::match( CString szRegex ,INT nRegexOptions)
{
	USES_CONVERSION;
	m_pRgxp=new CRegexpT <char> (T2A(szRegex.GetBuffer(0)),nRegexOptions);//Unicode转ansi
	szRegex.ReleaseBuffer();
}

BOOL CRegex::getMatchString( CString &szReturned,INT nGroupIndex/*=0*/ )
{
	USES_CONVERSION;
	char *buffer = T2A(m_szSourceString);//这个buffer是一个副本，m_szSourceString复制出来的

	szReturned=_T("");

	m_mrMatchResult = m_pRgxp->Match(buffer);
	if(m_mrMatchResult.IsMatched())
	{
		buffer[m_mrMatchResult.GetGroupEnd(nGroupIndex)]=0;
		buffer+=m_mrMatchResult.GetGroupStart(nGroupIndex);

		szReturned.Append(A2T(buffer));
		
		return TRUE;
	}
	
	return FALSE;
}

CString CRegex::getMatchString( INT nGroupIndex/*=0*/ )
{
	CString szTmp;

	getMatchString(szTmp,nGroupIndex);

	return szTmp;
}
