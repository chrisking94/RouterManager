#pragma once

class CCSVarType
{
public:
	typedef enum
	{
		_nullType = -1,
		_void,
		_char,
		_double,
		_int,//short long int ͳһΪint
		_ptr,//ָ��
		_string,

		_struct,
		_enum,
		_union,
	}TYPE;
public:
	CCSVarType(void);
	~CCSVarType(void);
	CCSVarType(char *szType);
	CCSVarType(const CCSVarType &t);
	CCSVarType(const TYPE t);

	TYPE TypeOf() const{return m_type;}
	BOOL ForceChangeType(const TYPE t);//ǿ�Ƹı����ͣ������ı�����
	char* GetTypeString() const;

	static TYPE GetTypeByString(char * sz);
protected:
	void SetType(const TYPE t);
	TYPE m_type;
	static char *s_typeList[];
};

