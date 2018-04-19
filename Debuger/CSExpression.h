#pragma once
#include "CSVarManager.h"
#include "CSOperator.h"
#include "CSStringParser.h"

class CCSExpression:protected CCSStringParser
{
public:
	~CCSExpression(void);
	CCSExpression(CCSVarManager *pvm);

	BOOL Parse(char *&s);
	CCSVar *Execute();
private:
	typedef struct ExpNode
	{
		CCSVar *pVar;
		CCSOperator *pOpt;//运算符，如果是操作数则这个opt = 0
		ExpNode *lchild;
		ExpNode *rchild;
	}ExpNode,*ExpTree;//表达式树

	ExpTree InPostCreateTree(CCSVar **in1,CCSVar **in2,CCSVar **post1,CCSVar **post2);//通过中序序列后序序列创建树
	void InOrder(ExpTree p);//中序遍历表达式树并计算

	void DestroyTree(ExpTree p);//删除树

protected:
	ExpTree m_expTree;
private:
	CCSVarManager *m_pCVM;
};

