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
		CCSOperator *pOpt;//�����������ǲ����������opt = 0
		ExpNode *lchild;
		ExpNode *rchild;
	}ExpNode,*ExpTree;//���ʽ��

	ExpTree InPostCreateTree(CCSVar **in1,CCSVar **in2,CCSVar **post1,CCSVar **post2);//ͨ���������к������д�����
	void InOrder(ExpTree p);//����������ʽ��������

	void DestroyTree(ExpTree p);//ɾ����

protected:
	ExpTree m_expTree;
private:
	CCSVarManager *m_pCVM;
};

