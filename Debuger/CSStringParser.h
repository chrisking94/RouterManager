#pragma once
//�ַ���������

class CCSStringParser
{
public:
	CCSStringParser();
	~CCSStringParser(void);
	void SetParseObject(char *&ps);//���ý������󣬺͹��캯��һ����˼

	BOOL Parse(char *buff , int *startIndex);//��ʼ�������������buff�����ı�startIndex��ֵ
	BOOL Parse(char *buff , int startIndex);//��ʼ�������������buff
	BOOL ParseBracket(char *buff , int &startIndex);//��ʼ�������������buff
	BOOL ParseStatement(char *buff,int * startIndex);

	BOOL Listen(char *buff,int *startIndex);//��������string�ͽ��������������ַ�����"
protected:
	char **m_ps;
};

