#pragma once
class CCharControl
{
public:
	char Table[256];
public:
	CCharControl(void);
	~CCharControl(void);
	void SetFlag(char ch,char flag);
	void SetFlag(char from,char to,char flag);
	void SetFlag(char *s,char flag);
	void RecoverFlag(char flag);//将Flag恢复到上次SetFlag之前的值

	void FlagCharactor(const char flag);//为字母设置Flag
	void FlagNumber(const char flag);//为数字设置Flag
	void FlagVarName(const char flag);//为变量名设置Flag,包括字母，数字，下划线
private:
	char m_CompareTable[256];//用于对比Table的修改，每次修改Table前，将要修改的项存入
};

