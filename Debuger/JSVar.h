#pragma once
class CJSVar
{
public:
	CString szName;
	CString szValue;
	int nValue;
	typedef enum
	{
		_String,
		_Number,
		_Array,
	}VarType;
public:
	CJSVar(void);
	~CJSVar(void);
	CJSVar(CString szName);

	CString operator=(const CString szVal);
	int operator=(const int szVal);
	VarType GetType();
	void GetValue(CString &val);
	void SetPointer(void *p);
private:
	VarType m_type;
	void *m_pVar;//指向的C++中的某个变量
};

