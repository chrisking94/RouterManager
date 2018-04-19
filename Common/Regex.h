#pragma once

#include "deelx.h"

class __declspec(dllexport) CRegex
{
public:
	CRegex(void);
	CRegex(CString szSourceString);
	~CRegex(void);

	void match(CString szRegex , INT nRegexOptions = NO_FLAG);
	BOOL getMatchString(CString &szReturned,INT nGroupIndex=0);
	CString getMatchString(INT nGroupIndex=0);
	typedef enum REGEX_FLAGS RegexOptions;
private:
	/* ���Ա�У���������Ĭ�����������Զ��ͷ��ڴ棬��ָ���ڴ���Ҫ�Զ������������ͷ� */
	/* Ŀǰ�������⣺ɾ��ָ��ʱ�ᵼ�������󡣲��룺mfc�����Զ�����ָ���Ա */
	CString m_szSourceString;
	const char* m_pchSourceString;
	MatchResult m_mrMatchResult;
	CRegexpT <char> *m_pRgxp;
};

