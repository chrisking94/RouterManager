
// RouterPropertiesEditorView.cpp : CRouterPropertiesEditorView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRouterPropertiesEditorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CRouterPropertiesEditorView 构造/析构

CRouterPropertiesEditorView::CRouterPropertiesEditorView()
{
	// TODO: 在此处添加构造代码

}

CRouterPropertiesEditorView::~CRouterPropertiesEditorView()
{
}

BOOL CRouterPropertiesEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CRouterPropertiesEditorView 绘制

void CRouterPropertiesEditorView::OnDraw(CDC* /*pDC*/)
{
	CRouterPropertiesEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CRouterPropertiesEditorView 打印


void CRouterPropertiesEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CRouterPropertiesEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CRouterPropertiesEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CRouterPropertiesEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CRouterPropertiesEditorView 诊断

#ifdef _DEBUG
void CRouterPropertiesEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CRouterPropertiesEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRouterPropertiesEditorDoc* CRouterPropertiesEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRouterPropertiesEditorDoc)));
	return (CRouterPropertiesEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CRouterPropertiesEditorView 消息处理程序
