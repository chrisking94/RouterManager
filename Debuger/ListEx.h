#pragma once
#include "afxtempl.h"

template  <typename TYPE>
class CListEx :
	public CList<typename TYPE>
{
public:
	CListEx(void);
	~CListEx(void);

	TYPE *AppendNew();
	TYPE& operator[](UINT nIndex);
	CListEx(const CListEx<TYPE>& lstSource);
	CListEx<TYPE>& operator=(const CListEx<TYPE>& lstSource);
};

template  <typename TYPE>
CListEx<TYPE>::CListEx( const CListEx<TYPE>& lstSource )
{
	CList::RemoveAll();
	CList::AddTail((CList*)&lstSource);
}

template  <typename TYPE>
CListEx<TYPE>::~CListEx( void )
{

}

template  <typename TYPE>
CListEx<TYPE>::CListEx( void )
{

}

template  <typename TYPE>
CListEx<TYPE>& CListEx<TYPE>::operator=(const CListEx<TYPE>& lstSource )
{
	CList::RemoveAll();
	CList::AddTail((CList*)&lstSource);

	return *this;
}

template  <typename TYPE>
TYPE& CListEx<TYPE>::operator[]( UINT nIndex )
{
	POSITION p=CList::GetHeadPosition();

	while(nIndex--) CList::GetNext(p);
	TRACE("%d",CList::GetCount());
	return CList::GetAt(p);
}

template  <typename TYPE>
TYPE * CListEx<TYPE>::AppendNew()
{
	CList::AddTail(*new TYPE);

	return &CList::GetTail();
}

