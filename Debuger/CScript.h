#pragma once
#include "CSNameSpace.h"
#include "CSFunction.h"
#include "ListEx.h"

class CCScript:protected CCSNameSpace
{
public:
	CCScript(void);
	~CCScript(void);
	CCScript(CString szScript);
	
	BOOL LoadFile(CString szFilePath);
	BOOL Parse(CString szJS=_T(""));//解析
	CCScript(const CCScript &jsSource);
	CCScript& operator=(const CCScript &csSource);
	CCSFunction *operator[](char *szFunction);//用于函数调用，格式如 CScript["func"]->Invoke();
private:
	void PreTreatment(char *szScript);//预处理，处理宏，注释等
	CCSFunction *FindFunction(char * szFunctionName);
	BOOL FindFunction(char * szFunctionName,CCSFunction *&pFuncRet);
protected:
	CString m_szScript;
	CListEx<CCSFunction*> m_lstCSFunction;
};

