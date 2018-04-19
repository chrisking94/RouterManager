#pragma once
#include "CSVarType.h"
//变量类型管理
class CCSTypeManager
{
public:
	CCSTypeManager(void);
	~CCSTypeManager(void);
	CCSTypeManager(int nListLen);

	CCSVarType::TYPE GetTypeByString(char *s);//如"char" -> char
	//这个与CSVarType不同之处在于：可以找到struct,enum,typedef定义的类型
protected:
	void *m_pns;
};

