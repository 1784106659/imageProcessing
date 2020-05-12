// CDialogScalingFactor.cpp: 实现文件
//

#include "pch.h"
#include "imageProcessing.h"
#include "DialogScalingFactor.h"
#include "afxdialogex.h"


// CDialogScalingFactor 对话框

IMPLEMENT_DYNAMIC(CDialogScalingFactor, CDialogEx)

CDialogScalingFactor::CDialogScalingFactor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SCALING_FACTOR, pParent)
	, mHoriScalingFactor(0)
	, mVeriScalingFactor(0)
{

}

CDialogScalingFactor::~CDialogScalingFactor()
{
}

void CDialogScalingFactor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HORI_SCALING_FACTOR, mHoriScalingFactor);
	DDX_Text(pDX, IDC_EDIT_VERI_SCALING_FACTOR, mVeriScalingFactor);
}


BEGIN_MESSAGE_MAP(CDialogScalingFactor, CDialogEx)
END_MESSAGE_MAP()


// CDialogScalingFactor 消息处理程序
