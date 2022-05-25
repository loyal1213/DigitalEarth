// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// DigitaleEarthView.h : CDigitaleEarthView ��Ľӿ�
//

#pragma once
#include "OsgObject.h"

class CDigitaleEarthView : public CView
{
protected: // �������л�����
	CDigitaleEarthView();
	DECLARE_DYNCREATE(CDigitaleEarthView)

// ����
public:
	CDigitaleEarthDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
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

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // DigitaleEarthView.cpp �еĵ��԰汾
inline CDigitaleEarthDoc* CDigitaleEarthView::GetDocument() const
   { return reinterpret_cast<CDigitaleEarthDoc*>(m_pDocument); }
#endif

