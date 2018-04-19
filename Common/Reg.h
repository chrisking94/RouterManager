#pragma once
class __declspec(dllexport) CReg:private CRegKey
{
public:
	CReg(void);
	CReg(HKEY hKey,LPCTSTR lpszSubKey);
	CReg(PHKEY pHKey);
	~CReg(void);

	BOOL Read(LPCTSTR lpszValueName,
		DWORD dwKeyType,       // type buffer�������͡�����REG_SZ,REG_DWORD
		LPBYTE lpData,// data buffer�������ѯ����Ļ�����
		DWORD dwDataBufferLen);// size of data buffer����������С
	BOOL Write(LPCTSTR lpszValueName,
		DWORD dwKeyType,// type buffer�������͡�����REG_SZ,REG_DWORD
		CONST BYTE *lpData,// value data Ҫд�������
		DWORD dwDataBufLen);// size of value data ������SIZE

	BOOL Open(REGSAM samDesired = KEY_ALL_ACCESS);
	CString Read(LPCTSTR lpszValueName);// value nameҪ��ȡ�ļ�����
	BOOL Read(LPCTSTR lpszValueName,CString& szReturned);
	BOOL Write(LPCTSTR lpszValueName,CONST CString szValue);
	BOOL Write(LPCTSTR lpszValueName,CONST DWORD dwValue);
	BOOL Delete(LPCTSTR lpszValueName);
	BOOL CreateKey(LPCTSTR lpSubKey);//����һ���¼��������ؼ�ָ��
	BOOL DeleteKey(LPCTSTR lpSubKey);//ɾ��һ���Ӽ�
private:
	HKEY m_HKEY;
	HKEY m_RootKey;
	LPCTSTR m_lpszSubKey;
	REGSAM m_samDesired64_32;
};

