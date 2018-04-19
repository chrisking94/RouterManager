#include "StdAfx.h"
#include "Picture.h"


CPicture::CPicture(void):
m_picture(NULL)
{
}


CPicture::~CPicture(void)
{
	m_picture=NULL;
}

BOOL CPicture::SetDrawPictureCtrl( CWnd* pDlg ,CWnd* pPicCtrl )
{
	if(m_picture==NULL) return FALSE;
	//�ڿؼ���Χ��ʾͼƬ
	m_picture->get_Width(&m_width);
	m_picture->get_Height(&m_height);

	pPicCtrl->GetWindowRect(m_Rect);

	m_Rect.bottom=m_Rect.top+m_height/26;
	m_Rect.right=m_Rect.left+m_width/26;

	dc = pDlg->GetDC();

	pDlg->ScreenToClient(&m_Rect); 
	pPicCtrl->MoveWindow(m_Rect);

	return TRUE;
}

void CPicture::DrawOnPictureCtrl()
{
	if(m_picture==NULL) return;
	m_picture->Render(*dc,m_Rect.left,m_Rect.top,m_Rect.Width(),m_Rect.Height(),0,m_height,m_width,-m_height,NULL);
}

bool CPicture::LoadFile( CString sFilePath )
{
	if(!PathFileExists(sFilePath)) return false;

	m_picture=NULL;

	CFile m_file(sFilePath,CFile::modeRead );

	//��ȡ�ļ�����
	DWORD m_filelen = m_file.GetLength(); 

	//�ڶ��Ϸ���ռ�
	HGLOBAL m_hglobal = GlobalAlloc(GMEM_MOVEABLE,m_filelen);

	LPVOID pvdata = NULL;
	//�����ѿռ�,��ȡָ��ѿռ��ָ��
	pvdata = GlobalLock(m_hglobal);

	//���ļ����ݶ���������
	m_file.Read(pvdata,m_filelen);

	IStream*  m_stream;

	GlobalUnlock(m_hglobal);

	//�ڶ��д���������
	CreateStreamOnHGlobal(m_hglobal,TRUE,&m_stream);

	//����������ͼ��
	OleLoadPicture(m_stream,m_filelen,TRUE,IID_IPicture,(LPVOID*)&m_picture);

	GlobalFree(m_stream);

	return true;
}

bool CPicture::LoadResource(HMODULE hModule, DWORD dwSourceID )
{
	// ������Դ
	HRSRC hRsrc = ::FindResource(hModule, MAKEINTRESOURCE(dwSourceID), _T("PNG"));
	if (hRsrc == NULL)  return false;
	// ������Դ
	HGLOBAL hImgData = ::LoadResource(hModule, hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return false;
	}
	// �����ڴ��е�ָ����Դ
	LPVOID lpVoid = ::LockResource(hImgData);
	LPSTREAM pStream = NULL;
	DWORD dwSize = ::SizeofResource(hModule, hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);
	// ����ڴ��е�ָ����Դ
	::GlobalUnlock(hNew);
	// ��ָ���ڴ洴��������
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
	if ( ht == S_OK )
	{
		// ����ͼƬ
		OleLoadPicture(pStream,dwSize,TRUE,IID_IPicture,(LPVOID*)&m_picture);//��֧��PNG
	}
	GlobalFree(hNew);
	// �ͷ���Դ
	::FreeResource(hImgData);
	
	return true;
}
