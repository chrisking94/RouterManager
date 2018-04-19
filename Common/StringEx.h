#include "StdAfx.h"
#include "ArrayEx.h"

#pragma once

class __declspec(dllexport) CStringEx:public CString
{
private:
	const CString m_szLower;
public:
	typedef enum LeftOrRight
	{
		LEFT,
		RIGHT
	}LeftOrRight;

	typedef CArrayEx<CString> CStringArray;
public:
	CStringEx(void);
	CStringEx(LPCTSTR lpszStr);
	CStringEx(CString szStr);
	~CStringEx(void);

	UINT split(CString szToken,CStringEx::CStringArray &szArrayReturned, BOOL bIgnoreCase = TRUE);
	UINT split(CString szToken,CString* &szArrayReturned, BOOL bIgnoreCase = TRUE);
	UINT split(CString szToken,CArrayEx<INT> &nArrayReturned, BOOL bIgnoreCase = TRUE);
	UINT split(CString szToken,INT* &nArrayReturned, BOOL bIgnoreCase = TRUE);
	CString glors(CString szToken,LeftOrRight lorPosition, BOOL bIgnoreCase = TRUE);
	CString mid(CString strLeftToken,CString strRightToken, UINT nStartIndex=0, BOOL bIgnoreCase = TRUE);//如果strLeftToken为空，则从头匹配到strRightToken，同理strRightToken为空...
	UINT mid(CString strLeftToken,CString strRightToken,CString& strRet,UINT nStartIndex=0, BOOL bIgnoreCase = TRUE);//返回strRightToken的'\0'在m_szSourceString对应的索引，如果strLeftToken为空，则从头匹配到strRightToken，同理strRightToken为空...
	void RemoveChineseChars(CString &szRet);//删除所有中文字符
	CString RemoveChineseChars();
};

