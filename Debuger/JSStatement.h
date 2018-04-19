#pragma once
#include "JSNameSpace.h"

class CJSNameSpace;

class CJSStatement
{
public:
	const enum SType
	{
		_default,
		_assign,
		_var,
		_if,
		_for,
		_while,
		_do,
		_invoke
	}TYPE;
public:
	CJSStatement(void);
	CJSStatement(CJSNameSpace *pNS,SType t);
	~CJSStatement(void);

	void Execute();
private:
	CString m_szStatment;
	CJSNameSpace *m_pNameSpace;//���ڵ������ռ�
};

class CJSStatAssign:public CJSStatement//��ֵ���
{
public:
	CJSStatAssign(void);
	~CJSStatAssign(void);
	CJSStatAssign(CJSNameSpace *parentNS):CJSStatement(parentNS,_assign){}
	CJSStatAssign(CJSNameSpace *parentNS,SType t):CJSStatement(parentNS,t){}
public:
	CString szVar;//��ֵ������
	CString szExp;//ֵ���ʽ
};

class CJSStatVar:public CJSStatAssign//��������
{
public:
	CJSStatVar(void);
	~CJSStatVar(void);
	CJSStatVar(CJSNameSpace *parentNS):CJSStatAssign(parentNS,_var){}
};

class CJSStatIf:public CJSStatement//If
{
public:
	CJSStatIf(void);
	~CJSStatIf(void);
	CJSStatIf(CJSNameSpace *parentNS):CJSStatement(parentNS,_if){}
	CJSStatIf(CJSNameSpace *parentNS,SType t):CJSStatement(parentNS,t){}
public:
	CJSNameSpace *m_pNS;//if����������ռ�
	CString szCondition;//����
};

class CJSStatWhile:public CJSStatIf
{
public:
	CJSStatWhile(void);
	~CJSStatWhile(void);
	CJSStatWhile(CJSNameSpace *parentNS):CJSStatIf(parentNS,_while){}
};

class CJSStatDo:public CJSStatIf
{
public:
	CJSStatDo(void);
	~CJSStatDo(void);
	CJSStatDo(CJSNameSpace *parentNS):CJSStatIf(parentNS,_do){}
};

class CJSStatFor:public CJSStatIf
{
public:
	CJSStatFor(void);
	~CJSStatFor(void);
	CJSStatFor(CJSNameSpace *parentNS):CJSStatIf(parentNS,_for){}
public:
	CString szSeg1;//for(szSeg1;condition;szSeg3)
	CString szSeg3;
};

class CJSStatInvoke:public CJSStatement//��������
{
public:
	CJSStatInvoke(void);
	~CJSStatInvoke(void);
	CJSStatInvoke(CJSNameSpace *parentNS,CString szInvoke):CJSStatement(parentNS,_invoke),m_szInvoke(szInvoke){}
public:
	CListEx<CString> m_lstPara;
	CString m_szInvoke;
};

