#pragma once
#include "ListEx.h"
#include "JSVar.h"
#include "JSNameSpace.h"
#include "JSFunction.h"
#include <MsHTML.h>
#include "WebBrowser1.h"

class CJavaScript
{
public:
	typedef struct
	{
		void* pFunction();//接口函数指针
		CString szFunction;//函数名
	}AppFunctionInterface;
public:
	CJavaScript(void);
	CJavaScript(CString szFilePath);
	void Invoke(CString szFunction,...);
	CJavaScript(const CJavaScript &jsSource);
	CJavaScript& operator=(const CJavaScript &jsSource);
	~CJavaScript(void);
private:
	CListEx<AppFunctionInterface> m_lstFuncInterface;
	CListEx<CJSFunction> m_lstJavaFunction;
	CString m_szJavaScript;
};

