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
		_int,//short long int 统一为int
		_ptr,//指针
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
	BOOL ForceChangeType(const TYPE t);//强制改变类型，但不改变数据
	char* GetTypeString() const;

	static TYPE GetTypeByString(char * sz);
protected:
	void SetType(const TYPE t);
	TYPE m_type;
	static char *s_typeList[];
};

