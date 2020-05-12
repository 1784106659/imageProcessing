#pragma once


// CDialogImageAddRatio 对话框

class CDialogImageAddRatio : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogImageAddRatio)

public:
	CDialogImageAddRatio(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDialogImageAddRatio();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_IMAGE_ADD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float addRadio;
	CSliderCtrl m_slider;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
