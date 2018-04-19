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
	//在控件范围显示图片
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

	//获取文件长度
	DWORD m_filelen = m_file.GetLength(); 

	//在堆上分配空间
	HGLOBAL m_hglobal = GlobalAlloc(GMEM_MOVEABLE,m_filelen);

	LPVOID pvdata = NULL;
	//锁定堆空间,获取指向堆空间的指针
	pvdata = GlobalLock(m_hglobal);

	//将文件数据读区到堆中
	m_file.Read(pvdata,m_filelen);

	IStream*  m_stream;

	GlobalUnlock(m_hglobal);

	//在堆中创建流对象
	CreateStreamOnHGlobal(m_hglobal,TRUE,&m_stream);

	//利用流加载图像
	OleLoadPicture(m_stream,m_filelen,TRUE,IID_IPicture,(LPVOID*)&m_picture);

	GlobalFree(m_stream);

	return true;
}

bool CPicture::LoadResource(HMODULE hModule, DWORD dwSourceID )
{
	// 查找资源
	HRSRC hRsrc = ::FindResource(hModule, MAKEINTRESOURCE(dwSourceID), _T("PNG"));
	if (hRsrc == NULL)  return false;
	// 加载资源
	HGLOBAL hImgData = ::LoadResource(hModule, hRsrc);
	if (hImgData == NULL)
	{
		::FreeResource(hImgData);
		return false;
	}
	// 锁定内存中的指定资源
	LPVOID lpVoid = ::LockResource(hImgData);
	LPSTREAM pStream = NULL;
	DWORD dwSize = ::SizeofResource(hModule, hRsrc);
	HGLOBAL hNew = ::GlobalAlloc(GMEM_MOVEABLE, dwSize);
	LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
	::memcpy(lpByte, lpVoid, dwSize);
	// 解除内存中的指定资源
	::GlobalUnlock(hNew);
	// 从指定内存创建流对象
	HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
	if ( ht == S_OK )
	{
		// 加载图片
		OleLoadPicture(pStream,dwSize,TRUE,IID_IPicture,(LPVOID*)&m_picture);//不支持PNG
	}
	GlobalFree(hNew);
	// 释放资源
	::FreeResource(hImgData);
	
	return true;
}
