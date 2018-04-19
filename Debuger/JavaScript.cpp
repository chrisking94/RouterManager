#include "StdAfx.h"
#include "JavaScript.h"
#include "CharQueue.h"//char队列
#include "Stack.h"
#include "InitCharControl.h"


CJavaScript::CJavaScript(void)
{
}

CJavaScript::CJavaScript( CString szFilePath )
{
	CFile rfile(szFilePath,CFile::modeRead|CFile::shareExclusive);
	BYTE *pBuf=new BYTE[rfile.GetLength()+1];
	rfile.Read(pBuf,rfile.GetLength());
	pBuf[rfile.GetLength()]=0;
	rfile.Close();
	
	m_szJavaScript = CString(pBuf);
	//delete pBuf;
}


CJavaScript::CJavaScript( const CJavaScript &jsSource )
{
	m_lstJavaFunction=jsSource.m_lstJavaFunction;
	m_lstFuncInterface=jsSource.m_lstFuncInterface;
	m_szJavaScript=jsSource.m_szJavaScript;
}


CJavaScript::~CJavaScript(void)
{
}

void CJavaScript::Invoke( CString szFunction,... )
{
	va_list args;

	va_start(args,szFunction);
	
	CWebBrowser1 m_WebBrowser;
	//查找函数
	CComQIPtr<IHTMLDocument2> spDoc = m_WebBrowser.get_Document();
	CComDispatchDriver spScript;
	spDoc->get_Script(&spScript);

	CComVariant var1 = 10, var2 = 20, varRet;
	spScript.Invoke2(L"Add", &var1, &var2, &varRet);

	//va_arg(args,int)
	va_end(args);
}

CJavaScript& CJavaScript::operator=( const CJavaScript &jsSource )
{
	//CJavaScript::CJavaScript(jsSource);
	m_lstJavaFunction=jsSource.m_lstJavaFunction;
	m_lstFuncInterface=jsSource.m_lstFuncInterface;
	m_szJavaScript=jsSource.m_szJavaScript;

	return *this;
}
