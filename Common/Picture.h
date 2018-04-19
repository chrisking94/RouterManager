#pragma once
#include "atlimage.h"

class __declspec(dllexport) CPicture
{
private:
	OLE_XSIZE_HIMETRIC m_width;
	OLE_YSIZE_HIMETRIC m_height;
	CDC* dc;
	CRect m_Rect;
	IPicture *m_picture;
public:
	CPicture(void);
	~CPicture(void);

	bool LoadFile(CString sFilePath);
	bool LoadResource(HMODULE hModule,DWORD dwSourceID);
	BOOL SetDrawPictureCtrl(CWnd* pDlg ,CWnd* pPicCtrl);
	void DrawOnPictureCtrl();
};

