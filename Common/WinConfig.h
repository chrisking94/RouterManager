#pragma once
#include "inifile.h"
class __declspec(dllexport)  CWinConfig :
	private CIniFile
{
public:
	CWinConfig();
	CWinConfig(LPCTSTR filePath);
	~CWinConfig(void);

	BOOL WriteToConfig(CButton* pButton);//以控件ID作为KeyName
	BOOL WriteToConfig(CEdit* pEdit);
	BOOL WriteToConfig(CComboBox* pComboBox);
	BOOL WriteToConfig(CIPAddressCtrl* pIPAddressCtrl);

	BOOL ReadFromConfig(CButton* pButton);//以控件ID作为KeyName
	CString ReadFromConfig(CEdit* pEdit);
	CString ReadFromConfig(CComboBox* pComboBox);
	CString ReadFromConfig(CIPAddressCtrl* pIPAddressCtrl);
private:
	CString SetSectionGetKey(CWnd* pWnd);
};

