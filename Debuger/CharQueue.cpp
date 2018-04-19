#include "StdAfx.h"
#include "CharQueue.h"


CCharQueue::CCharQueue(void)
{
}


CCharQueue::~CCharQueue(void)
{
}

BOOL CCharQueue::Compare( char *s )
{
	CQueue<char> q;
	q.EnQueue(s,strlen(s));

	return CQueue<char>::Compare(q);
}

void CCharQueue::DeQueueCString( CString &s )
{
	char c=0;

	while(CQueue<char>::DeQueue(c))
	{
		s.AppendChar(c);
	}
}

