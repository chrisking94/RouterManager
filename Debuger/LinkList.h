#pragma once

/*********
单链表
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
	int Append(ElemType e);//追加到链尾并返回链长
	int Append(ElemType *pE,int nLen);//从数组批量追加，nLen指示插入长度
	int RemoveRear();//删除链尾并返回链长，若链为空返回-1
	int InsertHead(ElemType e);//头插，返回链长
	int InsertHead(ElemType *pE,int nLen);//从数组批量插入，nLen指示插入长度
	BOOL GetHead(ElemType &e);//返回第一个节点，空则返回false
	int RemoveHead();//删除头第一个节点，返回链长，若链本空返回-1
	BOOL IsEmpty();
	BOOL Compare(CLinkList &L);//比较两个链表是否相同，相同返回TRUE
	void Clear();//清空
protected:
	LNode m_head;//头节点
	LNode *m_pEnd;//尾指针
	LNode *m_pCur;//当前指针
	int m_nCur;//当前整型指针
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

	//删除所有节点
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
	if(m_nLen==0) return -1;//链空

	LNode* p=m_head.next;
	m_head.next = m_head.next->next;
	delete p;
	if(m_head.next==NULL) m_pEnd=&m_head;//链表为空

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


