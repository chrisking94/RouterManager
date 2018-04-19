#pragma once
#include "CSVar.h"

//变量管理类
class CCSVarManager
{
public:
	CCSVarManager(CCSVarManager *parentMgr,int nLstLen = 50);
	~CCSVarManager(void);

	CCSVar *FindVar(char *szName);
	CCSVar::VAR_ID AddVar( CCSVar *pV);//添加变量到列表
	void SetKeepCount(int nKeep);
	CCSVar *operator[](const int index);//把CVM看做一个数组
	CCSVar &Var(const int index);
protected:
	int m_nKeep;//保持前几个变量 在运行结束后不删除，用于保存函数的形参
private:
	CCSVar **m_lstPtr;//变量指针列表
	int m_lstLen;//列表长
	int m_nCount;//变量数
	int m_nNamedVarCount;//有名字的变量变量数，无名变量会被放到列表尾部
	CCSVarManager *m_parentMgr;
};

