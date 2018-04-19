#pragma once
#include "HtmlLabel.h"
#include "StringEx.h"
class __declspec(dllexport) CHtmlDocument
{
public:
	CHtmlDocument(void);
	CHtmlDocument(CString& szDocumentString);
	CHtmlDocument(LPCTSTR lpszDocumentString);
	~CHtmlDocument(void);

	UINT GetJSArray_ToCStringArray(CString szArrayName , CStringEx::CStringArray& szaReturned);
	CString GetJSArrayItem(CString szArrayName,int nIndex);

	BOOL Parse(TCHAR *&s);
private:
	CHtmlLabel m_html;
	LPCTSTR m_lpszHtml;
};

