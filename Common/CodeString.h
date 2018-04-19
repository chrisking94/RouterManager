#pragma once
#include "stdafx.h"
class __declspec(dllexport) CCodeString
{
public:
	CCodeString(void);
	CCodeString(LPCTSTR lpszSourceString);
	~CCodeString(void);

public:
	CString B64Encode(void);
	CString B64Decode(void);

	CString KEncrypt(void);
	CString KDecrypt(void);

	CString Md5Encrypt(void);
	//CString Md5Decrypt(void);

	static CString ToHexString(ULONG num);
	static CString ConvertUnits(ULONG nNumber);//将字节(b)转换为b,k,m,g,t
private:
	CString m_szSourceString;
	const UINT m_nSourceLength;
};

