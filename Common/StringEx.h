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
	CString mid(CString strLeftToken,CString strRightToken, UINT nStartIndex=0, BOOL bIgnoreCase = TRUE);//���strLeftTokenΪ�գ����ͷƥ�䵽strRightToken��ͬ��strRightTokenΪ��...
	UINT mid(CString strLeftToken,CString strRightToken,CString& strRet,UINT nStartIndex=0, BOOL bIgnoreCase = TRUE);//����strRightToken��'\0'��m_szSourceString��Ӧ�����������strLeftTokenΪ�գ����ͷƥ�䵽strRightToken��ͬ��strRightTokenΪ��...
	void RemoveChineseChars(CString &szRet);//ɾ�����������ַ�
	CString RemoveChineseChars();
};

