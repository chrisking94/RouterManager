#pragma once
#include "CharControl.h"
#include "StrParallelCmp.h"
#include "ListEx.h"
#include "CSNameSpace.h"
#include "CSVar.h"
#include "CSExpression.h"
#include "CSStringParser.h"
#include "CSCodeManager.h"

class CCSNameSpace;

#define JUMP_WHITE_CHARS while(*s==' '||*s=='	'||*s=='\r'||*s=='\n') s++ //�����հ��ַ�

using namespace CM;

class CCSStatement
{
public:
	typedef enum
	{
		_null,
		_macros,
		_expr,
		_declare,
		_if,
		_for,
		_while,
		_do,
		_invoke,
		_function,
		_typedef,
		_return,
		_break,
		_continue,
	}SType;
public:
	CCSStatement(void);
	~CCSStatement(void);
	CCSStatement(CCSNameSpace *parentNameSpace);

	SType GetType();
	virtual SRCode Execute(CCSVar *&pVarRet);

	static BOOL Parse(char *&s,CCSStatement *&p_statRet,CCSNameSpace *pNS,char endWithChar=';');
protected:
	CCSNameSpace *m_parentNS;//���������ռ�
	SType m_statType;
};

class CCSStatExp :
	public CCSExpression
	,public CCSStatement
{
public:
	CCSStatExp(CCSNameSpace *parentNameSpace);
	~CCSStatExp(void);
	BOOL Parse(char *&s);
	SRCode Execute(CCSVar *&pVarRet);
};

class CCSStatDeclare:
	public CCSStatement
	,public CCSStringParser
{
public:
	CCSStatDeclare(CCSNameSpace *parentNameSpace);
	~CCSStatDeclare();
	CCSStatDeclare(const CCSStatDeclare &d);

	BOOL Parse(char *&s );
	SRCode Execute(CCSVar *&pVarRet);

	//��Щ�ӿ���������������
	CCSVarType::TYPE GetVarType(){return m_vt;}
	char *GetFirstVarNamer(){ return m_lstDelc->szVName;}
	int GetFirstVarPtrLevel(){return m_lstDelc->nPtrLevel;}
private:
	typedef struct DeclNode
	{
		char *szVName;
		int nPtrLevel;
		int nArrSize;//�����С
		CCSExpression *pAssiExpr;//��ʼ����ֵ���ʽ
		DeclNode *pNext;
	}*DelcList;
protected:
	CCSVarType::TYPE m_vt;
	DelcList m_lstDelc;
};

class CCSStatJudge://�ж�ִ����䣬��Ϊif while do for�Ļ���
	public CCSStatExp
{
public:
	CCSStatJudge(CCSNameSpace *parentNameSpace);
	~CCSStatJudge();

	BOOL Parse(char *&s);//�����������ռ�
	BOOL Execute(BOOL bJudge);
private:
	SRCode Execute(CCSVar *&pVarRet);
protected:
	CCSNameSpace *m_pNS;
};

class CCSStatIf://If���
	public CCSStatJudge
{
public:
	CCSStatIf(CCSNameSpace *parentNameSpace);
	~CCSStatIf();

	BOOL Parse(char *&s);
	SRCode Execute(CCSVar *&pVarRet);
protected:
	CCSStatIf *m_pElse;//else if(){} else {}
};

class CCSStatWhile:public CCSStatJudge//while
{
public:
	CCSStatWhile(CCSNameSpace *parentNameSpace);
	~CCSStatWhile();
	BOOL Parse(char *&s);
	SRCode Execute(CCSVar *&pVarRet);
};

class CCSStatDo:public CCSStatJudge//do
{
public:
	CCSStatDo(CCSNameSpace *parentNameSpace);
	~CCSStatDo();
	BOOL Parse(char *&s);
	SRCode Execute(CCSVar *&pVarRet);
};

class CCSStatFor:public CCSStatJudge//for
{
public:
	CCSStatFor(CCSNameSpace *parentNameSpace);
	~CCSStatFor();
	BOOL Parse(char *&s);
	SRCode Execute(CCSVar *&pVarRet);
protected:
	CCSStatExp m_ExpInit;
	CCSStatExp m_ExpChange;
};

class CCSStatTypedef:public CCSStatement//typedef
{
public:
	typedef enum
	{
		_typedef,
		_struct,
		_enum,
		_union,
	}TType;
	CCSStatTypedef();
	~CCSStatTypedef();

	BOOL Parse(char *&s);
protected:
	TType m_type;
	CString m_szTypeName;
};

class CCSStatTypeStruct:public CCSStatTypedef//�ṹ��
{
public:
	CCSStatTypeStruct();
	~CCSStatTypeStruct();

	BOOL Parse(char *&s);
};

class CCSStatTypeEnum:public CCSStatTypedef//ö��
{
public:
	CCSStatTypeEnum();
	~CCSStatTypeEnum();

	BOOL Parse(char *&s);
protected:
	typedef struct
	{
		CString szEnum;//ö������
		int v;//ֵ
	}EnumItem;
	CListEx<EnumItem> m_lstEnum;
};

class CCSStatInvoke:
	public CCSStatement
	,private CCSExpression
{
public:
	CCSStatInvoke(CCSNameSpace *parentNameSpace);
	~CCSStatInvoke();

	BOOL Parse(char *&s);
};

class CCSStatReturn:
	public CCSStatement//return
	,public CCSExpression
{
public:
	CCSStatReturn(CCSNameSpace *parentNameSpace);
	~CCSStatReturn();

	SRCode Execute(CCSVar *&pVarRet);
	BOOL Parse(char *&s);
};

class CCSStatBreak://break���
	public CCSStatement
{
public:
	CCSStatBreak(CCSNameSpace *parentNameSpace);
	~CCSStatBreak();

	SRCode Execute(CCSVar *&pVarRet);
	BOOL Parse(char *&s);
};

class CCSStatContinue://break���
	public CCSStatement
{
public:
	CCSStatContinue(CCSNameSpace *parentNameSpace);
	~CCSStatContinue();

	SRCode Execute(CCSVar *&pVarRet);
	BOOL Parse(char *&s);
};