#pragma once
#include "afx.h"

class __declspec(dllexport) CHeapFile:protected CFile
{
public:
	typedef OpenFlags OpenFlags;
public:
	CHeapFile(void);
	~CHeapFile(void);
	CHeapFile(char *szFilePath);
	CHeapFile(LPCTSTR szFile);

	//File operation
	void Open(OpenFlags nOpenFlag);
	void Close();

	// Write
	void Write8(char val);
	void Write16(short val);
	void Write32(int val);
	void Write64(long long val);//64λϵͳ
	void WriteDouble(double val);
	void WriteFloat(float val);
	void WriteString(char *sz);
	void WriteString(LPCTSTR sz);
	
	// Read
	void Read8(char &val);
	char Read8();
	void Read16(short &val);
	short Read16();
	void Read32(int &val);
	int Read32();
	void Read64(long long &val);//64λϵͳ
	long long Read64();
	void ReadDouble(double &val);
	double ReadDouble();
	void ReadFloat(float &val);
	float ReadFloat();
	void ReadString(char *&sz);//this function would allocate memory
	char * ReadString();

	void ReadString(LPCTSTR &sz);

private:
	LPCTSTR m_lpszFilePath;
};

