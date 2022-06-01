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
#include "MainFrm.h"

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
	ON_COMMAND(ID_CHINA_BOUND, &CDigitaleEarthView::OnChinaBound)
	ON_COMMAND(ID_SET_BOUND, &CDigitaleEarthView::OnSetBound)
	ON_UPDATE_COMMAND_UI(ID_SHOW_BOUND, &CDigitaleEarthView::OnUpdateShowBound)
	ON_COMMAND(ID_SHOW_BOUND, &CDigitaleEarthView::OnShowBound)
	ON_COMMAND(ID_BUTTON5, &CDigitaleEarthView::OnButton5)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LONGITUDE, &CDigitaleEarthView::OnUpdateEditLongitude)
	ON_COMMAND(ID_CHECK_START_AIRPLANE, &CDigitaleEarthView::OnCheckStartAirplane)
	ON_UPDATE_COMMAND_UI(ID_CHECK_START_AIRPLANE, &CDigitaleEarthView::OnUpdateCheckStartAirplane)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LATITUDE, &CDigitaleEarthView::OnUpdateEditLatitude)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ALTITUDE, &CDigitaleEarthView::OnUpdateEditAltitude)
	ON_COMMAND(ID_EDIT_LONGITUDE, &CDigitaleEarthView::OnEditLongitude)
	ON_COMMAND(ID_EDIT_LATITUDE, &CDigitaleEarthView::OnEditLatitude)
	ON_COMMAND(ID_EDIT_ALTITUDE, &CDigitaleEarthView::OnEditAltitude)
//	ON_COMMAND(ID_CHECK_TRACK, &CDigitaleEarthView::OnCheckTrack)
//ON_UPDATE_COMMAND_UI(ID_CHECK_TRACK, &CDigitaleEarthView::OnUpdateCheckTrack)
//ON_COMMAND(ID_CHECK_TRACK, &CDigitaleEarthView::OnCheckTrack)
ON_COMMAND(ID_CHECK_TRACK, &CDigitaleEarthView::OnCheckTrack)
ON_UPDATE_COMMAND_UI(ID_CHECK_TRACK, &CDigitaleEarthView::OnUpdateCheckTrack)
END_MESSAGE_MAP()

// CDigitaleEarthView ����/����

CDigitaleEarthView::CDigitaleEarthView(): mOSG(0L),
	mThreadHandle(nullptr),
	fly_longitude_(112),
	fly_latitude_(33),
	fly_altitude_(4000),
	isFillWorld_(false),
	isShowContour_(false),
	isShowBound_(true),
	isStartFly_(false),
	isTrack_(false)
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
#ifdef SHARED_HANDLERS
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
	CView::OnDestroy();
	WaitForSingleObject(mThreadHandle, 1000);
	delete mThreadHandle;
	if(mOSG != 0){
		delete mOSG;
		mOSG = nullptr;
	}

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
	if (0 == mOSG){
		return CView::OnEraseBkgnd(pDC);
	}else{
		return FALSE;
	}

	return CView::OnEraseBkgnd(pDC);
}


void CDigitaleEarthView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	// Get Filename from DocumentOpen Dialog
	CString csFileName = GetDocument()->GetFileName();
	if (csFileName.IsEmpty()){
		csFileName = "./data/earth_file/china_simple.earth";
	}

	// Init the osg class
	mOSG->InitOSG(csFileName.GetString());

	// Start the thread to do OSG Rendering
	//mThreadHandle = (HANDLE)_beginthread(&cOSG::Render, 0, mOSG); 
	mThreadHandle = new CRenderingThread(mOSG);
	mThreadHandle->start();
}


void CDigitaleEarthView::OnChinaBound()
{
	// TODO: �ڴ���������������
}


void CDigitaleEarthView::OnSetBound()
{
	// TODO: �ڴ���������������
	CDigitaleEarthApp* p_app = (CDigitaleEarthApp*)AfxGetApp();
	CMainFrame* p_wnd = (CMainFrame*)p_app->GetMainWnd();
	CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->m_wndRibbonBar.FindByID(ID_CHINA_BOUND));
	if (p_edit){
		CString str  = p_edit->GetEditText();
		std::string strTemp(str.GetBuffer());
		double opt = std::atof(strTemp.c_str());
		if (opt<0){
			AfxMessageBox("����͸���ȱ���Ϊ��ֵ",MB_OK,MB_ICONEXCLAMATION);
		}else{
			mOSG->set_boundaries(opt);
		}
	}
}


void CDigitaleEarthView::OnUpdateShowBound(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(isShowBound_);
}


void CDigitaleEarthView::OnShowBound()
{
	isShowBound_ = !isShowBound_;
	if (isShowBound_){
		mOSG->addWorldBound();
	}else{
		mOSG->rmWorldBound();
	}
}

// ����Ŀ�ĵ�
void CDigitaleEarthView::OnButton5()
{
	// double longitude=0.0f, latitude=0.0f, altitude=0.0f;
	CDigitaleEarthApp* p_app = (CDigitaleEarthApp*)AfxGetApp();
	CMainFrame* p_wnd = (CMainFrame*)p_app->GetMainWnd();
	{  // ��ȡ����
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->m_wndRibbonBar.FindByID(ID_EDIT_LONGITUDE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			fly_longitude_ = std::atof(strTemp.c_str());
			if ((fly_longitude_<-180) || (fly_longitude_>180)){
				AfxMessageBox("���ȱ������(-180,180)֮�� !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}

	}

	{	// ��ȡγ��
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->m_wndRibbonBar.FindByID(ID_EDIT_LATITUDE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			fly_latitude_ = std::atof(strTemp.c_str());
			if ((fly_latitude_<-90) || (fly_latitude_>90)){
				AfxMessageBox("γ�ȱ�����ڣ�-90,90��֮�� !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	{	// ��ȡ�߶�
		CMFCRibbonEdit *p_edit = dynamic_cast<CMFCRibbonEdit*>(p_wnd->m_wndRibbonBar.FindByID(ID_EDIT_ALTITUDE));
		if (p_edit){
			CString str  = p_edit->GetEditText();
			std::string strTemp(str.GetBuffer());
			fly_altitude_ = std::atof(strTemp.c_str());
			if ((fly_altitude_<0)){
				AfxMessageBox("�߶ȱ���Ϊ��ֵ !",MB_OK,MB_ICONEXCLAMATION);
				return ;
			}
		}
	}

	mOSG->FlyTo(fly_longitude_,fly_latitude_,fly_altitude_);
}

void CDigitaleEarthView::OnUpdateEditLongitude(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}

void CDigitaleEarthView::OnCheckStartAirplane()
{
	// TODO: �ڴ���������������
	isStartFly_ = !isStartFly_;
	TRACE("OnCheckStartAirplane 1111111111\n");
	if (isStartFly_){	// �����ʱ�򴥷�
		// ��ʼ���
		mOSG->DoPreLineNow(); 
	}
	
}


void CDigitaleEarthView::OnUpdateCheckStartAirplane(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(isStartFly_);		//  һֱ����ˢ��
	// TRACE("OnUpdateCheckStartAirplane 222222222\n");
}


void CDigitaleEarthView::OnUpdateEditLatitude(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CDigitaleEarthView::OnUpdateEditAltitude(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
}


void CDigitaleEarthView::OnEditLongitude()
{
	// TODO: �ڴ���������������
}


void CDigitaleEarthView::OnEditLatitude()
{
	// TODO: �ڴ���������������
}


void CDigitaleEarthView::OnEditAltitude()
{
	// TODO: �ڴ���������������
}


//void CDigitaleEarthView::OnUpdateCheckTrack(CCmdUI *pCmdUI)
//{
//	// TODO: �ڴ������������û����洦��������
//	pCmdUI->SetCheck(isTrack_);
//	if (!isTrack_){
//		pCmdUI->Enable(false);
//	}
//}


//void CDigitaleEarthView::OnCheckTrack()
//{
//	// TODO: �ڴ���������������
//	isTrack_ = !isTrack_;
//	mOSG->IsTrack(isTrack_);
//
//}


void CDigitaleEarthView::OnCheckTrack()
{
	isTrack_ = !isTrack_;
	if (isTrack_){
		mOSG->IsTrack(isTrack_);
	}
}


void CDigitaleEarthView::OnUpdateCheckTrack(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(isTrack_);
}
