#pragma once
#include "queue.h"

class CCharQueue :
	public CQueue<char>
{
public:
	CCharQueue(void);
	~CCharQueue(void);
	BOOL Compare(char *s);//与字符串比较
	void DeQueueCString(CString &s);//输出到字符串
};

