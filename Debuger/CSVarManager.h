#pragma once
#include "CSVar.h"

//����������
class CCSVarManager
{
public:
	CCSVarManager(CCSVarManager *parentMgr,int nLstLen = 50);
	~CCSVarManager(void);

	CCSVar *FindVar(char *szName);
	CCSVar::VAR_ID AddVar( CCSVar *pV);//��ӱ������б�
	void SetKeepCount(int nKeep);
	CCSVar *operator[](const int index);//��CVM����һ������
	CCSVar &Var(const int index);
protected:
	int m_nKeep;//����ǰ�������� �����н�����ɾ�������ڱ��溯�����β�
private:
	CCSVar **m_lstPtr;//����ָ���б�
	int m_lstLen;//�б�
	int m_nCount;//������
	int m_nNamedVarCount;//�����ֵı��������������������ᱻ�ŵ��б�β��
	CCSVarManager *m_parentMgr;
};

