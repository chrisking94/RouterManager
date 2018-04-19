#pragma once
class __declspec(dllexport) CIniFile
{
public:
	CIniFile(void);
	CIniFile(LPCTSTR filePath);
	~CIniFile(void);
public:
	BOOL write(LPCTSTR lpszSection,LPCTSTR lpszKey,LPCTSTR lpszValue);
	BOOL write(LPCTSTR lpszKey,LPCTSTR lpszValue);
	BOOL writeBool(LPCTSTR lpszKey,BOOL bValue);
	CString read(LPCTSTR lpszSection,LPCTSTR lpszKey,LPCTSTR lpszDefault=_T(""));
	CString read(LPCTSTR lpszKey);
	DWORD read(LPCTSTR lpszKey,CString &szReturned);
	DWORD read(LPCTSTR lpszSection,LPCTSTR lpszKey,CString& csReturned,LPCTSTR lpszDefault=_T(""));
	void readBool(LPCTSTR lpszKey,BOOL &bReturned );
	void setDefaultSection(LPCTSTR lpszDefaultSection);
private:
	CString m_szFilePath;//如果使用LPCTSTR会出错，原因暂时不明
	LPCTSTR m_lpszDefaultSection;
};

