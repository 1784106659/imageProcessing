#pragma once


// CDialogScalingFactor 对话框

class CDialogScalingFactor : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogScalingFactor)

public:
	CDialogScalingFactor(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogScalingFactor();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCALING_FACTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float mHoriScalingFactor;
	float mVeriScalingFactor;
};
