#pragma once
#include "inifile.h"
class __declspec(dllexport)  CWinConfig :
	private CIniFile
{
public:
	CWinConfig();
	CWinConfig(LPCTSTR filePath);
	~CWinConfig(void);

	BOOL WriteToConfig(CButton* pButton);//�Կؼ�ID��ΪKeyName
	BOOL WriteToConfig(CEdit* pEdit);
	BOOL WriteToConfig(CComboBox* pComboBox);
	BOOL WriteToConfig(CIPAddressCtrl* pIPAddressCtrl);

	BOOL ReadFromConfig(CButton* pButton);//�Կؼ�ID��ΪKeyName
	CString ReadFromConfig(CEdit* pEdit);
	CString ReadFromConfig(CComboBox* pComboBox);
	CString ReadFromConfig(CIPAddressCtrl* pIPAddressCtrl);
private:
	CString SetSectionGetKey(CWnd* pWnd);
};

