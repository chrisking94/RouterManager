#pragma once
#include "queue.h"

class CCharQueue :
	public CQueue<char>
{
public:
	CCharQueue(void);
	~CCharQueue(void);
	BOOL Compare(char *s);//���ַ����Ƚ�
	void DeQueueCString(CString &s);//������ַ���
};

