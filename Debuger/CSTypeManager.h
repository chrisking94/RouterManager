#pragma once
#include "CSVarType.h"
//�������͹���
class CCSTypeManager
{
public:
	CCSTypeManager(void);
	~CCSTypeManager(void);
	CCSTypeManager(int nListLen);

	CCSVarType::TYPE GetTypeByString(char *s);//��"char" -> char
	//�����CSVarType��֮ͬ�����ڣ������ҵ�struct,enum,typedef���������
protected:
	void *m_pns;
};

