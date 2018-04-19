#pragma once
#include "afx.h"
#include "CSVar.h"

class CCSException //:private CMemoryException
{
public:
	typedef enum
	{
		_UnKnownError,//未知错误
		_VarTypeConvert,//类型转换错误
		_BadPointer,
		_UnDeclared,
		_ExcuteError,
		_InternalError,//内部错误
	}ExceptType;
public:
	CCSException(void);
	~CCSException(void);
	CCSException(char *msg);
	CCSException(const ExceptType t);

	CCSVar GetMsg(void) const;
	void AppendMsg(CCSVar msg);
	void Throw();
private:
	CCSVar m_msg;
};

