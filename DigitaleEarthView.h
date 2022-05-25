// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// DigitaleEarthView.h : CDigitaleEarthView 类的接口
//

#pragma once
#include "OsgObject.h"

class CDigitaleEarthView : public CView
{
protected: // 仅从序列化创建
	CDigitaleEarthView();
	DECLARE_DYNCREATE(CDigitaleEarthView)

// 特性
public:
	CDigitaleEarthDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CDigitaleEarthView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	cOSG* mOSG;
	//HANDLE mThreadHandle;
	CRenderingThread* mThreadHandle;

private:
	bool isShowBound_;

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnChinaBound();
	afx_msg void OnSetBound();
	afx_msg void OnUpdateShowBound(CCmdUI *pCmdUI);
	afx_msg void OnShowBound();
	afx_msg void OnButton5();
	afx_msg void OnEditLongitude();
	afx_msg void OnUpdateEditLongitude(CCmdUI *pCmdUI);
	afx_msg void OnEditLatitude();
	afx_msg void OnEditAltitude();
};

#ifndef _DEBUG  // DigitaleEarthView.cpp 中的调试版本
inline CDigitaleEarthDoc* CDigitaleEarthView::GetDocument() const
   { return reinterpret_cast<CDigitaleEarthDoc*>(m_pDocument); }
#endif

