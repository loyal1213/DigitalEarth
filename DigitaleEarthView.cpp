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

// DigitaleEarthView.cpp : CDigitaleEarthView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "DigitaleEarth.h"
#endif

#include "DigitaleEarthDoc.h"
#include "DigitaleEarthView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDigitaleEarthView

IMPLEMENT_DYNCREATE(CDigitaleEarthView, CView)

BEGIN_MESSAGE_MAP(CDigitaleEarthView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CDigitaleEarthView ����/����

CDigitaleEarthView::CDigitaleEarthView(): mOSG(0L)
{
	// TODO: �ڴ˴���ӹ������

}

CDigitaleEarthView::~CDigitaleEarthView()
{
}

BOOL CDigitaleEarthView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CDigitaleEarthView ����

void CDigitaleEarthView::OnDraw(CDC* /*pDC*/)
{
	CDigitaleEarthDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CDigitaleEarthView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDigitaleEarthView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDigitaleEarthView ���

#ifdef _DEBUG
void CDigitaleEarthView::AssertValid() const
{
	CView::AssertValid();
}

void CDigitaleEarthView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDigitaleEarthDoc* CDigitaleEarthView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDigitaleEarthDoc)));
	return (CDigitaleEarthDoc*)m_pDocument;
}
#endif //_DEBUG


// CDigitaleEarthView ��Ϣ�������


int CDigitaleEarthView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	// Now that the window is created setup OSG
	mOSG = new cOSG(m_hWnd);

	return 1;
}


void CDigitaleEarthView::OnDestroy()
{
	// TODO: �ڴ˴������Ϣ����������
	delete mThreadHandle;
	if(mOSG != 0){
		delete mOSG;
		mOSG = nullptr;
	}
	CView::OnDestroy();
}


void CDigitaleEarthView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// Close Window on Escape Key
	if(nChar == VK_ESCAPE){
		GetParent()->SendMessage(WM_CLOSE);
	}
	// CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CDigitaleEarthView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//BOOL ret = CView::OnEraseBkgnd(pDC);
	//return ret;
	/* Do nothing, to avoid flashing on MSW */
	// ����������˱������򷵻ط���ֵ�����򷵻�0��
	return true;
}


void CDigitaleEarthView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	// Get Filename from DocumentOpen Dialog
	CString csFileName = GetDocument()->GetFileName();
	if (csFileName.IsEmpty()){
		csFileName = "gdal_interp.earth";
	}

	// Init the osg class
	mOSG->InitOSG(csFileName.GetString());

	// Start the thread to do OSG Rendering
	//mThreadHandle = (HANDLE)_beginthread(&cOSG::Render, 0, mOSG); 
	mThreadHandle = new CRenderingThread(mOSG);
	mThreadHandle->start();
}
