// CDialogImageAddRatio.cpp: 实现文件
//

#include "pch.h"
#include "imageProcessing.h"
#include "CDialogImageAddRatio.h"
#include "afxdialogex.h"


// CDialogImageAddRatio 对话框

IMPLEMENT_DYNAMIC(CDialogImageAddRatio, CDialogEx)

CDialogImageAddRatio::CDialogImageAddRatio(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_IMAGE_ADD, pParent)
{

}

CDialogImageAddRatio::~CDialogImageAddRatio()
{
}

void CDialogImageAddRatio::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, addRadio);
	DDX_Control(pDX, IDC_SLIDER_ADD, m_slider);
}


BEGIN_MESSAGE_MAP(CDialogImageAddRatio, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDialogImageAddRatio::OnBnClickedOk)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDialogImageAddRatio 消息处理程序


void CDialogImageAddRatio::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


BOOL CDialogImageAddRatio::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_slider.SetRange(0,100);
	m_slider.SetTicFreq(5);
	m_slider.SetPos(50);
	m_slider.SetLineSize(10);
	UpdateData(true);
	addRadio = m_slider.GetPos();
	UpdateData(false);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDialogImageAddRatio::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSliderCtrl* slider = (CSliderCtrl*)pScrollBar;
	if (slider = &m_slider)
	{
		UpdateData(TRUE);
		addRadio = m_slider.GetPos();
		UpdateData(FALSE);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
