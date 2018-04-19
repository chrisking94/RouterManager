#pragma once

//运算符管理类
class CCSOperator
{
public:
	typedef enum
	{
		_Sharp,
		_PostPlusPlus,_PostMinusMinus,_LSquareBracket,_RSqureBracket,_LBracket,_RBracket,_Dot,_Pointer
		,_NegativeSign,_ForceTypeChange,_PrePlusPlus,_PreMinusMinus,_Analyze,_GetAddress,_Not,_BitWiseNot,_SizeOf//-为负号，tc为强制类型转换，前置++，so为sizeof
		,_Div,_Mul,_Mod
		,_Add,_Sub
		,_Lshift,_Rshift
		,_GR,_GE,_LS,_LE
		,_Equ,_NEqu
		,_BAnd
		,_BEOR
		,_BOr
		,_And
		,_Or
		,_ConditionExpression
		,_Assign,_ADiv,_AMul,_AMod,_AAdd,_ASub,_ALShift,_ARShift,_ABAnd,_ABEor,_ABOr
		,_Comma,_At//多目,doller为char$a;//声明运算符
	}OPT_ID;

	typedef enum
	{
		_null//多目
		,_single//单目
		,_double//双目
		,_triple//三目
	}Mesh_Number;//操作数目数

	typedef enum
	{
		_Ignore,//忽略
		_Right2Left,
		_Left2Right,
	}Associativity;//结合性
public:
	CCSOperator(void);
	~CCSOperator(void);

	BOOL Parse(char *&s);
	int ISP() const;//In stack priority，同一个运算符ISP一般高于ICP
	int ICP() const;//In coming priority
	OPT_ID GetID() const;
	Mesh_Number GetMN() const;//操作数目数
	Associativity GetAsso() const;//结合性
	void SetAsso(const Associativity as);//设置结合性

protected:
	OPT_ID m_nOptID;//ID，从0开始
	Mesh_Number m_nMeshNum;//操作数目数
	Associativity m_nAsso;
	char m_nPriority;//优先级，这里采用的优先级是小端，即运算符优先级越大，该数越大
private:
	static char opts[][6];//运算符列表
	static char prioritys[];//表示一个范围，第一个char表示优先级为1的运算符ID范围
	static bool optSigns[256];//运算符特征标记，如果char c; optSigns[c]==1真，则c为运算符
};

