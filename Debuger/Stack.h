#pragma once
#include "LinkList.h"

/*********
ջ��
**********/
template  <class ElemType>
class CStack:protected CLinkList<ElemType>
{
public:
	CStack(void);
	~CStack(void);
	void Push(ElemType e);
	void Push(ElemType *pE,int nCount);//������������ջ
	BOOL Pop(ElemType &e);
	ElemType Pop();
	BOOL GetTop(ElemType &e);//ջ���򷵻�False
	ElemType GetTop();
	BOOL IsEmpty();
};

template  <class ElemType>
BOOL CStack<ElemType>::IsEmpty()
{
	return CLinkList::IsEmpty();
}

template  <class ElemType>
CStack<ElemType>::CStack( void )
{
	
}

template  <class ElemType>
CStack<ElemType>::~CStack( void )
{

}

template  <class ElemType>
void CStack<ElemType>::Push( ElemType *pE,int nCount )
{
	CLinkList::InsertHead(pE,nCount);
}

template  <class ElemType>
ElemType CStack<ElemType>::GetTop()
{
	ElemType e;
	GetTop(e);

	return e;
}

template  <class ElemType>
BOOL CStack<ElemType>::GetTop( ElemType &e )
{
	return CLinkList::GetHead(e);
}

template  <class ElemType>
ElemType CStack<ElemType>::Pop()
{
	ElemType e;
	Pop(e);

	return e;
}

template  <class ElemType>
BOOL CStack<ElemType>::Pop( ElemType &e )
{
	BOOL bRet = GetTop(e);
	CLinkList::RemoveHead();

	return bRet;
}

template  <class ElemType>
void CStack<ElemType>::Push( ElemType e )
{
	CLinkList::InsertHead(e);
}

