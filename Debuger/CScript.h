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
	BOOL Parse(CString szJS=_T(""));//����
	CCScript(const CCScript &jsSource);
	CCScript& operator=(const CCScript &csSource);
	CCSFunction *operator[](char *szFunction);//���ں������ã���ʽ�� CScript["func"]->Invoke();
private:
	void PreTreatment(char *szScript);//Ԥ��������꣬ע�͵�
	CCSFunction *FindFunction(char * szFunctionName);
	BOOL FindFunction(char * szFunctionName,CCSFunction *&pFuncRet);
protected:
	CString m_szScript;
	CListEx<CCSFunction*> m_lstCSFunction;
};

