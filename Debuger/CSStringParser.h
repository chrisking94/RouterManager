#pragma once
//字符串解析器

class CCSStringParser
{
public:
	CCSStringParser();
	~CCSStringParser(void);
	void SetParseObject(char *&ps);//设置解析对象，和构造函数一个意思

	BOOL Parse(char *buff , int *startIndex);//开始解析，结果存入buff，并改变startIndex的值
	BOOL Parse(char *buff , int startIndex);//开始解析，结果存入buff
	BOOL ParseBracket(char *buff , int &startIndex);//开始解析，结果存入buff
	BOOL ParseStatement(char *buff,int * startIndex);

	BOOL Listen(char *buff,int *startIndex);//监听，有string就解析，解析出的字符串带"
protected:
	char **m_ps;
};

