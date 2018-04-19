#pragma once
#include "afx.h"
#include "CSVar.h"

class CCSException //:private CMemoryException
{
public:
	typedef enum
	{
		_UnKnownError,//δ֪����
		_VarTypeConvert,//����ת������
		_BadPointer,
		_UnDeclared,
		_ExcuteError,
		_InternalError,//�ڲ�����
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

