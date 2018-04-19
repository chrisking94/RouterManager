#include "StdAfx.h"
#include "HeapFile.h"


CHeapFile::CHeapFile(void)
{
}

CHeapFile::CHeapFile(char *szFilePath)
{
	USES_CONVERSION;
	m_lpszFilePath = A2T(szFilePath);
}

CHeapFile::CHeapFile(LPCTSTR szFile)
{
	m_lpszFilePath = szFile;
}


CHeapFile::~CHeapFile(void)
{
}

void CHeapFile::Open(CFile::OpenFlags nOpenFlag)
{
	CFile::Open(m_lpszFilePath,nOpenFlag);
}

void CHeapFile::Write8(char val)
{
	CFile::Write((byte*)(&val),1);
}

void CHeapFile::Write16(short val)
{
	CFile::Write((byte*)(&val),2);
}

void CHeapFile::Write32(int val)
{
	CFile::Write((byte*)(&val),4);
}

void CHeapFile::Write64(long long val)
{
	CFile::Write((byte*)(&val),8);
}

void CHeapFile::WriteFloat(float val)
{
	CFile::Write((byte*)(&val),4);
}

void CHeapFile::WriteDouble(double val)
{
	CFile::Write((byte*)(&val),8);
}

void CHeapFile::WriteString(char *sz)
{
	int len = strlen(sz);
	Write32(len);//write length
	CFile::Write((byte*)(sz),len);
}

void CHeapFile::WriteString(LPCTSTR sz)
{
	USES_CONVERSION;
	WriteString(T2A(sz));
}

void CHeapFile::Read8(char &val)
{
	CFile::Read((byte*)&val,1);
}

char CHeapFile::Read8()
{
	char val;
	Read8(val);
	return val;
}

void CHeapFile::Read16(short &val)
{
	CFile::Read((byte*)&val,2);
}

short CHeapFile::Read16()
{
	short val;
	Read16(val);
	return val;
}

void CHeapFile::Read32(int &val)
{
	CFile::Read((byte*)&val,4);
}

int CHeapFile::Read32()
{
	int val;
	Read32(val);
	return val;
}

void CHeapFile::Read64(long long &val)
{
	CFile::Read((byte*)&val,8);
}

long long CHeapFile::Read64()
{
	long long val;
	Read64(val);
	return val;
}

void CHeapFile::ReadDouble(double &val)
{
	CFile::Read((byte*)&val,8);
}

double CHeapFile::ReadDouble()
{
	double val;
	ReadDouble(val);
	return val;
}

void CHeapFile::ReadFloat(float &val)
{
	CFile::Read((byte*)&val,4);
}

float CHeapFile::ReadFloat()
{
	float val;
	ReadFloat(val);
	return val;
}

void CHeapFile::ReadString(char *&sz)
{
	int len;
	Read32(len);//read string length
	sz = new char[len+1];
	sz[len] = '\0';
	CFile::Read(sz,len);
}

char * CHeapFile::ReadString()
{
	char *sz;
	ReadString(sz);
	return sz;
}

void CHeapFile::ReadString(LPCTSTR &sz)
{
	USES_CONVERSION;
	char *s;
	ReadString(s);
	sz = A2T(s);
}

void CHeapFile::Close()
{
	CFile::Close();
}

