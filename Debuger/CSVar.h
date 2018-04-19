#pragma once
#include "CSVarType.h"

class CCSVarType;

class CCSVar:public CCSVarType
{
public:
	typedef char* ptrSZ;//�ַ���ָ��
	typedef unsigned int VAR_ID;
public:
	CCSVar();
	~CCSVar(void);
	CCSVar(const CCSVar &v);//��������
	const CCSVar &operator=(const CCSVar &v);//���������=���� ��ʹ��=ʱ�����Ĭ�Ͽ������캯������������
	
	//�����ʹ���
	CCSVar(const CCSVarType::TYPE t);
	//������������
	CCSVar(const char v);
	CCSVar(const double v);
	CCSVar(const float v);
	CCSVar(const int v);
	CCSVar(const long v);
	CCSVar(const CCSVar *v);//ָ���ͱ���
	CCSVar(const short v);
	CCSVar(const char * v);//�������
	

	//�����
	CCSVar operator+(const CCSVar &v) const;
	CCSVar operator-(const CCSVar &v) const;
	CCSVar operator*(const CCSVar &v) const;
	CCSVar operator/(const CCSVar &v) const;
	CCSVar operator%(const CCSVar &v) const;
	CCSVar operator^(const CCSVar &v) const;
	//CCSVar operator&(const CCSVar &v) const;
	//CCSVar operator|(const CCSVar &v) const;

	//�Ա������
	CCSVar operator+=(const CCSVar &v);
	CCSVar operator-=(const CCSVar &v);
	CCSVar operator*=(const CCSVar &v);
	CCSVar operator/=(const CCSVar &v);
	CCSVar operator%=(const CCSVar &v);
	CCSVar operator^=(const CCSVar &v);
	//CCSVar operator&=(const CCSVar &v);
	//CCSVar operator|=(const CCSVar &v);
	CCSVar operator++(const int);
	CCSVar operator++();//�������ǰ��++
	CCSVar operator--(const int);
	CCSVar operator--();

	//�Ƚ������
	bool operator>(const CCSVar &v) const;
	bool operator>=(const CCSVar &v) const;
	bool operator<(const CCSVar &v) const;
	bool operator<=(const CCSVar &v) const;
	bool operator==(const CCSVar &v) const;
	bool operator!=(const CCSVar &v) const;

	CCSVar &operator[](const int v);//�ַ�����ȡchar�����Ʒ���a[i]
	CCSVar &operator*();//����
	CCSVar operator&();//ȡ��ַ������ptr����var

	//�ⲿ�ӿں���
	char *StrVal() const;
	long NumVal() const;

	CCSVar &ToNumber();//֧��16����
	CCSVar &ToString();//ת��ΪString

	bool IsMe(char *szName) const;//���Լ���m_szName�Ƚ�
	void SetName(char *name);//��������
	char *GetName() const;//��ȡ����
	long GetPtr() const;//����m_ptr

	char &At(const int v);//�ַ����Ĳ�������Ϊ[]���ظ�ָ���ˣ������ַ����������ַ�ʽ

	CCSVar *GetPtrVar();//ָ����(_ptr)�����ã����ڷ��ʵ�ָ������ָ��ı���
	BOOL IsBadPtr() const; //_ptr����_ptr�򷵻ر�����this
	void SetPtrLevel(int nLevel);//ָ�����
	CCSVar *GetMyAddr();//���ر������ĵ�ַ
private:
	void Realse();//�ͷŴ洢����������ڴ�ռ�
protected:
	long m_ptr;//�������ڵ��ڴ�λ��ָ��
private:
	VAR_ID m_nID;//ȫ��ΨһID
	static VAR_ID s_nGlobalID;
	int m_strlen;//ֻ���ַ������ͱ���
	char* m_szName;

	//�ڲ�����ʹ��
	char *buff;
};

