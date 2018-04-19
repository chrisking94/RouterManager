#pragma once

/*********
������
**********/
template  <class ElemType>
class CLinkList
{
public:
	typedef struct LNode
	{
		ElemType data;
		LNode *next;
	}LNode;
public:
	CLinkList(void);
	~CLinkList(void);
	int Append(ElemType e);//׷�ӵ���β����������
	int Append(ElemType *pE,int nLen);//����������׷�ӣ�nLenָʾ���볤��
	int RemoveRear();//ɾ����β����������������Ϊ�շ���-1
	int InsertHead(ElemType e);//ͷ�壬��������
	int InsertHead(ElemType *pE,int nLen);//�������������룬nLenָʾ���볤��
	BOOL GetHead(ElemType &e);//���ص�һ���ڵ㣬���򷵻�false
	int RemoveHead();//ɾ��ͷ��һ���ڵ㣬�����������������շ���-1
	BOOL IsEmpty();
	BOOL Compare(CLinkList &L);//�Ƚ����������Ƿ���ͬ����ͬ����TRUE
	void Clear();//���
protected:
	LNode m_head;//ͷ�ڵ�
	LNode *m_pEnd;//βָ��
	LNode *m_pCur;//��ǰָ��
	int m_nCur;//��ǰ����ָ��
	int m_nLen;
};

template  <class ElemType>
CLinkList<ElemType>::CLinkList( void )
{
	m_pEnd = &m_head;
	m_head.next = NULL;
	m_pCur = NULL;
	m_nLen=0;
	m_nCur=-1;
}

template  <class ElemType>
void CLinkList<ElemType>::Clear()
{
	LNode* p=m_head.next,*u;

	//ɾ�����нڵ�
	while(p)
	{
		u=p;
		p=p->next;
		delete u;
	}

	m_head.next=NULL;
	m_pCur=NULL;
	m_pEnd=&m_head;
	m_nLen=0;
	m_nCur=-1;
}

template  <class ElemType>
int CLinkList<ElemType>::InsertHead( ElemType *pE,int nLen )
{
	for(int i=0;i<nLen;i++)
	{
		InsertHead(pE[i]);
	}

	return m_nLen;
}

template  <class ElemType>
int CLinkList<ElemType>::Append( ElemType *pE,int nLen )
{
	for(int i=0;i<nLen;i++)
	{
		Append(pE[i]);
	}

	return m_nLen;
}

template  <class ElemType>
BOOL CLinkList<ElemType>::Compare( CLinkList &L )
{
	LNode *p=m_head.next,*q=L.m_head.next;

	while(p&&q)
	{
		if(p->data==q->data)
		{
			p=p->next;
			q=q->next;
		}
		else
			return FALSE;
	}

	if(p||q) return FALSE;
	return TRUE;
}

template  <class ElemType>
BOOL CLinkList<ElemType>::GetHead( ElemType &e )
{
	if(m_head.next) e=m_head.next->data;
	else return FALSE;
	return TRUE;
}

template  <class ElemType>
BOOL CLinkList<ElemType>::IsEmpty()
{
	return !m_head.next;
}

template  <class ElemType>
int CLinkList<ElemType>::RemoveHead()
{
	if(m_nLen==0) return -1;//����

	LNode* p=m_head.next;
	m_head.next = m_head.next->next;
	delete p;
	if(m_head.next==NULL) m_pEnd=&m_head;//����Ϊ��

	return --m_nLen;
}

template  <class ElemType>
int CLinkList<ElemType>::InsertHead( ElemType e )
{
	LNode *p = new LNode;

	p->next = m_head.next;
	m_head.next = p;
	if(m_pEnd==&m_head) m_pEnd = p;

	return ++m_nLen;
}

template  <class ElemType>
int CLinkList<ElemType>::RemoveRear()
{
	if(m_nLen==0) return -1;


}

template  <class ElemType>
int CLinkList<ElemType>::Append( ElemType e )
{
	LNode *ln = new LNode;

	ln->data = e;
	m_pEnd->next=ln;
	m_pEnd=ln;

	return ++m_nLen;
}

template  <class ElemType>
CLinkList<ElemType>::~CLinkList( void )
{
	Clear();
}


