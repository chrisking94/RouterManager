
// RouterPropertiesEditorView.cpp : CRouterPropertiesEditorView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "RouterPropertiesEditor.h"
#endif

#include "RouterPropertiesEditorDoc.h"
#include "RouterPropertiesEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRouterPropertiesEditorView

IMPLEMENT_DYNCREATE(CRouterPropertiesEditorView, CView)

BEGIN_MESSAGE_MAP(CRouterPropertiesEditorView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRouterPropertiesEditorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CRouterPropertiesEditorView ����/����

CRouterPropertiesEditorView::CRouterPropertiesEditorView()
{
	// TODO: �ڴ˴���ӹ������

}

CRouterPropertiesEditorView::~CRouterPropertiesEditorView()
{
}

BOOL CRouterPropertiesEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CRouterPropertiesEditorView ����

void CRouterPropertiesEditorView::OnDraw(CDC* /*pDC*/)
{
	CRouterPropertiesEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CRouterPropertiesEditorView ��ӡ


void CRouterPropertiesEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CRouterPropertiesEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CRouterPropertiesEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CRouterPropertiesEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CRouterPropertiesEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRouterPropertiesEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRouterPropertiesEditorView ���

#ifdef _DEBUG
void CRouterPropertiesEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CRouterPropertiesEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRouterPropertiesEditorDoc* CRouterPropertiesEditorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRouterPropertiesEditorDoc)));
	return (CRouterPropertiesEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CRouterPropertiesEditorView ��Ϣ�������
