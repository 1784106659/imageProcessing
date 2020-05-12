
// imageProcessingView.cpp: CimageProcessingView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "imageProcessing.h"
#endif

#include "imageProcessingDoc.h"
#include "imageProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CimageProcessingView

IMPLEMENT_DYNCREATE(CimageProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CimageProcessingView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CimageProcessingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CimageProcessingView 构造/析构

CimageProcessingView::CimageProcessingView() noexcept
{
	// TODO: 在此处添加构造代码

}

CimageProcessingView::~CimageProcessingView()
{
}

BOOL CimageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CimageProcessingView 绘图

void CimageProcessingView::OnDraw(CDC* pDC)
{
	CimageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if (!pDoc->m_sImage.IsNull()) {
		pDoc->m_sImage.Draw(pDC->m_hDC, 0, 0);
	}

	if (!pDoc->m_rImage.IsNull()) {
		pDoc->m_rImage.Draw(pDC->m_hDC, pDoc->m_sImage.GetWidth() + 10, 0);
	}

}

void CimageProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 10000 ;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CimageProcessingView 打印


void CimageProcessingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CimageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CimageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CimageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CimageProcessingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CimageProcessingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CimageProcessingView 诊断

#ifdef _DEBUG
void CimageProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CimageProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CimageProcessingDoc* CimageProcessingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CimageProcessingDoc)));
	return (CimageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CimageProcessingView 消息处理程序
