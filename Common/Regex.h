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
	/* 类成员中，类对象会由默认析构函数自动释放内存，但指针内存需要自定义析构函数释放 */
	/* 目前遇到问题：删除指针时会导致类存错误。猜想：mfc类能自动回收指针成员 */
	CString m_szSourceString;
	const char* m_pchSourceString;
	MatchResult m_mrMatchResult;
	CRegexpT <char> *m_pRgxp;
};

