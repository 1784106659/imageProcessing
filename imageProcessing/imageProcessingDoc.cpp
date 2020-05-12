
// imageProcessingDoc.cpp: CimageProcessingDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "imageProcessing.h"
#endif

#include "imageProcessingDoc.h"
#include "DialogScalingFactor.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CimageProcessingDoc

IMPLEMENT_DYNCREATE(CimageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CimageProcessingDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CimageProcessingDoc::OnFileOpen)
	ON_COMMAND(ID_NEAREST_INTERPOLATION_SCALING, &CimageProcessingDoc::OnNearestInterpolationScaling)
	ON_COMMAND(ID_RED, &CimageProcessingDoc::OnRed)
	ON_COMMAND(ID_GREEN, &CimageProcessingDoc::OnGreen)
	ON_COMMAND(ID_BLUE, &CimageProcessingDoc::OnBlue)
	ON_COMMAND(ID_COLOR_TO_GREY, &CimageProcessingDoc::OnColorToGrey)
	ON_COMMAND(ID_IMAGE_ADD, &CimageProcessingDoc::OnImageAdd)
	ON_COMMAND(ID_32788, &CimageProcessingDoc::OnHsvHue)
	ON_COMMAND(ID_32789, &CimageProcessingDoc::OnHsvSaturation)
	ON_COMMAND(ID_32790, &CimageProcessingDoc::OnHsvValue)
	ON_COMMAND(ID_32791, &CimageProcessingDoc::OnImageSub)
	ON_COMMAND(ID_32794, &CimageProcessingDoc::OnImageMul)
	ON_COMMAND(ID_32792, &CimageProcessingDoc::OnImageAnd)
	ON_COMMAND(ID_32793, &CimageProcessingDoc::OnImageOr)
END_MESSAGE_MAP()


// CimageProcessingDoc 构造/析构

CimageProcessingDoc::CimageProcessingDoc() noexcept
{
	// TODO: 在此添加一次性构造代码

}

CimageProcessingDoc::~CimageProcessingDoc()
{
}

BOOL CimageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CimageProcessingDoc 序列化

void CimageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CimageProcessingDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CimageProcessingDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CimageProcessingDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CimageProcessingDoc 诊断

#ifdef _DEBUG
void CimageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CimageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CimageProcessingDoc 命令

//打开图像文件
void CimageProcessingDoc::OnFileOpen() //文件打开
{
	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	HRESULT hResult;

	//获取CImage支持的图像文件的过滤字符串
	hResult = m_sImage.GetExporterFilterString(strFilter, aguidFileTypes, _T("All Image Files"));
	if (FAILED(hResult)) {
		AfxMessageBox(_T("GetExporterFilter函数失败！"));
		return;
	}
	CFileDialog dig(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	if (IDOK != dig.DoModal())
		return;

	m_sImage.Destroy();  //把之前的文件删除
	m_rImage.Destroy();

	  //将外部图像文件装载到CImage对象中
	hResult = m_sImage.Load(dig.GetPathName());
	if (FAILED(hResult)) {
		AfxMessageBox(_T("图像文件加载失败！"));
		return;
	}

	if (m_sImage.GetBPP() != 24) {
		m_sImage.Destroy();
		AfxMessageBox(_T("只处理24位色图像！"));
	}

	//设置主窗口标题栏的内容
	CString str;
	str.LoadString(AFX_IDS_APP_TITLE);
	AfxGetMainWnd()->SetWindowText(str +  " - " + dig.GetFileName());
	InvalidateRect(NULL, NULL, FALSE);  //刷新窗口
}  


void CimageProcessingDoc::OnNearestInterpolationScaling()//缩放
{
	// TODO: 在此添加命令处理程序代码
	if (m_sImage.IsNull()) {
		AfxMessageBox(_T("请打开要处理的图像！"));
		return;
	}

	int x;
	int y;
	CDialogScalingFactor mdlg;
	if (mdlg.DoModal() != IDOK) {  //输入水平和垂直方向放大系数
		return;
	}
	if (mdlg.mHoriScalingFactor < 0 || mdlg.mVeriScalingFactor < 0) {
		AfxMessageBox(_T("缩放参数错误!"));
		return;
	}

	int sWidth = m_sImage.GetWidth();  //原始图像宽度
	int sHeight = m_sImage.GetHeight();  //原始图像高度

	int rWidth = (int)(sWidth * mdlg.mHoriScalingFactor + 0.5);  //目标图像宽度
	int rHeight = (int)(sHeight * mdlg.mVeriScalingFactor + 0.5);  //目标图像高度
	m_rImage.Destroy();
	if (!m_rImage.Create(rWidth, rHeight, m_sImage.GetBPP(), 0)) {
		return;
	}

	int sRowBytes = m_sImage.GetPitch();
	int rRowBytes = m_rImage.GetPitch();

	BYTE*sImageData = (BYTE*)m_sImage.GetBits();
	BYTE*rImageData = (BYTE*)m_rImage.GetBits();

	for (y = 1; y < rHeight - 1; y++) {
		for (x = 1; x < rWidth - 1; x++) {
			int i = (int)(x / mdlg.mHoriScalingFactor);
			int j = (int)(y / mdlg.mVeriScalingFactor);
			rImageData[y*rRowBytes + x * 3] = sImageData[j * sRowBytes + i * 3];
			rImageData[y*rRowBytes + x * 3 + 1] = sImageData[j * sRowBytes + i * 3 + 1];
			rImageData[y*rRowBytes + x * 3 + 2] = sImageData[j * sRowBytes + i * 3 + 2];
		}
		
	}
       UpdateAllViews(NULL); //强调调用onDraw
		return;
}


void CimageProcessingDoc::OnRed()//红色分量
{
	// TODO: 在此添加命令处理程序代码

	if (m_sImage.IsNull()) {
		AfxMessageBox(_T("请打开要处理的图像!"));
		return;
	}

	int x;
	int y;
	int nRows = m_sImage.GetHeight();
	int nCols = m_sImage.GetWidth();
	int nBPP = m_sImage.GetBPP();  //每一个像素占空间大小 （24）

	BYTE nGrey;
	m_rImage.Destroy();

	if (!m_rImage.Create(nCols, nRows, nBPP)) {
		return;
	}
	BYTE *sImageData = (BYTE*)m_sImage.GetBits();  // 获得指向位图的指针
	BYTE *rImageData = (BYTE*)m_rImage.GetBits();
	int nRowBytes = m_sImage.GetPitch();  //位图一行的间距


	for(y = 0; y<nRows; y++)
		for (x = 0; x < nCols; x++) {
			nGrey = *(sImageData + y * nRowBytes + x * 3 + 2);//红色
			rImageData[y * nRowBytes + x * 3] = 0; // nGrey;
			rImageData[y * nRowBytes + x * 3 + 1] = 0;// nGrey;
			rImageData[y * nRowBytes + x * 3 + 2] = nGrey;   
		}
	UpdateAllViews(NULL);
}


void CimageProcessingDoc::OnGreen()//绿色分量
{
	// TODO: 在此添加命令处理程序代码
	if (m_sImage.IsNull()) {
		AfxMessageBox(_T("请打开要处理的图像!"));
		return;
	}

	int x;
	int y;
	int nRows = m_sImage.GetHeight();
	int nCols = m_sImage.GetWidth();
	int nBPP = m_sImage.GetBPP();

	BYTE nGrey;
	m_rImage.Destroy();

	if (!m_rImage.Create(nCols, nRows, nBPP)) {
		return;
	}
	BYTE *sImageData = (BYTE*)m_sImage.GetBits();
	BYTE *rImageData = (BYTE*)m_rImage.GetBits();
	int nRowBytes = m_sImage.GetPitch();


	for (y = 0; y < nRows; y++)
		for (x = 0; x < nCols; x++) {
			nGrey = *(sImageData + y * nRowBytes + x * 3 + 1);//绿色
			rImageData[y * nRowBytes + x * 3] = 0; // nGrey;
			rImageData[y * nRowBytes + x * 3 + 1] = nGrey;
			rImageData[y * nRowBytes + x * 3 + 2] = 0; // nGrey;
		}
	UpdateAllViews(NULL);
}


void CimageProcessingDoc::OnBlue()//蓝色分量
{
	// TODO: 在此添加命令处理程序代码
	if (m_sImage.IsNull()) {
		AfxMessageBox(_T("请打开要处理的图像!"));
		return;
	}

	int x;
	int y;
	int nRows = m_sImage.GetHeight();
	int nCols = m_sImage.GetWidth();
	int nBPP = m_sImage.GetBPP();

	BYTE nGrey;
	m_rImage.Destroy();

	if (!m_rImage.Create(nCols, nRows, nBPP)) {
		return;
	}
	BYTE *sImageData = (BYTE*)m_sImage.GetBits();
	BYTE *rImageData = (BYTE*)m_rImage.GetBits();
	int nRowBytes = m_sImage.GetPitch();


	for (y = 0; y < nRows; y++)
		for (x = 0; x < nCols; x++) {
			nGrey = *(sImageData + y * nRowBytes + x * 3 + 0);//蓝色分量
			rImageData[y * nRowBytes + x * 3] = nGrey;
			rImageData[y * nRowBytes + x * 3 + 1] = 0;// nGrey;
			rImageData[y * nRowBytes + x * 3 + 2] = 0;// nGrey;
		}
	UpdateAllViews(NULL);
}


void CimageProcessingDoc::OnColorToGrey()//灰度图像
{
	// TODO: 在此添加命令处理程序代码
	if (m_sImage.IsNull()) {
		AfxMessageBox(_T("请打开要处理的图像!"));
		return;
	}
	//直接访问图像数据
	int x;
	int y;
	int nRows = m_sImage.GetHeight();
	int nCols = m_sImage.GetWidth();
	int nBPP = m_sImage.GetBPP();

	BYTE nRed,nGreen,nBlue, nGrey;
	m_rImage.Destroy();

	if (!m_rImage.Create(nCols, nRows, nBPP)) {
		return;
	}
	BYTE *sImageData = (BYTE*)m_sImage.GetBits();
	BYTE *rImageData = (BYTE*)m_rImage.GetBits();
	int nRowBytes = m_sImage.GetPitch();


	for (y = 0; y < nRows; y++)
		for (x = 0; x < nCols; x++) {
			nBlue = sImageData[y *nRowBytes + x * 3 + 0];
			nGreen = sImageData[y *nRowBytes + x * 3 + 1];
			nRed = sImageData[y *nRowBytes + x * 3 + 2];
			nGrey = (BYTE)(0.299*nRed + 0.587*nGreen + 0.114*nBlue);  //也可用(nRed+nGreen+nBlue)/3
			rImageData[y * nRowBytes + x * 3] = nGrey;
			rImageData[y * nRowBytes + x * 3 + 1] = nGrey;
			rImageData[y * nRowBytes + x * 3 + 2] = nGrey;
		}
	UpdateAllViews(NULL);
}//


void CimageProcessingDoc::OnImageAdd()//图像相加
{
	// TODO: 在此添加命令处理程序代码

	if (m_sImage.IsNull()) {
		AfxMessageBox(_T("请打开要处理的图像！"));
		return;
	}

	CImage isImage;    //要相加的图
	CString strFilter;
	CSimpleArray<GUID> aguidFileTypes;
	HRESULT hResult;
	//获取CImage支持的图像文件的过滤字符串
	hResult = m_sImage.GetExporterFilterString(strFilter, aguidFileTypes, _T("ALL Image Files"));
	if (FAILED(hResult)) {
		AfxMessageBox(_T("GetExporterFilter函数失败！"));
		return;
	}

	CFileDialog dig(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	if (IDOK != dig.DoModal()) {
		return;
	}
	isImage.Destroy();
	//将外部图像文件装载到CImage对象中
	hResult = isImage.Load(dig.GetPathName());
	if (FAILED(hResult)) {
		AfxMessageBox(_T("图像文件加载失败！"));
			return;
	}

	if (isImage.GetBPP() != 24) {
		isImage.Destroy();
		AfxMessageBox(_T("只处理24位色图像！"));
		return;
	}
	if ((isImage.GetHeight() != m_sImage.GetHeight()) || (isImage.GetWidth() != m_sImage.GetWidth()) ){
		isImage.Destroy();
		AfxMessageBox(_T("本软件只处理相同大小图像的逻辑和代数运算！"));
		return;
	}
	CDialogImageAddRatio mdlg;
	if (mdlg.DoModal() != IDOK) {
		isImage.Destroy();
		return;
	}
	double lambuda = mdlg.addRadio / 100.0;
	
	//double lambuda = 0.5;
	int x;
	int y;
	int nRows = m_sImage.GetHeight();
	int nCols = m_sImage.GetWidth();
	int nBPP = m_sImage.GetBPP();

	m_rImage.Destroy();
	if (!m_rImage.Create(nCols, nRows, nBPP)) {
		isImage.Destroy();
		return;
	}
	BYTE *sImageData = (BYTE*)m_sImage.GetBits();
	BYTE *isImageData = (BYTE*)isImage.GetBits();
	BYTE *rImageData = (BYTE*)m_rImage.GetBits();

	int nRowBytes = m_sImage.GetPitch();
	int sRowBytes = m_sImage.GetPitch();

	for(y = 0; y <nRows; y++)
		for (x = 0; x < nCols; x++) {
			rImageData[y*nRowBytes + x * 3 + 0] = (int)(lambuda*sImageData[y*nRowBytes + x * 3 + 0] + (1 - lambuda)*isImageData[y*nRowBytes + x * 3 + 0]);
			rImageData[y*nRowBytes + x * 3 + 1] = (int)(lambuda*sImageData[y*nRowBytes + x * 3 + 1] + (1 - lambuda)*isImageData[y*nRowBytes + x * 3 + 1]);
			rImageData[y*nRowBytes + x * 3 + 2] = (int)(lambuda*sImageData[y*nRowBytes + x * 3 + 2] + (1 - lambuda)*isImageData[y*nRowBytes + x * 3 + 2]);
		}
	isImage.Destroy();
	UpdateAllViews(NULL);
	
}


void CimageProcessingDoc::OnHsvHue()//色调：图像的整体颜色的概括
{
	// TODO: 在此添加命令处理程序代码
	if (m_sImage.IsNull())
	{
		AfxMessageBox(_T("请打开要处理的图像！"));
		return;
	}

	int x;
	int y;
	int nRows = m_sImage.GetHeight();
	int nCols = m_sImage.GetWidth();
	int nBPP = m_sImage.GetBPP();

	BYTE nRed, nGreen, nBlue, nGrey;
	double  dHue;//色调分量
	m_rImage.Destroy();

	if (!m_rImage.Create(nCols, nRows, nBPP))
		return;
	BYTE *sImageData = (BYTE *)m_sImage.GetBits();
	BYTE *rImageData = (BYTE *)m_rImage.GetBits();
	int nRowBytes = m_sImage.GetPitch();

	for (y = 0; y < nRows; y++)
		for (x = 0; x < nCols; x++)
		{
			nBlue = sImageData[y * nRowBytes + x * 3 + 0];
			nGreen = sImageData[y * nRowBytes + x * 3 + 1];
			nRed = sImageData[y * nRowBytes + x * 3 + 2];
			if (nRed == nGreen && nBlue == nRed)
				dHue = 0.0;
			else
			{
				dHue = acos(0.5*((nRed - nGreen) + (nRed - nBlue)) / sqrt((nRed - nGreen)*(nRed - nGreen) + (nRed - nBlue)*(nGreen - nBlue)));//公式
				if (nBlue > nGreen)
					dHue = 2 * PI - dHue;
			}
			nGrey = (BYTE)(255.0 * dHue / (2 * PI));
			rImageData[y * nRowBytes + x * 3] = (BYTE)nGrey;
			rImageData[y * nRowBytes + x * 3 + 1] = (BYTE)nGrey;
			rImageData[y * nRowBytes + x * 3 + 2] = (BYTE)nGrey;
		}

	UpdateAllViews(NULL);
}


void CimageProcessingDoc::OnHsvSaturation()//饱和度：彩度除以明度，色彩的鲜艳程度
{
	// TODO: 在此添加命令处理程序代码
	if (m_sImage.IsNull())
	{
		AfxMessageBox(_T("请打开要处理的图像！"));
		return;
	}

	int x;
	int y;
	int nRows = m_sImage.GetHeight();
	int nCols = m_sImage.GetWidth();
	int nBPP = m_sImage.GetBPP();

	BYTE nRed, nGreen, nBlue, nGrey;
	double  dSat;//饱和度分量
	m_rImage.Destroy();

	if (!m_rImage.Create(nCols, nRows, nBPP))
		return;
	BYTE *sImageData = (BYTE *)m_sImage.GetBits();
	BYTE *rImageData = (BYTE *)m_rImage.GetBits();
	int nRowBytes = m_sImage.GetPitch();
	BYTE MIN;
	for (y = 0; y < nRows; y++)
		for (x = 0; x < nCols; x++)
		{
			nBlue = sImageData[y * nRowBytes + x * 3 + 0];
			nGreen = sImageData[y * nRowBytes + x * 3 + 1];
			nRed = sImageData[y * nRowBytes + x * 3 + 2];

			if (nBlue < nGreen) {
				MIN = nBlue;
			}
			else {
				MIN = nGreen;
			}
			if (nRed < MIN) {
				MIN = nRed;
			}

			if ((nRed + nGreen + nBlue) == 0)
				dSat = 0.0;
			else
			{			
			dSat = 1.0-((double)(3*MIN) / (double)(nRed+ nGreen+nBlue));//除法运算要谨慎！ 
			}
			nGrey = (BYTE)(255.0*dSat);
			rImageData[y * nRowBytes + x * 3] = (BYTE)nGrey;
			rImageData[y * nRowBytes + x * 3 + 1] = (BYTE)nGrey;
			rImageData[y * nRowBytes + x * 3 + 2] = (BYTE)nGrey;
		}

	UpdateAllViews(NULL);
}


void CimageProcessingDoc::OnHsvValue()//亮度
{
	// TODO: 在此添加命令处理程序代码
	if (m_sImage.IsNull())
	{
		AfxMessageBox(_T("请打开要处理的图像！"));
		return;
	}

	int x;
	int y;
	int nRows = m_sImage.GetHeight();
	int nCols = m_sImage.GetWidth();
	int nBPP = m_sImage.GetBPP();

	BYTE nRed, nGreen, nBlue, nGrey;
	double  dVal;//饱和度分量
	m_rImage.Destroy();

	if (!m_rImage.Create(nCols, nRows, nBPP))
		return;
	BYTE *sImageData = (BYTE *)m_sImage.GetBits();
	BYTE *rImageData = (BYTE *)m_rImage.GetBits();
	int nRowBytes = m_sImage.GetPitch();
	BYTE MIN;
	for (y = 0; y < nRows; y++)
		for (x = 0; x < nCols; x++)
		{
			nBlue = sImageData[y * nRowBytes + x * 3 + 0];
			nGreen = sImageData[y * nRowBytes + x * 3 + 1];
			nRed = sImageData[y * nRowBytes + x * 3 + 2];

			if (nBlue < nGreen) {
				MIN = nBlue;
			}
			else {
				MIN = nGreen;
			}
			if (nRed < MIN) {
				MIN = nRed;
			}
			dVal = (double)(nRed + nGreen + nBlue) / 3.0;
			
			nGrey = (BYTE)(255.0*dVal);
			rImageData[y * nRowBytes + x * 3] = (BYTE)nGrey;
			rImageData[y * nRowBytes + x * 3 + 1] = (BYTE)nGrey;
			rImageData[y * nRowBytes + x * 3 + 2] = (BYTE)nGrey;
		}

	UpdateAllViews(NULL);
}


void CimageProcessingDoc::OnImageSub() 
{
	// TODO: 在此添加命令处理程序代码
	if (m_sImage.IsNull())
	{
		AfxMessageBox(_T("请打开要处理的图像！"));
		return;
	}

	CImage lsImage;
	CString strFilter;
	CSimpleArray<GUID>  aguidFileTypes;
	HRESULT hResult;

	// 获取CImage支持的图像文件的过滤字符串
	hResult = m_sImage.GetExporterFilterString(strFilter, aguidFileTypes, _T("All Image Files"));
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("GetExporterFilter函数失败!"));
		return;
	}

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	if (IDOK != dlg.DoModal())
		return;

	lsImage.Destroy();

	// 将外部图像文件装载到CImage对象中
	hResult = lsImage.Load(dlg.GetPathName());
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("图像文件加载失败！"));
		return;
	}

	if (lsImage.GetBPP() != 24)
	{
		lsImage.Destroy();
		AfxMessageBox(_T("只处理24位色图像！"));
		return;
	}

	if ((lsImage.GetHeight() != m_sImage.GetHeight()) || (lsImage.GetWidth() != m_sImage.GetWidth()))
	{
		lsImage.Destroy();
		AfxMessageBox(_T("本软件只处理相同大小图像的逻辑与代数运算！"));
		return;
	}

	CDialogImageAddRatio mdlg;  //图像减法占比对话框
	if (mdlg.DoModal() != IDOK)
	{
		lsImage.Destroy();
		return;
	}

	double lambuda = mdlg.addRadio / 100.0;

	int x;
	int y;
	int nRows = m_sImage.GetHeight();
	int nCols = m_sImage.GetWidth();
	int nBPP = m_sImage.GetBPP();

	m_rImage.Destroy();
	if (!m_rImage.Create(nCols, nRows, nBPP))
	{
		lsImage.Destroy();
		return;
	}

	BYTE *sImageData = (BYTE *)m_sImage.GetBits();
	BYTE *lsImageData = (BYTE *)lsImage.GetBits();
	BYTE *rImageData = (BYTE *)m_rImage.GetBits();
	int nRowBytes = m_sImage.GetPitch();

	int sRowBytes = m_sImage.GetPitch();
	int nrgb;
	for (y = 0; y < nRows; y++)
		for (x = 0; x < nCols; x++)
		{
			nrgb = (int)(lambuda * sImageData[y * nRowBytes + x * 3 + 0] - (1 - lambuda) * lsImageData[y * nRowBytes + x * 3 + 0]);
			nrgb = (nrgb < 0) ? 0 : ((nrgb > 255) ? 255 : nrgb);
			rImageData[y * nRowBytes + x * 3 + 0] = nrgb;
			nrgb = (int)(lambuda * sImageData[y * nRowBytes + x * 3 + 1] - (1 - lambuda) * lsImageData[y * nRowBytes + x * 3 + 1]);
			nrgb = (nrgb < 0) ? 0 : ((nrgb > 255) ? 255 : nrgb);
			rImageData[y * nRowBytes + x * 3 + 1] = nrgb;
			nrgb = (int)(lambuda * sImageData[y * nRowBytes + x * 3 + 2] - (1 - lambuda) * lsImageData[y * nRowBytes + x * 3 + 2]);
			nrgb = (nrgb < 0) ? 0 : ((nrgb > 255) ? 255 : nrgb);
			rImageData[y * nRowBytes + x * 3 + 2] = nrgb;
		}

	lsImage.Destroy();

	UpdateAllViews(NULL);
}



void CimageProcessingDoc::OnImageMul()
{
	// TODO: 在此添加命令处理程序代码
	if (m_sImage.IsNull())
	{
		AfxMessageBox(_T("请打开要处理的图像！"));
		return;
	}

	CImage lsImage;
	CString strFilter;
	CSimpleArray<GUID>  aguidFileTypes;
	HRESULT hResult;

	// 获取CImage支持的图像文件的过滤字符串
	hResult = m_sImage.GetExporterFilterString(strFilter, aguidFileTypes, _T("All Image Files"));
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("GetExporterFilter函数失败!"));
		return;
	}

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	if (IDOK != dlg.DoModal())
		return;

	lsImage.Destroy();

	// 将外部图像文件装载到CImage对象中
	hResult = lsImage.Load(dlg.GetPathName());
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("图像文件加载失败！"));
		return;
	}

	if (lsImage.GetBPP() != 24)
	{
		lsImage.Destroy();
		AfxMessageBox(_T("只处理24位色图像！"));
		return;
	}

	if ((lsImage.GetHeight() != m_sImage.GetHeight()) || (lsImage.GetWidth() != m_sImage.GetWidth()))
	{
		lsImage.Destroy();
		AfxMessageBox(_T("本软件只处理相同大小图像的逻辑与代数运算！"));
		return;
	}

	int x;
	int y;
	int nRows = m_sImage.GetHeight();
	int nCols = m_sImage.GetWidth();
	int nBPP = m_sImage.GetBPP();

	m_rImage.Destroy();
	if (!m_rImage.Create(nCols, nRows, nBPP))
	{
		lsImage.Destroy();
		return;
	}

	BYTE *sImageData = (BYTE *)m_sImage.GetBits();
	BYTE *lsImageData = (BYTE *)lsImage.GetBits();
	BYTE *rImageData = (BYTE *)m_rImage.GetBits();
	int nRowBytes = m_sImage.GetPitch();

	int sRowBytes = m_sImage.GetPitch();
	int nrgb;
	for (y = 0; y < nRows; y++)
		for (x = 0; x < nCols; x++)
		{
			nrgb = (int)(sImageData[y * nRowBytes + x * 3 + 0] * lsImageData[y * nRowBytes + x * 3 + 0]);
			nrgb = (nrgb < 0) ? 0 : ((nrgb > 255) ? 255 : nrgb);
			rImageData[y * nRowBytes + x * 3 + 0] = nrgb;
			nrgb = (int)(sImageData[y * nRowBytes + x * 3 + 1] * lsImageData[y * nRowBytes + x * 3 + 1]);
			nrgb = (nrgb < 0) ? 0 : ((nrgb > 255) ? 255 : nrgb);
			rImageData[y * nRowBytes + x * 3 + 1] = nrgb;
			nrgb = (int)(sImageData[y * nRowBytes + x * 3 + 2] * lsImageData[y * nRowBytes + x * 3 + 2]);
			nrgb = (nrgb < 0) ? 0 : ((nrgb > 255) ? 255 : nrgb);
			rImageData[y * nRowBytes + x * 3 + 2] = nrgb;
		}

	lsImage.Destroy();

	UpdateAllViews(NULL);
}



void CimageProcessingDoc::OnImageAnd()//与运算
{
	// TODO: 在此添加命令处理程序代码
	if (m_sImage.IsNull())
	{
		AfxMessageBox(_T("请打开要处理的图像！"));
		return;
	}

	CImage lsImage;
	CString strFilter;
	CSimpleArray<GUID>  aguidFileTypes;
	HRESULT hResult;

	// 获取CImage支持的图像文件的过滤字符串
	hResult = m_sImage.GetExporterFilterString(strFilter, aguidFileTypes, _T("All Image Files"));
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("GetExporterFilter函数失败!"));
		return;
	}

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	if (IDOK != dlg.DoModal())
		return;

	lsImage.Destroy();

	// 将外部图像文件装载到CImage对象中
	hResult = lsImage.Load(dlg.GetPathName());
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("图像文件加载失败！"));
		return;
	}

	if (lsImage.GetBPP() != 24)
	{
		lsImage.Destroy();
		AfxMessageBox(_T("只处理24位色图像！"));
		return;
	}

	if ((lsImage.GetHeight() != m_sImage.GetHeight()) || (lsImage.GetWidth() != m_sImage.GetWidth()))
	{
		lsImage.Destroy();
		AfxMessageBox(_T("本软件只处理相同大小图像的逻辑与代数运算！"));
		return;
	}

	int x;
	int y;
	int nRows = m_sImage.GetHeight();
	int nCols = m_sImage.GetWidth();
	int nBPP = m_sImage.GetBPP();

	m_rImage.Destroy();
	if (!m_rImage.Create(nCols, nRows, nBPP))
	{
		lsImage.Destroy();
		return;
	}

	BYTE *sImageData = (BYTE *)m_sImage.GetBits();
	BYTE *lsImageData = (BYTE *)lsImage.GetBits();
	BYTE *rImageData = (BYTE *)m_rImage.GetBits();
	int nRowBytes = m_sImage.GetPitch();

	int sRowBytes = m_sImage.GetPitch();
	int nrgb;
	for (y = 0; y < nRows; y++)
		for (x = 0; x < nCols; x++){
			nrgb = (sImageData[y * nRowBytes + x * 3 + 0] < lsImageData[y * nRowBytes + x * 3 + 0])
				? sImageData[y * nRowBytes + x * 3 + 0] : lsImageData[y * nRowBytes + x * 3 + 0];
			rImageData[y * nRowBytes + x * 3 + 0] = nrgb;
			nrgb = (sImageData[y * nRowBytes + x * 3 + 1] < lsImageData[y * nRowBytes + x * 3 + 1])
				? sImageData[y * nRowBytes + x * 3 + 1] : lsImageData[y * nRowBytes + x * 3 + 1];
			rImageData[y * nRowBytes + x * 3 + 1] = nrgb;
			nrgb = (sImageData[y * nRowBytes + x * 3 + 2] < lsImageData[y * nRowBytes + x * 3 + 2])
				? sImageData[y * nRowBytes + x * 3 + 2] : lsImageData[y * nRowBytes + x * 3 + 2];
			rImageData[y * nRowBytes + x * 3 + 2] = nrgb;
			
		}

	lsImage.Destroy();

	UpdateAllViews(NULL);

}


void CimageProcessingDoc::OnImageOr()//或运算
{
	// TODO: 在此添加命令处理程序代码
	if (m_sImage.IsNull())
	{
		AfxMessageBox(_T("请打开要处理的图像！"));
		return;
	}

	CImage lsImage;
	CString strFilter;
	CSimpleArray<GUID>  aguidFileTypes;
	HRESULT hResult;

	// 获取CImage支持的图像文件的过滤字符串
	hResult = m_sImage.GetExporterFilterString(strFilter, aguidFileTypes, _T("All Image Files"));
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("GetExporterFilter函数失败!"));
		return;
	}

	CFileDialog dlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST, strFilter);
	if (IDOK != dlg.DoModal())
		return;

	lsImage.Destroy();

	// 将外部图像文件装载到CImage对象中
	hResult = lsImage.Load(dlg.GetPathName());
	if (FAILED(hResult))
	{
		AfxMessageBox(_T("图像文件加载失败！"));
		return;
	}

	if (lsImage.GetBPP() != 24)
	{
		lsImage.Destroy();
		AfxMessageBox(_T("只处理24位色图像！"));
		return;
	}

	if ((lsImage.GetHeight() != m_sImage.GetHeight()) || (lsImage.GetWidth() != m_sImage.GetWidth()))
	{
		lsImage.Destroy();
		AfxMessageBox(_T("本软件只处理相同大小图像的逻辑与代数运算！"));
		return;
	}

	int x;
	int y;
	int nRows = m_sImage.GetHeight();
	int nCols = m_sImage.GetWidth();
	int nBPP = m_sImage.GetBPP();

	m_rImage.Destroy();
	if (!m_rImage.Create(nCols, nRows, nBPP))
	{
		lsImage.Destroy();
		return;
	}

	BYTE *sImageData = (BYTE *)m_sImage.GetBits();
	BYTE *lsImageData = (BYTE *)lsImage.GetBits();
	BYTE *rImageData = (BYTE *)m_rImage.GetBits();
	int nRowBytes = m_sImage.GetPitch();

	int sRowBytes = m_sImage.GetPitch();
	int nrgb;
	for (y = 0; y < nRows; y++)
		for (x = 0; x < nCols; x++) {
			nrgb = (sImageData[y * nRowBytes + x * 3 + 0] > lsImageData[y * nRowBytes + x * 3 + 0])
				? sImageData[y * nRowBytes + x * 3 + 0] : lsImageData[y * nRowBytes + x * 3 + 0];
			rImageData[y * nRowBytes + x * 3 + 0] = nrgb;
			nrgb = (sImageData[y * nRowBytes + x * 3 + 1] > lsImageData[y * nRowBytes + x * 3 + 1])
				? sImageData[y * nRowBytes + x * 3 + 1] : lsImageData[y * nRowBytes + x * 3 + 1];
			rImageData[y * nRowBytes + x * 3 + 1] = nrgb;
			nrgb = (sImageData[y * nRowBytes + x * 3 + 2] > lsImageData[y * nRowBytes + x * 3 + 2])
				? sImageData[y * nRowBytes + x * 3 + 2] : lsImageData[y * nRowBytes + x * 3 + 2];
			rImageData[y * nRowBytes + x * 3 + 2] = nrgb;

		}

	lsImage.Destroy();

	UpdateAllViews(NULL);

}
