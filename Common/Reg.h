#pragma once
class __declspec(dllexport) CReg:private CRegKey
{
public:
	CReg(void);
	CReg(HKEY hKey,LPCTSTR lpszSubKey);
	CReg(PHKEY pHKey);
	~CReg(void);

	BOOL Read(LPCTSTR lpszValueName,
		DWORD dwKeyType,       // type buffer，键类型。常用REG_SZ,REG_DWORD
		LPBYTE lpData,// data buffer。保存查询结果的缓冲区
		DWORD dwDataBufferLen);// size of data buffer。缓冲区大小
	BOOL Write(LPCTSTR lpszValueName,
		DWORD dwKeyType,// type buffer，键类型。常用REG_SZ,REG_DWORD
		CONST BYTE *lpData,// value data 要写入的数据
		DWORD dwDataBufLen);// size of value data 。数据SIZE

	BOOL Open(REGSAM samDesired = KEY_ALL_ACCESS);
	CString Read(LPCTSTR lpszValueName);// value name要读取的键名称
	BOOL Read(LPCTSTR lpszValueName,CString& szReturned);
	BOOL Write(LPCTSTR lpszValueName,CONST CString szValue);
	BOOL Write(LPCTSTR lpszValueName,CONST DWORD dwValue);
	BOOL Delete(LPCTSTR lpszValueName);
	BOOL CreateKey(LPCTSTR lpSubKey);//创建一个新键，并返回键指针
	BOOL DeleteKey(LPCTSTR lpSubKey);//删除一个子键
private:
	HKEY m_HKEY;
	HKEY m_RootKey;
	LPCTSTR m_lpszSubKey;
	REGSAM m_samDesired64_32;
};

