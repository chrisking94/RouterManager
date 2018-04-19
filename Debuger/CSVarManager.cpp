#include "StdAfx.h"
#include "CSVarManager.h"
#include "CSException.h"


CCSVarManager::~CCSVarManager(void)
{
	//�������
	for(int i=0;i<m_nCount;i++)
	{
		delete m_lstPtr[i];
	}
	delete [] m_lstPtr;
}

CCSVar * CCSVarManager::FindVar( char *szName )
{
	//���ַ�����
	int low=0,high=m_nNamedVarCount-1;
	int mid;
	char *s;
	int mi = 0,i;//ƥ��λ��

	while(low<=high)
	{
		mid = (low+high)/2;
		s=(m_lstPtr[mid])->GetName();
		if(s[mi]==*szName)//ƥ��һ���ַ�
		{
			//���ϲ���
			for(i= mid;i>=0&&*(m_lstPtr[i]->GetName())==*szName;--i)//��һ���ַ�����ƥ��
				if(m_lstPtr[i]->IsMe(szName))
					return m_lstPtr[i];
			//���²���
			for(i= mid;i<=m_nNamedVarCount&&*(m_lstPtr[i]->GetName())==*szName;++i)//��һ���ַ�����ƥ��
				if(m_lstPtr[i]->IsMe(szName))
					return m_lstPtr[i];
			break;//���û��ƥ�䵽˵������������
		}
		else if(*s>*szName)//�ֲ���
		{
			high = mid - 1;//�����
		}
		else
		{
			low = mid + 1;//���ұ�
		}
	}

	//δ�ҵ�
	if(m_parentMgr) return m_parentMgr->FindVar(szName);//�����ռ����

	return NULL;
}

CCSVar::VAR_ID CCSVarManager::AddVar(  CCSVar *pV )
{
	char *s,*s_n;
	int i,j,k;
	int low=0 , high = m_nNamedVarCount-1 ,mid=0 ,mi;

	++m_nCount;
	if(m_nCount>m_lstLen)//������������
	{
		m_lstLen *= 2;//�����б�ռ�Ϊ2��
		CCSVar **p = new CCSVar*[m_lstLen];
		//��������
		memcpy(p,m_lstPtr,(m_nCount-1)*sizeof(CCSVar*));
		//ɾ��ԭ����ָ��
		delete []m_lstPtr;
		//����ָ��
		m_lstPtr = p;
	}

	s_n = pV->GetName();
	if(s_n==NULL)//��������
	{
		//ֱ�Ӳ��뵽β��
	}
	else
	{
		++m_nNamedVarCount;
		//���ַ�����
		//1 ���Ҳ���λ��
		while(low<=high)
		{
			mid = (low + high)/2;
			s = m_lstPtr[mid]->GetName();

			if(*s_n == *s)
			{
				break;//��mid������
			}
			else if(*s_n>*s)
			{
				low = mid + 1;
			}
			else
			{
				high = mid -1;
			}
		}
	}
	//ֱ����low������
	for(i=m_nCount;i>low;i--) m_lstPtr[i] = m_lstPtr[i-1];//����Ԫ��
	m_lstPtr[low] = pV;//����Ԫ��

	return 0;
}

CCSVarManager::CCSVarManager(CCSVarManager *parentMgr, int nLstLen /*= 50*/ ):
m_lstLen(nLstLen),
	m_nCount(0),
	m_nNamedVarCount(0),
	m_parentMgr(parentMgr)
{
	m_lstPtr = new CCSVar*[m_lstLen];
}

void CCSVarManager::SetKeepCount( int nKeep )
{
	m_nKeep = nKeep;
}

CCSVar * CCSVarManager::operator[]( const int index )
{
	if(index<m_nCount)
	{
		return m_lstPtr[index];
	}
	else
	{
		CCSException e("CScript.VarManager.opt([])������ ");
		e.AppendMsg(index);
		e.AppendMsg("�������� Count=");
		e.AppendMsg(m_nCount);
		e.Throw();
	}

	return NULL;
}

CCSVar & CCSVarManager::Var( const int index )
{
	return *operator[](index);
}
