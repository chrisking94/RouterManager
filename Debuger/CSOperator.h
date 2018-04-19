#pragma once

//�����������
class CCSOperator
{
public:
	typedef enum
	{
		_Sharp,
		_PostPlusPlus,_PostMinusMinus,_LSquareBracket,_RSqureBracket,_LBracket,_RBracket,_Dot,_Pointer
		,_NegativeSign,_ForceTypeChange,_PrePlusPlus,_PreMinusMinus,_Analyze,_GetAddress,_Not,_BitWiseNot,_SizeOf//-Ϊ���ţ�tcΪǿ������ת����ǰ��++��soΪsizeof
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
		,_Comma,_At//��Ŀ,dollerΪchar$a;//���������
	}OPT_ID;

	typedef enum
	{
		_null//��Ŀ
		,_single//��Ŀ
		,_double//˫Ŀ
		,_triple//��Ŀ
	}Mesh_Number;//������Ŀ��

	typedef enum
	{
		_Ignore,//����
		_Right2Left,
		_Left2Right,
	}Associativity;//�����
public:
	CCSOperator(void);
	~CCSOperator(void);

	BOOL Parse(char *&s);
	int ISP() const;//In stack priority��ͬһ�������ISPһ�����ICP
	int ICP() const;//In coming priority
	OPT_ID GetID() const;
	Mesh_Number GetMN() const;//������Ŀ��
	Associativity GetAsso() const;//�����
	void SetAsso(const Associativity as);//���ý����

protected:
	OPT_ID m_nOptID;//ID����0��ʼ
	Mesh_Number m_nMeshNum;//������Ŀ��
	Associativity m_nAsso;
	char m_nPriority;//���ȼ���������õ����ȼ���С�ˣ�����������ȼ�Խ�󣬸���Խ��
private:
	static char opts[][6];//������б�
	static char prioritys[];//��ʾһ����Χ����һ��char��ʾ���ȼ�Ϊ1�������ID��Χ
	static bool optSigns[256];//�����������ǣ����char c; optSigns[c]==1�棬��cΪ�����
};

