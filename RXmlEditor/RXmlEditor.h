
// RXmlEditor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRXmlEditorApp:
// �йش����ʵ�֣������ RXmlEditor.cpp
//

class CRXmlEditorApp : public CWinApp
{
public:
	CRXmlEditorApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRXmlEditorApp theApp;