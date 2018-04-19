#include "StdAfx.h"
#include "HtmlDocument.h"
#include "Regex.h"


CHtmlDocument::CHtmlDocument(void)
{
}

CHtmlDocument::CHtmlDocument( CString& szDocumentString ):
m_lpszHtml(szDocumentString)
{
	TCHAR *s = (TCHAR*)m_lpszHtml;
	JUMP_WHITE_CHARS;
	if(*(s+1)=='!')// <!
		while(*s++!='>');
	Parse(s);
}

CHtmlDocument::CHtmlDocument( LPCTSTR lpszDocumentString ):
m_lpszHtml(lpszDocumentString)
{
	TCHAR *s = (TCHAR*)m_lpszHtml;
	JUMP_WHITE_CHARS;
	if(*(s+1)=='!')// <!
		while(*s++!='>');
	Parse(s);
}


CHtmlDocument::~CHtmlDocument(void)
{
}

UINT CHtmlDocument::GetJSArray_ToCStringArray( CString szArrayName , CStringEx::CStringArray& szaReturned )
{
	
	return 0;
}

CString CHtmlDocument::GetJSArrayItem( CString szArrayName,int nIndex )
{
	
	return _T("");
}

BOOL CHtmlDocument::Parse(TCHAR *&s)
{
	JUMP_WHITE_CHARS;
	if(*(s+1)=='!') //<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
	{
		while(*s++!='>');
	}
	JUMP_WHITE_CHARS;

	return m_html.Parse(s);
}

