#include "StdAfx.h"
#include "WinConfig.h"


CWinConfig::CWinConfig(LPCTSTR filePath):
CIniFile(filePath)
{
}

CWinConfig::CWinConfig()
{

}


CWinConfig::~CWinConfig(void)
{
}

BOOL CWinConfig::WriteToConfig( CButton* pButton )
{
	return CIniFile::writeBool(SetSectionGetKey(pButton),pButton->GetCheck());
}

BOOL CWinConfig::WriteToConfig( CEdit* pEdit )
{
	CString szTmp;
	pEdit->GetWindowText(szTmp);
	return CIniFile::write(SetSectionGetKey(pEdit),szTmp);
}

BOOL CWinConfig::WriteToConfig( CComboBox* pComboBox )
{
	CString szTmp;
	pComboBox->GetWindowText(szTmp);
	return CIniFile::write(SetSectionGetKey(pComboBox),szTmp);
}

BOOL CWinConfig::WriteToConfig( CIPAddressCtrl* pIPAddressCtrl )
{
	CString szTmp;
	pIPAddressCtrl->GetWindowText(szTmp);
	return CIniFile::write(SetSectionGetKey(pIPAddressCtrl),szTmp);
}

BOOL CWinConfig::ReadFromConfig( CButton* pButton )
{
	BOOL bCheck;

	CIniFile::readBool(SetSectionGetKey(pButton),bCheck);
	pButton->SetCheck(bCheck);

	return bCheck;
}

CString CWinConfig::ReadFromConfig( CEdit* pEdit )
{
	CString szTmp;
	
	CIniFile::read(SetSectionGetKey(pEdit),szTmp);
	pEdit->SetWindowText(szTmp);

	return szTmp;
}

CString CWinConfig::ReadFromConfig( CComboBox* pComboBox )
{
	CString szTmp;

	CIniFile::read(SetSectionGetKey(pComboBox),szTmp);
	pComboBox->SetWindowText(szTmp);

	return szTmp;
}

CString CWinConfig::ReadFromConfig( CIPAddressCtrl* pIPAddressCtrl )
{
	CString szTmp;

	CIniFile::read(SetSectionGetKey(pIPAddressCtrl),szTmp);
	pIPAddressCtrl->SetWindowText(szTmp);

	return szTmp;
}

CString CWinConfig::SetSectionGetKey( CWnd* pWnd )
{
	//CString sz1=_T("");

	//sz1.Format(_T("S%d"),pWnd->GetParent()->GetDlgCtrlID());
	CIniFile::setDefaultSection(_T("Wnd"));

	CString sz2=_T("");

	sz2.Format(_T("K%d"),pWnd->GetDlgCtrlID());

	return sz2;
}


