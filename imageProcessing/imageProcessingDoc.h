
// imageProcessingDoc.h: CimageProcessingDoc 类的接口
//


#pragma once


class CimageProcessingDoc : public CDocument
{
protected: // 仅从序列化创建
	CimageProcessingDoc() noexcept;
	DECLARE_DYNCREATE(CimageProcessingDoc)

// 特性
public:
	CImage m_sImage;    //原始图
	CImage m_rImage;     //处理后图
	const double PI = 3.1415927;

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CimageProcessingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnFileOpen();
	afx_msg void OnNearestInterpolationScaling();
	afx_msg void OnRed();
	afx_msg void OnGreen();
	afx_msg void OnBlue();
	afx_msg void OnColorToGrey();
	afx_msg void OnImageAdd();
	afx_msg void OnHsvHue();
	afx_msg void OnHsvSaturation();
};
